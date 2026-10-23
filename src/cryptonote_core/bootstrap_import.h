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

#pragma once

#include <string>
#include <vector>

#include "cryptonote_protocol/cryptonote_protocol_defs.h"

namespace cryptonote
{
  class core;

  // Blocks from a bootstrap file (blockchain.raw, written by morelo-blockchain-export)
  // go through the same checks as blocks synced from peers: inside the compiled block
  // hash area every group of HASH_OF_HASHES_STEP blocks must match the compiled hashes,
  // beyond it every block is fully verified. A file is never trusted.

  //! parses one chunk of a bootstrap file, false if it is malformed or carries
  //! transactions other than exactly the block's own
  bool bootstrap_chunk_to_entry(const std::string &chunk, block_complete_entry &entry);

  //! verifies and adds blocks once batch_size of them are queued and they end on a
  //! hash of hashes boundary, or now if force (inside the compiled block hash area only
  //! up to the last boundary, the rest is dropped). Returns false if a block fails,
  //! blocks are cleared once added.
  bool bootstrap_add_blocks(core &core, std::vector<block_complete_entry> &blocks, bool force, uint64_t batch_size);
}
