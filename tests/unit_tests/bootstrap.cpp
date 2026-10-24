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

namespace
{
  std::string u32(uint32_t value)
  {
    std::string blob;
    ::serialization::dump_binary(value, blob);
    return blob;
  }

  // as morelo-blockchain-export writes it: magic, then 1024 bytes starting with the file_info
  std::string file_header()
  {
    cryptonote::bootstrap::file_info info{0, 1, 1024};
    std::string blob;
    ::serialization::dump_binary(info, blob);
    std::string header = u32(cryptonote::bootstrap::blockchain_raw_magic) + u32(blob.size()) + blob;
    header.resize(4 + 1024, 0);
    return header;
  }

  std::vector<std::string> split(const std::string &data, uint64_t offset, size_t piece)
  {
    std::vector<std::string> chunks;
    cryptonote::bootstrap_stream stream(offset);
    for (size_t i = 0; i < data.size(); i += piece)
      EXPECT_TRUE(stream.feed(data.substr(i, piece), [&](const std::string &chunk) { chunks.push_back(chunk); return true; }));
    EXPECT_EQ(offset + data.size(), stream.offset());
    return chunks;
  }

  bool ignore(const std::string&) { return true; }
}

TEST(bootstrap, stream_any_piece_size)
{
  const std::vector<std::string> chunks = {"a", std::string(300, 'b'), "ccccc"};
  std::string file = file_header();
  for (const auto &chunk: chunks)
    file += u32(chunk.size()) + chunk;

  for (size_t piece = 1; piece <= file.size(); ++piece)
    ASSERT_EQ(chunks, split(file, 0, piece));

  // resuming at the second chunk
  const uint64_t second = file_header().size() + 4 + 1;
  ASSERT_EQ(std::vector<std::string>(chunks.begin() + 1, chunks.end()), split(file.substr(second), second, 7));
}

TEST(bootstrap, stream_rejects_bad_data)
{
  std::string header = file_header();
  header[0] ^= 1;
  ASSERT_FALSE(cryptonote::bootstrap_stream().feed(header, ignore));
  ASSERT_FALSE(cryptonote::bootstrap_stream().feed(file_header() + u32(0), ignore));
  ASSERT_FALSE(cryptonote::bootstrap_stream().feed(file_header() + u32(200000000), ignore));
  ASSERT_FALSE(cryptonote::bootstrap_stream().feed(file_header() + u32(1) + "a", [](const std::string&) { return false; }));

  // a truncated chunk waits for the rest
  cryptonote::bootstrap_stream stream;
  ASSERT_TRUE(stream.feed(file_header() + u32(10) + "abc", ignore));
  ASSERT_EQ(file_header().size(), stream.offset());
}

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
