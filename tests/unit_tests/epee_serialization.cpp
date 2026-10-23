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

#include <cstdint>
#include <gtest/gtest.h>

#include "storages/portable_storage.h"

TEST(epee_binary, two_keys)
{
  static constexpr const std::uint8_t data[] = {
    0x01, 0x11, 0x01, 0x1, 0x01, 0x01, 0x02, 0x1, 0x1, 0x08, 0x01, 'a',
    0x0B, 0x00, 0x01, 'b', 0x0B, 0x00
  };

  epee::serialization::portable_storage storage{};
  EXPECT_TRUE(storage.load_from_binary(data));
}

TEST(epee_binary, duplicate_key)
{
  static constexpr const std::uint8_t data[] = {
    0x01, 0x11, 0x01, 0x1, 0x01, 0x01, 0x02, 0x1, 0x1, 0x08, 0x01, 'a',
    0x0B, 0x00, 0x01, 'a', 0x0B, 0x00
  };

  epee::serialization::portable_storage storage{};
  EXPECT_FALSE(storage.load_from_binary(data));
}

TEST(epee_binary, unnamed_key)
{
  static constexpr const std::uint8_t data[] = {
    0x01, 0x11, 0x01, 0x1, 0x01, 0x01, 0x02, 0x1, 0x1, 0x04, 0x00, 0x0B, 0x00
  };

  epee::serialization::portable_storage storage{};
  EXPECT_FALSE(storage.load_from_binary(data));
}

TEST(epee_binary, array_size_sanity)
{
  // one field "a": array of uint64 claiming 2^28 elements with no data following
  static constexpr const std::uint8_t data[] = {
    0x01, 0x11, 0x01, 0x1, 0x01, 0x01, 0x02, 0x1, 0x1, 0x04, 0x01, 'a',
    0x85, 0x02, 0x00, 0x00, 0x40
  };

  epee::serialization::portable_storage storage{};
  EXPECT_FALSE(storage.load_from_binary(data));
}

TEST(epee_binary, limits)
{
  // one field "a": array of 3 empty objects
  static constexpr const std::uint8_t data[] = {
    0x01, 0x11, 0x01, 0x1, 0x01, 0x01, 0x02, 0x1, 0x1, 0x04, 0x01, 'a',
    0x8C, 0x0C, 0x00, 0x00, 0x00
  };

  epee::serialization::portable_storage storage{};
  EXPECT_TRUE(storage.load_from_binary(data));

  const epee::serialization::portable_storage::limits_t three_objects = {3, 16, 16};
  EXPECT_TRUE(storage.load_from_binary(data, &three_objects));

  const epee::serialization::portable_storage::limits_t two_objects = {2, 16, 16};
  EXPECT_FALSE(storage.load_from_binary(data, &two_objects));

  // the root section has one field
  const epee::serialization::portable_storage::limits_t no_fields = {16, 0, 16};
  EXPECT_FALSE(storage.load_from_binary(data, &no_fields));
}
