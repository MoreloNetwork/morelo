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

#include <sstream>
#include <boost/asio/ip/address_v6.hpp>

#include "net/net_parse_helpers.h"
#include "net/net_utils_base.h"
#include "net/parse.h"
#include "p2p/net_peerlist.h"
#include "p2p/p2p_protocol_defs.h"
#include "storages/portable_storage.h"

namespace
{
  epee::net_utils::network_address v6(const char* ip, uint16_t port)
  {
    return epee::net_utils::ipv6_network_address{boost::asio::ip::make_address_v6(ip), port};
  }

  epee::net_utils::network_address v4(uint32_t ip, uint16_t port)
  {
    return epee::net_utils::ipv4_network_address{ip, port};
  }

  void na_host_and_port_test(std::string addr, std::string exp_host, std::string exp_port)
  {
    std::string host{"xxxxx"};
    std::string port{"xxxxx"};
    net::get_network_address_host_and_port(addr, host, port);
    EXPECT_EQ(exp_host, host);
    EXPECT_EQ(exp_port, port);
  }

  nodetool::peerlist_entry make_peer(const epee::net_utils::network_address& adr, uint64_t id, int64_t last_seen)
  {
    nodetool::peerlist_entry pe{};
    pe.adr = adr;
    pe.id = id;
    pe.last_seen = last_seen;
    pe.pruning_seed = 0;
    pe.rpc_port = 38302;
    pe.zmq_port = 0;
    pe.rpc_credits_per_hash = 0;
    return pe;
  }
}

TEST(get_network_address_host_and_port, ipv4)
{
  na_host_and_port_test("9.9.9.9", "9.9.9.9", "xxxxx");
  na_host_and_port_test("9.9.9.9:18081", "9.9.9.9", "18081");
}

TEST(get_network_address_host_and_port, ipv6)
{
  na_host_and_port_test("::ffff", "::ffff", "xxxxx");
  na_host_and_port_test("[::ffff]", "::ffff", "xxxxx");
  na_host_and_port_test("[::ffff]:00231", "::ffff", "00231");
  na_host_and_port_test("ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff", "ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff", "xxxxx");
  na_host_and_port_test("[7777:7777:7777:7777:7777:7777:7777:7777]", "7777:7777:7777:7777:7777:7777:7777:7777", "xxxxx");
  na_host_and_port_test("[7777:7777:7777:7777:7777:7777:7777:7777]:48080", "7777:7777:7777:7777:7777:7777:7777:7777", "48080");
}

TEST(get_network_address_host_and_port, hostname)
{
  na_host_and_port_test("localhost", "localhost", "xxxxx");
  na_host_and_port_test("bar:29080", "bar", "29080"); // dotless hostnames, monero#8633
  na_host_and_port_test("seeds.morelonetwork.pl:38301", "seeds.morelonetwork.pl", "38301");
}

TEST(get_network_address, ipv4)
{
  expect<epee::net_utils::network_address> address = net::get_network_address("23.0.0.254", 1000);
  ASSERT_TRUE(bool(address));
  EXPECT_EQ(epee::net_utils::address_type::ipv4, address->get_type_id());
  EXPECT_STREQ("23.0.0.254:1000", address->str().c_str());

  address = net::get_network_address("23.0.0.254:2000", 1000);
  ASSERT_TRUE(bool(address));
  EXPECT_STREQ("23.0.0.254:2000", address->str().c_str());
  EXPECT_STREQ("23.0.0.254", address->host_str().c_str());

  EXPECT_EQ(net::error::invalid_port, net::get_network_address("23.0.0.254:x", 1000));
  EXPECT_EQ(net::error::unsupported_address, net::get_network_address("host:1234", 1000));
  EXPECT_EQ(net::error::invalid_host, net::get_network_address(":1234", 1000));
}

TEST(get_network_address, ipv6)
{
  expect<epee::net_utils::network_address> address = net::get_network_address("[::1]:38303", 1000);
  ASSERT_TRUE(bool(address));
  EXPECT_EQ(epee::net_utils::address_type::ipv6, address->get_type_id());
  EXPECT_EQ(epee::net_utils::zone::public_, address->get_zone());
  EXPECT_STREQ("[::1]:38303", address->str().c_str());
  EXPECT_STREQ("::1", address->host_str().c_str());
  EXPECT_TRUE(address->is_loopback());
  EXPECT_EQ(38303, address->as<epee::net_utils::ipv6_network_address>().port());

  address = net::get_network_address("[2001:db8::1]", 1000);
  ASSERT_TRUE(bool(address));
  EXPECT_STREQ("[2001:db8::1]:1000", address->str().c_str());
  EXPECT_FALSE(address->is_loopback());
  EXPECT_FALSE(address->is_local());

  address = net::get_network_address("2001:db8::1", 1000);
  ASSERT_TRUE(bool(address));
  EXPECT_STREQ("[2001:db8::1]:1000", address->str().c_str());
  EXPECT_EQ(*address, v6("2001:db8::1", 1000));
  EXPECT_NE(*address, v6("2001:db8::1", 1001));
  EXPECT_NE(*address, v4(0, 1000));
  EXPECT_TRUE(address->is_same_host(v6("2001:db8::1", 2)));
  EXPECT_FALSE(address->is_same_host(v6("2001:db8::2", 1000)));

  // link-local, site-local and unique local addresses count as local like private IPv4
  EXPECT_TRUE(v6("fe80::1", 1).is_local());
  EXPECT_TRUE(v6("fec0::1", 1).is_local());
  EXPECT_TRUE(v6("fc00::1", 1).is_local());
  EXPECT_TRUE(v6("fd12:3456::1", 1).is_local());
  EXPECT_FALSE(v6("2001:db8::1", 1).is_local());
  EXPECT_FALSE(v6("::1", 1).is_local());

  EXPECT_EQ(net::error::invalid_port, net::get_network_address("[::1]:x", 1000));
  EXPECT_EQ(net::error::invalid_host, net::get_network_address("[]:1000", 1000));
}

TEST(get_network_address, ipv4_mapped_ipv6)
{
  expect<epee::net_utils::network_address> address = net::get_network_address("[::ffff:203.0.113.1]:18080", 0);
  ASSERT_TRUE(bool(address));
  EXPECT_EQ(epee::net_utils::address_type::ipv4, address->get_type_id());
  EXPECT_STREQ("203.0.113.1", address->host_str().c_str());
  EXPECT_STREQ("203.0.113.1:18080", address->str().c_str());

  boost::asio::ip::address_v6::bytes_type bytes = {};
  bytes[10] = 0xff;
  bytes[11] = 0xff;
  bytes[12] = 45;
  bytes[13] = 155;
  bytes[14] = 69;
  bytes[15] = 10;
  const epee::net_utils::network_address mapped{epee::net_utils::ipv6_network_address{boost::asio::ip::address_v6{bytes}, 18080}};
  const boost::optional<epee::net_utils::ipv4_network_address> ipv4 = epee::net_utils::get_ipv4_mapped_address(mapped);
  ASSERT_TRUE(bool(ipv4));
  EXPECT_EQ("45.155.69.10", ipv4->host_str());
  EXPECT_EQ(18080, ipv4->port());
  EXPECT_FALSE(epee::net_utils::get_ipv4_mapped_address(v6("::1", 18080)));
  EXPECT_FALSE(epee::net_utils::get_ipv4_mapped_address(v4(1, 18080)));
}

TEST(get_network_address, ipv6subnet)
{
  expect<epee::net_utils::ipv6_network_subnet> subnet = net::get_ipv6_subnet_address("2001:db8::1", true);
  ASSERT_TRUE(bool(subnet));
  EXPECT_STREQ("2001:db8::1/128", subnet->str().c_str());
  EXPECT_EQ(128, subnet->mask());

  EXPECT_EQ(net::error::invalid_mask, net::get_ipv6_subnet_address("2001:db8::1"));
  EXPECT_EQ(net::error::invalid_mask, net::get_ipv6_subnet_address("2001:db8::1/129"));
  EXPECT_EQ(net::error::invalid_mask, net::get_ipv6_subnet_address("2001:db8::1/x"));
  EXPECT_EQ(net::error::invalid_host, net::get_ipv6_subnet_address("1.2.3.4/24"));
  EXPECT_EQ(net::error::invalid_host, net::get_ipv6_subnet_address("host/24"));
  EXPECT_EQ(net::error::invalid_host, net::get_ipv6_subnet_address("::ffff:1.2.3.0/120"));

  subnet = net::get_ipv6_subnet_address("[2001:db8:1:2:3:4:5:6]/64");
  ASSERT_TRUE(bool(subnet));
  EXPECT_STREQ("2001:db8:1:2::/64", subnet->str().c_str());
  EXPECT_STREQ("2001:db8:1:2::/64", subnet->host_str().c_str());
  EXPECT_TRUE(subnet->matches(v6("2001:db8:1:2::1", 1).as<epee::net_utils::ipv6_network_address>()));
  EXPECT_TRUE(subnet->matches(v6("2001:db8:1:2:ffff:ffff:ffff:ffff", 1).as<epee::net_utils::ipv6_network_address>()));
  EXPECT_FALSE(subnet->matches(v6("2001:db8:1:3::1", 1).as<epee::net_utils::ipv6_network_address>()));
  EXPECT_EQ(*subnet, *net::get_ipv6_subnet_address("2001:db8:1:2::/64"));
  EXPECT_NE(*subnet, *net::get_ipv6_subnet_address("2001:db8:1:2::/65"));

  subnet = net::get_ipv6_subnet_address("2001:db8::/29");
  ASSERT_TRUE(bool(subnet));
  EXPECT_STREQ("2001:db8::/29", subnet->str().c_str());
  EXPECT_TRUE(subnet->matches(v6("2001:dbf:ffff::1", 1).as<epee::net_utils::ipv6_network_address>()));
  EXPECT_FALSE(subnet->matches(v6("2001:dc0::1", 1).as<epee::net_utils::ipv6_network_address>()));
  EXPECT_FALSE(subnet->is_local());
  EXPECT_TRUE(net::get_ipv6_subnet_address("::1/128")->is_loopback());
  EXPECT_TRUE(net::get_ipv6_subnet_address("fd00::/8")->is_local());
}

TEST(net_utils, host_limit_key)
{
  // public IPv6 hosts are grouped by their /64, everything else by host
  EXPECT_EQ("2001:db8:1:2::/64", epee::net_utils::get_host_limit_key(v6("2001:db8:1:2:3:4:5:6", 38303)));
  EXPECT_EQ("2001:db8:1:2::/64", epee::net_utils::get_host_limit_key(v6("2001:db8:1:2::1", 1)));
  EXPECT_EQ("::1", epee::net_utils::get_host_limit_key(v6("::1", 38303)));
  EXPECT_EQ("fe80::1", epee::net_utils::get_host_limit_key(v6("fe80::1", 38303)));
  EXPECT_EQ("fd00::1", epee::net_utils::get_host_limit_key(v6("fd00::1", 38303)));
  EXPECT_EQ("1.2.3.4", epee::net_utils::get_host_limit_key(v4(MAKE_IP(1,2,3,4), 38303)));

  EXPECT_TRUE(epee::net_utils::should_group_ipv6_by_prefix(boost::asio::ip::make_address_v6("2001:db8::1")));
  EXPECT_FALSE(epee::net_utils::should_group_ipv6_by_prefix(boost::asio::ip::make_address_v6("::")));
  EXPECT_FALSE(epee::net_utils::should_group_ipv6_by_prefix(boost::asio::ip::make_address_v6("::1")));
  EXPECT_FALSE(epee::net_utils::should_group_ipv6_by_prefix(boost::asio::ip::make_address_v6("ff02::1")));
  EXPECT_FALSE(epee::net_utils::should_group_ipv6_by_prefix(boost::asio::ip::make_address_v6("::ffff:1.2.3.4")));
  EXPECT_EQ(boost::asio::ip::make_address_v6("2001:db8::"), epee::net_utils::get_ipv6_subnet_address(boost::asio::ip::make_address_v6("2001:db8:1:2:3:4:5:6"), 32));
  EXPECT_EQ(boost::asio::ip::make_address_v6("2001:db8:1:2:3:4:5:6"), epee::net_utils::get_ipv6_subnet_address(boost::asio::ip::make_address_v6("2001:db8:1:2:3:4:5:6"), 128));
  EXPECT_EQ(boost::asio::ip::make_address_v6("::"), epee::net_utils::get_ipv6_subnet_address(boost::asio::ip::make_address_v6("2001:db8:1:2:3:4:5:6"), 0));
}

TEST(parse_url, ipv6)
{
  epee::net_utils::http::url_content parsed{};
  ASSERT_TRUE(epee::net_utils::parse_url("http://[::1]:38302/json_rpc", parsed));
  EXPECT_EQ("http", parsed.schema);
  EXPECT_EQ("::1", parsed.host);
  EXPECT_EQ(38302, parsed.port);
  EXPECT_EQ("/json_rpc", parsed.uri);

  parsed = {};
  ASSERT_TRUE(epee::net_utils::parse_url("[2001:db8::1]:38302", parsed));
  EXPECT_EQ("2001:db8::1", parsed.host);
  EXPECT_EQ(38302, parsed.port);

  parsed = {};
  ASSERT_TRUE(epee::net_utils::parse_url("[2001:db8::1]", parsed));
  EXPECT_EQ("2001:db8::1", parsed.host);
  EXPECT_EQ(0, parsed.port);

  // IPv4 and hostnames keep working as before
  parsed = {};
  ASSERT_TRUE(epee::net_utils::parse_url("http://1.2.3.4:80/x", parsed));
  EXPECT_EQ("1.2.3.4", parsed.host);
  EXPECT_EQ(80, parsed.port);
  EXPECT_EQ("/x", parsed.uri);

  parsed = {};
  ASSERT_TRUE(epee::net_utils::parse_url("localhost:38302", parsed));
  EXPECT_EQ("localhost", parsed.host);
  EXPECT_EQ(38302, parsed.port);
}

TEST(peerlist_entry, kv_round_trip_ipv6)
{
  const nodetool::peerlist_entry in = make_peer(v6("2001:db8::1", 38303), 0x1234567890abcdefull, 7);

  epee::serialization::portable_storage stg{};
  ASSERT_TRUE(in.store(stg));
  std::string blob;
  ASSERT_TRUE(stg.store_to_binary(blob));

  epee::serialization::portable_storage stg2{};
  ASSERT_TRUE(stg2.load_from_binary(blob));
  nodetool::peerlist_entry out{};
  ASSERT_TRUE(out.load(stg2));
  EXPECT_EQ(epee::net_utils::address_type::ipv6, out.adr.get_type_id());
  EXPECT_EQ(in.adr, out.adr);
  EXPECT_EQ(in.id, out.id);
  EXPECT_EQ(in.last_seen, out.last_seen);
  EXPECT_EQ(in.rpc_port, out.rpc_port);
}

TEST(peerlist_storage, store_and_open_mixed)
{
  nodetool::peerlist_types types{};
  types.white.push_back(make_peer(v4(MAKE_IP(1,2,3,4), 38303), 1, 10));
  types.white.push_back(make_peer(v6("2001:db8::1", 38303), 2, 20));
  types.gray.push_back(make_peer(v6("2001:db8::2", 38303), 3, 30));
  nodetool::anchor_peerlist_entry anchor{};
  anchor.adr = v6("2001:db8::3", 38303);
  anchor.id = 4;
  anchor.first_seen = 40;
  types.anchor.push_back(anchor);

  std::stringstream stream;
  const nodetool::peerlist_storage empty{};
  ASSERT_TRUE(empty.store(stream, types));

  boost::optional<nodetool::peerlist_storage> loaded = nodetool::peerlist_storage::open(stream, true);
  ASSERT_TRUE(bool(loaded));
  nodetool::peerlist_types out = loaded->take_zone(epee::net_utils::zone::public_);
  ASSERT_EQ(2, out.white.size());
  ASSERT_EQ(1, out.gray.size());
  ASSERT_EQ(1, out.anchor.size());
  EXPECT_TRUE((out.white[0].adr == types.white[0].adr && out.white[1].adr == types.white[1].adr) ||
              (out.white[0].adr == types.white[1].adr && out.white[1].adr == types.white[0].adr));
  EXPECT_EQ(types.gray[0].adr, out.gray[0].adr);
  EXPECT_EQ(3, out.gray[0].id);
  EXPECT_EQ(types.anchor[0].adr, out.anchor[0].adr);
  EXPECT_EQ(40, out.anchor[0].first_seen);
}

TEST(peerlist_entry, kv_malformed_ipv6)
{
  // an IPv6 entry with a short address blob loads as an invalid address, which
  // sanitize_peerlist drops, instead of a made up one
  epee::serialization::portable_storage stg{};
  ASSERT_TRUE(make_peer(v6("2001:db8::1", 38303), 1, 7).store(stg));
  const auto adr = stg.open_section("adr", nullptr);
  ASSERT_NE(nullptr, adr);
  const auto addr = stg.open_section("addr", adr);
  ASSERT_NE(nullptr, addr);
  ASSERT_TRUE(stg.set_value("addr", std::string(15, 'a'), addr));
  std::string blob;
  ASSERT_TRUE(stg.store_to_binary(blob));

  epee::serialization::portable_storage stg2{};
  ASSERT_TRUE(stg2.load_from_binary(blob));
  nodetool::peerlist_entry out{};
  out.load(stg2);
  EXPECT_EQ(epee::net_utils::address_type::invalid, out.adr.get_type_id());
}

TEST(peerlist_manager, head_filter_before_depth)
{
  // the newest entries are IPv6; a peer that may only get IPv4 ones still gets a full list
  nodetool::peerlist_types types{};
  for (uint16_t i = 0; i < 20; ++i)
    types.white.push_back(make_peer(v6("2001:db8::1", 1000 + i), i, 2000 + i));
  for (uint16_t i = 0; i < 10; ++i)
    types.white.push_back(make_peer(v4(MAKE_IP(1,2,3,4), 1000 + i), 100 + i, 100 + i));
  nodetool::peerlist_manager plm;
  ASSERT_TRUE(plm.init(std::move(types), false));

  const auto ipv4_only = [](const nodetool::peerlist_entry& pe) { return pe.adr.get_type_id() == epee::net_utils::address_type::ipv4; };
  for (const bool anonymize : {false, true})
  {
    std::vector<nodetool::peerlist_entry> head;
    ASSERT_TRUE(plm.get_peerlist_head(head, anonymize, 8, ipv4_only));
    ASSERT_EQ(8, head.size());
    for (const auto& pe : head)
      EXPECT_EQ(epee::net_utils::address_type::ipv4, pe.adr.get_type_id());
  }
}
