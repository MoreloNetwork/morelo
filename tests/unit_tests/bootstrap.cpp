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

#include "gtest/gtest.h"

#include "cryptonote_basic/cryptonote_format_utils.h"
#include "cryptonote_core/bootstrap_import.h"
#include "cryptonote_core/bootstrap_serialization.h"
#include "cryptonote_core/cryptonote_tx_utils.h"
#include "serialization/binary_utils.h"

TEST(bootstrap, chunk_only_carries_its_block_transactions)
{
  cryptonote::bootstrap::block_package bp;
  ASSERT_TRUE(cryptonote::generate_genesis_block(bp.block));
  bp.block_weight = 0;
  bp.cumulative_difficulty = 1;
  bp.coins_generated = 0;
  const auto chunk = [&bp]() { std::string blob; EXPECT_TRUE(::serialization::dump_binary(bp, blob)); return blob; };

  cryptonote::block_complete_entry entry;
  ASSERT_TRUE(cryptonote::bootstrap_chunk_to_entry(chunk(), entry));
  ASSERT_EQ(cryptonote::block_to_blob(bp.block), entry.block);
  ASSERT_TRUE(entry.txs.empty());

  cryptonote::transaction tx;
  tx.unlock_time = 1;
  bp.txs.push_back(tx);
  ASSERT_FALSE(cryptonote::bootstrap_chunk_to_entry(chunk(), entry));

  bp.block.tx_hashes.push_back(cryptonote::get_transaction_hash(tx));
  ASSERT_TRUE(cryptonote::bootstrap_chunk_to_entry(chunk(), entry));
  ASSERT_EQ(1, entry.txs.size());
  ASSERT_EQ(cryptonote::tx_to_blob(tx), entry.txs[0].blob);

  bp.block.tx_hashes[0] = crypto::null_hash;
  ASSERT_FALSE(cryptonote::bootstrap_chunk_to_entry(chunk(), entry));

  ASSERT_FALSE(cryptonote::bootstrap_chunk_to_entry("garbage", entry));
}
