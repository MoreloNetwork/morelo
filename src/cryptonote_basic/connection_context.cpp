// Copyright (c) 2020, The Monero Project
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

#include <limits>

#include "connection_context.h"

#include "cryptonote_protocol/cryptonote_protocol_defs.h"
#include "p2p/p2p_protocol_defs.h"

namespace cryptonote
{
  std::size_t cryptonote_connection_context::get_max_bytes(const int command) noexcept
  {
    // Commands carrying blocks or transactions (NOTIFY_NEW_BLOCK,
    // NOTIFY_NEW_TRANSACTIONS, NOTIFY_RESPONSE_GET_OBJECTS and
    // NOTIFY_NEW_FLUFFY_BLOCK, which carries missing txs) are only bounded by
    // the global packet limit, since block size follows the dynamic median.
    switch (command)
    {
    case nodetool::COMMAND_HANDSHAKE_T<cryptonote::CORE_SYNC_DATA>::ID:
      return 256 * 1024; // 251 peerlist entries are < 50 kB even with onion/i2p hosts
    case nodetool::COMMAND_TIMED_SYNC_T<cryptonote::CORE_SYNC_DATA>::ID:
      return 256 * 1024;
    case nodetool::COMMAND_PING::ID:
      return 4096;
    case nodetool::COMMAND_REQUEST_SUPPORT_FLAGS::ID:
      return 4096;
    case cryptonote::NOTIFY_REQUEST_GET_OBJECTS::ID:
      return 1024 * 1024 * 2; // 2 MB, 65536 hashes (requests are <= 2048)
    case cryptonote::NOTIFY_REQUEST_CHAIN::ID:
      return 512 * 1024; // 512 kB, short chain history is ~40 hashes
    case cryptonote::NOTIFY_RESPONSE_CHAIN_ENTRY::ID:
      return 1024 * 1024 * 4; // 4 MB, up to 10000 hashes + weights is ~400 kB
    case cryptonote::NOTIFY_REQUEST_FLUFFY_MISSING_TX::ID:
      return 1024 * 1024; // 1 MB, 8 bytes per missing tx index
    default:
      break;
    };
    return std::numeric_limits<size_t>::max();
  }

  void cryptonote_connection_context::set_state_normal()
  {
    m_state = state_normal;
    m_expected_heights_start = 0;
    m_needed_objects.clear();
    m_needed_objects.shrink_to_fit();
    m_expected_heights.clear();
    m_expected_heights.shrink_to_fit();
    m_requested_objects.clear();
  }

  boost::optional<crypto::hash> cryptonote_connection_context::get_expected_hash(const uint64_t height) const
  {
    const auto difference = height - m_expected_heights_start;
    if (height < m_expected_heights_start || m_expected_heights.size() <= difference)
      return boost::none;
    return m_expected_heights[difference];
  }
} // cryptonote
