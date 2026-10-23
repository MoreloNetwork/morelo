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

#include "bootstrap_import.h"
#include "bootstrap_serialization.h"
#include "cryptonote_core.h"
#include "cryptonote_basic/cryptonote_format_utils.h"
#include "serialization/binary_utils.h"

#undef MORELO_DEFAULT_LOG_CATEGORY
#define MORELO_DEFAULT_LOG_CATEGORY "bootstrap"

namespace cryptonote
{
  bool bootstrap_chunk_to_entry(const std::string &chunk, block_complete_entry &entry)
  {
    bootstrap::block_package bp;
    if (!::serialization::parse_binary(chunk, bp))
    {
      MERROR("Failed to parse a block from the bootstrap file");
      return false;
    }
    entry.pruned = false;
    entry.block = block_to_blob(bp.block);
    entry.txs.clear();
    for (const auto &tx: bp.txs)
      entry.txs.push_back({tx_to_blob(tx), crypto::null_hash});
    return true;
  }

  bool bootstrap_add_blocks(core &core, std::vector<block_complete_entry> &blocks, bool force, uint64_t batch_size)
  {
    if (blocks.empty())
      return true;
    if (!force && blocks.size() < batch_size)
      return true;

    // wait till we can verify a full HOH without extra, for speed
    const uint64_t start_height = core.get_blockchain_storage().get_db().height();
    uint64_t new_height = start_height + blocks.size();
    if (!force && new_height % HASH_OF_HASHES_STEP)
      return true;

    // inside the compiled block hash area we only ever flush on a hash of hashes
    // boundary, so that every block we add can be checked against the compiled
    // hashes below: a trailing partial group could not be, and would be added on
    // the strength of the file alone
    if (new_height % HASH_OF_HASHES_STEP && core.is_within_compiled_block_hash_area(new_height))
    {
      const uint64_t aligned_height = new_height - new_height % HASH_OF_HASHES_STEP;
      if (aligned_height <= start_height)
      {
        MWARNING("Not importing blocks " << start_height << " - " << (new_height - 1) << ": the file ends inside the compiled block hash area at a height which cannot be checked against it, these blocks will be synced from the network");
        blocks.clear();
        return true;
      }
      MWARNING("Not importing blocks " << aligned_height << " - " << (new_height - 1) << ": the file ends inside the compiled block hash area at a height which cannot be checked against it, these blocks will be synced from the network");
      blocks.resize(aligned_height - start_height);
      new_height = aligned_height;
    }

    std::vector<crypto::hash> hashes;
    for (const auto &b: blocks)
    {
      cryptonote::block block;
      if (!parse_and_validate_block_from_blob(b.block, block))
      {
        MERROR("Failed to parse block: "
            << epee::string_tools::pod_to_hex(get_blob_hash(b.block)));
        core.cleanup_handle_incoming_blocks();
        return false;
      }
      hashes.push_back(cryptonote::get_block_hash(block));
    }
    // outside the compiled block hash area this accepts everything, inside it a
    // mismatch means the file does not carry the chain these hashes were built
    // from, so do not let any of these blocks near the database
    const uint64_t usable = core.prevalidate_block_hashes(start_height, hashes, {});
    if (usable != hashes.size())
    {
      MERROR("Blocks " << start_height << " - " << (new_height - 1) << " do not match the compiled block hashes (" << usable << "/" << hashes.size() << " usable)");
      return false;
    }

    std::vector<block> pblocks;
    if(!core.prepare_handle_incoming_blocks(blocks, pblocks))
    {
      MERROR("Failed to prepare to add blocks");
      return false;
    }
    if(!pblocks.empty() && pblocks.size() != blocks.size())
    {
      MERROR("Unexpected parsed blocks size");
      core.cleanup_handle_incoming_blocks();
      return false;
    }

    size_t blockidx = 0;
    for(const block_complete_entry& block_entry: blocks)
    {
      // process transactions
      for(auto& tx_blob: block_entry.txs)
      {
        tx_verification_context tvc = AUTO_VAL_INIT(tvc);
        core.handle_incoming_tx(tx_blob, tvc, true, true, false);
        if(tvc.m_verifivation_failed)
        {
          MERROR("transaction verification failed, tx_id = "
              << epee::string_tools::pod_to_hex(get_blob_hash(tx_blob.blob)));
          core.cleanup_handle_incoming_blocks();
          return false;
        }
      }

      // process block

      block_verification_context bvc = {};

      core.handle_incoming_block(block_entry.block, pblocks.empty() ? NULL : &pblocks[blockidx++], bvc, false); // <--- process block

      if(bvc.m_verifivation_failed)
      {
        MERROR("Block verification failed, id = "
            << epee::string_tools::pod_to_hex(get_blob_hash(block_entry.block)));
        core.cleanup_handle_incoming_blocks();
        return false;
      }
      if(bvc.m_marked_as_orphaned)
      {
        MERROR("Block received at sync phase was marked as orphaned");
        core.cleanup_handle_incoming_blocks();
        return false;
      }

    } // each download block
    if (!core.cleanup_handle_incoming_blocks())
      return false;

    blocks.clear();
    return true;
  }
}
