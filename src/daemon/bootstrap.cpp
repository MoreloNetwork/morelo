// Copyright (c) 2018-2020, The Arqma Network
// Copyright (c) 2014-2020, The Monero Project
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this list of
//    conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice, this list
//    of conditions and the following disclaimer in the documentation and/or other
//    materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its contributors may be
//    used to endorse or promote products derived from this software without specific
//    prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
// THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
// THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <algorithm>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include "common/dns_utils.h"
#include "cryptonote_core/bootstrap_import.h"
#include "cryptonote_core/cryptonote_core.h"
#include "daemon/bootstrap.h"
#include "daemon/command_line_args.h"
#include "file_io_utils.h"
#include "net/http_client.h"
#include "net/net_parse_helpers.h"

#undef MORELO_DEFAULT_LOG_CATEGORY
#define MORELO_DEFAULT_LOG_CATEGORY "daemon.bootstrap"

namespace daemonize
{
namespace
{
  // blocks per database transaction, a stop request waits for at most this many to be
  // verified. Inside the compiled block hash area batches also end on a multiple of
  // HASH_OF_HASHES_STEP, so that all of a batch can be checked against those hashes
  const uint64_t batch_size = 1024;
  // a batch also ends at the next such boundary once its blocks take flush_bytes, and
  // a mirror is dropped if they reach max_pending_bytes first: the largest 512 blocks
  // on mainnet take 21.5 MB
  const uint64_t flush_bytes = 64 * 1024 * 1024;
  const uint64_t max_pending_bytes = 256 * 1024 * 1024;
  // to connect, for the TLS handshake and for each read
  const std::chrono::seconds timeout(30);
  const std::chrono::seconds progress_interval(30);
  // a mirror sending under 1 MB/s, time spent verifying aside, is left for the next
  // one: peers wait for the bootstrap to end
  const uint64_t min_window_bytes = 60 * 1024 * 1024;
  const std::chrono::seconds window(60);
  const size_t max_dns_mirrors = 8;
  // a crash leaves the saved position at most a batch behind the blockchain, further
  // behind the node synced on without the bootstrap (eg. with --no-bootstrap)
  const uint64_t max_resume_lag = 2 * batch_size;

  // where the import resumes, every block before it is in the database
  struct position
  {
    uint64_t offset; // in the file, 0 is its start, before the header
    uint64_t height; // of the block at offset
  };

  void save_state(const std::string &path, const position &pos)
  {
    // written aside and renamed over, so a crash leaves either the old or the new one
    const std::string tmp = path + ".tmp";
    boost::system::error_code ec;
    if (epee::file_io_utils::save_string_to_file(tmp, std::to_string(pos.offset) + " " + std::to_string(pos.height) + "\n"))
      boost::filesystem::rename(tmp, path, ec);
  }

  std::vector<std::string> get_mirrors(const boost::program_options::variables_map &vm, cryptonote::network_type nettype)
  {
    std::vector<std::string> urls = command_line::get_arg(vm, daemon_args::arg_bootstrap_url);
    if (urls.empty() && nettype == cryptonote::MAINNET)
    {
      // checked like the update and checkpoint records, they hold space separated URLs
      static const std::vector<std::string> dns_urls = {
        "bootstrap.morelonetwork.pl", "bootstrap.morelo.vip", "bootstrap.mrl.laj.lol"
      };
      std::vector<std::string> records;
      if (tools::dns_utils::load_txt_records_from_dns(records, dns_urls))
      {
        for (const auto &record: records)
        {
          std::vector<std::string> parts;
          boost::split(parts, record, boost::is_any_of(" "), boost::token_compress_on);
          for (const auto &part: parts)
            if (!part.empty())
              urls.push_back(part);
        }
      }
      // spread the load over the mirrors
      std::shuffle(urls.begin(), urls.end(), crypto::random_device{});
      if (urls.size() > max_dns_mirrors)
        urls.resize(max_dns_mirrors);
    }

    std::vector<std::string> mirrors;
    for (const auto &url: urls)
    {
      epee::net_utils::http::url_content u;
      if (epee::net_utils::parse_url(url, u) && u.schema == "https" && !u.host.empty())
        mirrors.push_back(url);
      else
        MWARNING("Ignoring bootstrap mirror " << url << ", only https URLs are accepted");
    }
    return mirrors;
  }

  enum class outcome
  {
    done,   // the mirror has nothing more
    failed, // it failed without adding blocks, or is too slow
    retry,  // it failed after adding blocks, eg. the server gave up on us while we were verifying
    bad     // it sent invalid data
  };

  class mirror_client: public epee::net_utils::http::http_simple_client
  {
  public:
    mirror_client(cryptonote::core &core, position &resume, const std::string &state_path, const std::function<bool()> &stop):
      m_core(core), m_resume(resume), m_state_path(state_path), m_stop(stop), m_last_progress(std::chrono::steady_clock::now()) {}

    //! imports what the mirror has past the resume point, or past the blocks we have
    //! reading the file from its start
    outcome fetch(const std::string &url, bool from_start)
    {
      epee::net_utils::http::url_content u;
      epee::net_utils::parse_url(url, u);
      const uint64_t start_height = m_core.get_current_blockchain_height();
      m_from = from_start ? position{0, 0} : m_resume;
      m_stream = cryptonote::bootstrap_stream(m_from.offset);
      m_height = m_from.height;
      m_skip_below = start_height;
      m_bad = false;
      m_slow = false;
      m_status = 0;
      m_start = m_from.offset;
      m_length = -1;
      m_received = 0;
      m_blocks.clear();
      m_pending_bytes = 0;
      m_window_start = std::chrono::steady_clock::now();
      m_window_busy = std::chrono::steady_clock::duration::zero();
      m_window_bytes = 0;

      MGINFO("Bootstrapping from " << url << " at height " << start_height << (from_start ? ", reading the file from its start" : ""));
      set_server(u.host, std::to_string(u.port ? u.port : 443), boost::none, epee::net_utils::ssl_support_t::e_ssl_support_enabled);
      epee::net_utils::http::fields_list fields;
      if (m_from.offset)
        fields.emplace_back("Range", "bytes=" + std::to_string(m_from.offset) + "-");
      const bool ok = invoke_get(u.uri.empty() ? "/" : u.uri, timeout, std::string(), nullptr, fields);
      disconnect();

      // what arrived before the transfer ended is checked and added as well
      if (!m_bad && !m_stop() && !m_blocks.empty() && !add(true))
        m_bad = true;
      m_blocks.clear();

      const uint64_t height = m_core.get_current_blockchain_height();
      if (m_bad)
      {
        MWARNING("Bootstrap mirror " << url << " sent invalid data (height " << height << ")");
        return outcome::bad;
      }
      if (m_stop())
        return outcome::failed;
      if (m_slow)
      {
        MWARNING("Bootstrap mirror " << url << " is too slow, not using it any more (height " << height << ")");
        return outcome::failed;
      }
      // nothing past our offset, or everything up to the end of the file
      if (m_status == 416 || (ok && (m_status == 200 || m_status == 206) && (m_length < 0 || m_received == (uint64_t)m_length)))
      {
        MGINFO("Done with bootstrap mirror " << url << " at height " << height);
        return outcome::done;
      }
      MWARNING("Bootstrap mirror " << url << " failed" << (m_status ? " with HTTP status " + std::to_string(m_status) : std::string()) << " (height " << height << ")");
      return height > start_height ? outcome::retry : outcome::failed;
    }

  private:
    bool on_header(const epee::net_utils::http::http_response_info &info) override
    {
      m_status = info.m_response_code;
      if (m_status == 206)
      {
        // a range must start where we asked it to
        const std::string range = "bytes " + std::to_string(m_from.offset) + "-";
        bool expected = false;
        for (const auto &field: info.m_header_info.m_etc_fields)
          if (boost::iequals(field.first, "Content-Range"))
            expected = m_from.offset && boost::starts_with(field.second, range);
        if (!expected)
        {
          MERROR("Unexpected range from bootstrap mirror");
          m_bad = true;
          return false;
        }
      }
      else if (m_status == 200 && m_from.offset)
      {
        // the server ignored the range, read the file from its start skipping the blocks we have
        m_stream = cryptonote::bootstrap_stream();
        m_height = 0;
        m_start = 0;
      }
      else if (m_status != 200)
        return false;
      if (!epee::string_tools::get_xtype_from_string(m_length, info.m_header_info.m_content_length))
        m_length = -1;
      return true;
    }

    bool handle_target_data(std::string &piece) override
    {
      if (m_stop())
        return false;
      const auto now = std::chrono::steady_clock::now();
      if (now - m_window_start - m_window_busy >= window)
      {
        if (m_window_bytes < min_window_bytes)
        {
          m_slow = true;
          return false;
        }
        m_window_start = now;
        m_window_busy = std::chrono::steady_clock::duration::zero();
        m_window_bytes = 0;
      }
      m_received += piece.size();
      m_window_bytes += piece.size();
      bool ok = false;
      try
      {
        ok = m_stream.feed(piece, [this](const std::string &chunk) { return add_chunk(chunk); });
      }
      catch (const std::exception &e)
      {
        MERROR("Failed to import from bootstrap mirror: " << e.what());
      }
      const auto done = std::chrono::steady_clock::now();
      m_window_busy += done - now;
      if (!ok)
      {
        m_bad = true;
        return false;
      }
      if (done - m_last_progress >= progress_interval)
      {
        m_last_progress = done;
        MGINFO("Bootstrap: height " << m_core.get_current_blockchain_height() << ", " << (m_start + m_received) / 1000000 << " MB"
            << (m_length >= 0 ? " of " + std::to_string((m_start + m_length) / 1000000) + " MB" : std::string()));
      }
      return true;
    }

    bool add_chunk(const std::string &chunk)
    {
      if (m_height++ < m_skip_below)
        return true; // already in the database
      cryptonote::block_complete_entry entry;
      if (!cryptonote::bootstrap_chunk_to_entry(chunk, entry))
        return false;
      m_blocks.push_back(std::move(entry));
      m_pending_bytes += chunk.size();
      if (m_pending_bytes > max_pending_bytes)
      {
        MERROR("Blocks from the bootstrap mirror take over " << max_pending_bytes / 1024 / 1024 << " MB before they can be checked");
        return false;
      }
      return add(false);
    }

    bool add(bool force)
    {
      if (!cryptonote::bootstrap_add_blocks(m_core, m_blocks, force, m_pending_bytes >= flush_bytes ? 1 : batch_size))
        return false;
      if (!m_blocks.empty())
        return true;
      m_pending_bytes = 0;
      // everything read so far is in, unless the end of it was left to peers
      if (m_core.get_current_blockchain_height() == m_height)
      {
        m_resume = {m_stream.offset(), m_height};
        save_state(m_state_path, m_resume);
      }
      return true;
    }

    cryptonote::core &m_core;
    position &m_resume;
    const std::string m_state_path;
    const std::function<bool()> m_stop;
    position m_from;       // where this transfer starts
    cryptonote::bootstrap_stream m_stream;
    std::vector<cryptonote::block_complete_entry> m_blocks;
    uint64_t m_pending_bytes;
    uint64_t m_height;     // of the next block in the stream
    uint64_t m_skip_below; // blocks below this height are in the database already
    bool m_bad;
    bool m_slow;
    int m_status;
    uint64_t m_start;      // offset of the response in the file
    int64_t m_length;
    uint64_t m_received;
    std::chrono::steady_clock::time_point m_last_progress;
    std::chrono::steady_clock::time_point m_window_start;
    std::chrono::steady_clock::duration m_window_busy;
    uint64_t m_window_bytes;
  };
}

void bootstrap(cryptonote::core &core, const boost::program_options::variables_map &vm, const std::function<bool()> &stop)
{
  // present while a bootstrap is under way, with the position it resumes from. It is
  // only removed once a mirror served all it has: after errors the next start tries again
  const std::string state_path = (boost::filesystem::path(command_line::get_arg(vm, cryptonote::arg_data_dir)) / "bootstrap.state").string();
  boost::system::error_code ec;
  try
  {
    if (core.offline() || command_line::get_arg(vm, daemon_args::arg_no_bootstrap))
      return;

    position resume = {0, 0};
    std::string state;
    bool resuming = epee::file_io_utils::load_file_to_string(state_path, state) && std::istringstream(state) >> resume.offset >> resume.height;
    const uint64_t height = core.get_current_blockchain_height();
    if (resuming && height > resume.height + max_resume_lag)
    {
      MINFO("Dropping the bootstrap state, the blockchain went on without it");
      boost::filesystem::remove(state_path, ec);
      resuming = false;
    }
    // unless the database lost blocks since, eg. a crash with --db-sync-mode fastest
    if (!resuming || resume.height > height)
      resume = {0, 0};

    const bool force = command_line::get_arg(vm, daemon_args::arg_bootstrap_force);
    if (height > 1 && !resuming && !force)
    {
      if (!command_line::get_arg(vm, daemon_args::arg_bootstrap_url).empty())
        MGINFO("Not bootstrapping, the local blockchain is not empty (see --bootstrap-force)");
      return;
    }
    if (force && !resuming)
      MWARNING("--bootstrap-force reads the bootstrap file from its start on every start, remove it once done");

    const std::vector<std::string> mirrors = get_mirrors(vm, core.get_nettype());
    if (mirrors.empty())
      return;

    MGINFO_YELLOW("Bootstrapping the blockchain from " << mirrors.size() << " mirror(s), --no-bootstrap to sync from peers only");
    save_state(state_path, resume);
    mirror_client client(core, resume, state_path, stop);
    bool completed = false;
    for (const auto &url: mirrors)
    {
      bool from_start = false;
      while (!stop())
      {
        const uint64_t mirror_height = core.get_current_blockchain_height();
        const uint64_t mirror_offset = resume.offset;
        outcome result = outcome::bad;
        try
        {
          result = client.fetch(url, from_start);
        }
        catch (const std::exception &e)
        {
          MERROR("Bootstrap mirror " << url << " failed: " << e.what());
        }
        if (result == outcome::retry)
          continue;
        // its file may not have the same layout as the one we resume, try it once from its start
        if (result == outcome::bad && !from_start && mirror_offset && core.get_current_blockchain_height() == mirror_height)
        {
          from_start = true;
          continue;
        }
        completed |= result == outcome::done;
        break;
      }
    }
    if (stop())
    {
      MGINFO("Bootstrap stopped at height " << core.get_current_blockchain_height() << ", it will resume on the next start");
      return;
    }
    if (core.get_current_blockchain_height() > height)
      MGINFO_GREEN("Bootstrap finished at height " << core.get_current_blockchain_height() << ", syncing the rest from peers");
    else
      MGINFO("No block could be bootstrapped, syncing from peers");
    if (completed)
      boost::filesystem::remove(state_path, ec);
  }
  catch (const std::exception &e)
  {
    MERROR("Bootstrap failed: " << e.what() << ", syncing from peers");
  }
}
}
