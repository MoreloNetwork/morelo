// Copyright (c) 2018-2019, The Arqma Network
// Copyright (c) 2014-2018, The Monero Project
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
//
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#pragma once

#include <algorithm>
#include <limits>
#include <type_traits>
#include <vector>
#include "serialization.h"

template <template <bool> class Archive, class T>
bool do_serialize(Archive<false> &ar, std::vector<T> &v);
template <template <bool> class Archive, class T>
bool do_serialize(Archive<true> &ar, std::vector<T> &v);

namespace serialization
{
  namespace detail
  {
    template <typename T>
    void do_reserve(std::vector<T> &c, size_t N, size_t B)
    {
      // N comes from the wire: never reserve more than the remaining bytes B
      // could decode to, assuming at most this many bytes of T per wire byte
      static constexpr size_t max_compression_ratio =
        is_blob_type<T>::type::value ? 1 :
        (std::is_same<T, uint32_t>::value || std::is_same<T, uint64_t>::value) ? sizeof(T) : // varint
        (std::is_same<T, char>::value || std::is_same<T, unsigned char>::value) ? 1 :
        4; // default

      B /= sizeof(T);
      B = std::max(size_t(1), B);
      if (std::numeric_limits<size_t>::max() / max_compression_ratio <= B)
        B = std::numeric_limits<size_t>::max();
      else
        B *= max_compression_ratio;

      c.reserve(std::min(N, B));
    }

    template <typename T>
    void do_add(std::vector<T> &c, T &&e)
    {
      c.emplace_back(std::move(e));
    }
  }
}

#include "container.h"

template <template <bool> class Archive, class T>
bool do_serialize(Archive<false> &ar, std::vector<T> &v) { return do_serialize_container(ar, v); }
template <template <bool> class Archive, class T>
bool do_serialize(Archive<true> &ar, std::vector<T> &v) { return do_serialize_container(ar, v); }
