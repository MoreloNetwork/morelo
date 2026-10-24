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
//
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#include "gtest/gtest.h"
#include <boost/filesystem.hpp>
#include <boost/program_options/parsers.hpp>
#include "cryptonote_core/cryptonote_core.h"
#include "p2p/net_node.h"
#include "p2p/net_node.inl"
#include "cryptonote_protocol/cryptonote_protocol_handler.h"
#include "cryptonote_protocol/cryptonote_protocol_handler.inl"

#define MAKE_IPV4_ADDRESS(a,b,c,d) epee::net_utils::ipv4_network_address{(uint32_t)MAKE_IP(a,b,c,d),0}
#define MAKE_IPV4_ADDRESS_PORT(a,b,c,d,e) epee::net_utils::ipv4_network_address{(uint32_t)MAKE_IP(a,b,c,d),e}
#define MAKE_IPV4_SUBNET(a,b,c,d,e) epee::net_utils::ipv4_network_subnet{(uint32_t)MAKE_IP(a,b,c,d),e}
#define MAKE_IPV6_ADDRESS(str,port) epee::net_utils::ipv6_network_address{boost::asio::ip::make_address_v6(str),port}

class test_core
{
public:
  void on_synchronized(){}
  void safesyncmode(const bool){}
  uint64_t get_current_blockchain_height() const {return 1;}
  void set_target_blockchain_height(uint64_t) {}
  bool init(const boost::program_options::variables_map& vm) {return true ;}
  bool deinit(){return true;}
  bool get_short_chain_history(std::list<crypto::hash>& ids) const { return true; }
  bool have_block(const crypto::hash& id) const {return true;}
  void get_blockchain_top(uint64_t& height, crypto::hash& top_id)const{height=0;top_id=crypto::null_hash;}
  bool handle_incoming_tx(const cryptonote::tx_blob_entry& tx_blob, cryptonote::tx_verification_context& tvc, bool keeped_by_block, bool relayed, bool do_not_relay) { return true; }
  bool handle_incoming_txs(const std::vector<cryptonote::tx_blob_entry>& tx_blob, std::vector<cryptonote::tx_verification_context>& tvc, bool keeped_by_block, bool relayed, bool do_not_relay) { return true; }
  bool handle_incoming_block(const cryptonote::blobdata& block_blob, const cryptonote::block *block, cryptonote::block_verification_context& bvc, bool update_miner_blocktemplate = true) { return true; }
  void pause_mine(){}
  void resume_mine(){}
  bool on_idle(){return true;}
  bool find_blockchain_supplement(const std::list<crypto::hash>& qblock_ids, bool clip_pruned, cryptonote::NOTIFY_RESPONSE_CHAIN_ENTRY::request& resp){return true;}
  bool handle_get_objects(cryptonote::NOTIFY_REQUEST_GET_OBJECTS::request& arg, cryptonote::NOTIFY_RESPONSE_GET_OBJECTS::request& rsp, cryptonote::cryptonote_connection_context& context){return true;}
  struct lockable { void lock() {} void unlock() {} } m_blockchain_storage;
  lockable &get_blockchain_storage() { return m_blockchain_storage; }
  bool get_test_drop_download() const {return true;}
  bool get_test_drop_download_height() const {return true;}
  bool prepare_handle_incoming_blocks(const std::vector<cryptonote::block_complete_entry>  &blocks_entry, std::vector<cryptonote::block> &blocks) { return true; }
  bool check_incoming_block_size(const cryptonote::blobdata& block_blob) const { return true; }
  bool cleanup_handle_incoming_blocks(bool force_sync = false) { return true; }
  uint64_t get_target_blockchain_height() const { return 1; }
  size_t get_block_sync_size(uint64_t height) const { return config::sync::NORMAL_SYNC; }
  virtual void on_transaction_relayed(const cryptonote::blobdata& tx) {}
  cryptonote::network_type get_nettype() const { return cryptonote::MAINNET; }
  bool get_pool_transaction(const crypto::hash& id, cryptonote::blobdata& tx_blob) const { return false; }
  bool pool_has_tx(const crypto::hash &txid) const { return false; }
  bool get_blocks(uint64_t start_offset, size_t count, std::vector<std::pair<cryptonote::blobdata, cryptonote::block>>& blocks, std::vector<cryptonote::blobdata>& txs) const { return false; }
  bool get_transactions(const std::vector<crypto::hash>& txs_ids, std::vector<cryptonote::transaction>& txs, std::vector<crypto::hash>& missed_txs) const { return false; }
  bool get_block_by_hash(const crypto::hash &h, cryptonote::block &blk, bool *orphan = NULL) const { return false; }
  crypto::hash get_block_id_by_height(uint64_t height) const { return crypto::null_hash; }
  uint8_t get_ideal_hard_fork_version() const { return 0; }
  uint8_t get_ideal_hard_fork_version(uint64_t height) const { return 0; }
  uint8_t get_hard_fork_version(uint64_t height) const { return 0; }
  uint64_t get_earliest_ideal_height_for_version(uint8_t version) const { return 0; }
  cryptonote::difficulty_type get_block_cumulative_difficulty(uint64_t height) const { return 0; }
  bool fluffy_blocks_enabled() const { return false; }
  uint64_t prevalidate_block_hashes(uint64_t height, const std::vector<crypto::hash> &hashes, const std::vector<uint64_t> &weights) { return 0; }
  bool pad_transactions() { return false; }
  uint32_t get_blockchain_pruning_seed() const { return 0; }
  bool prune_blockchain(uint32_t pruning_seed = 0) { return true; }
  bool is_within_compiled_block_hash_area(uint64_t height) const { return false; }
  bool has_block_weights(uint64_t height, uint64_t nblocks) const { return false; }
  void stop() {}
};

typedef nodetool::node_server<cryptonote::t_cryptonote_protocol_handler<test_core>> Server;

static bool is_blocked(Server &server, const epee::net_utils::network_address &address, time_t *t = NULL)
{
  const std::string host = address.host_str();
  std::map<std::string, time_t> hosts = server.get_blocked_hosts();
  for (auto rec: hosts)
  {
    if (rec.first == host)
    {
      if (t)
        *t = rec.second;
      return true;
    }
  }
  return false;
}

TEST(ban, add)
{
  test_core pr_core;
  cryptonote::t_cryptonote_protocol_handler<test_core> cprotocol(pr_core, NULL);
  Server server(cprotocol);
  cprotocol.set_p2p_endpoint(&server);

  // starts empty
  ASSERT_TRUE(server.get_blocked_hosts().empty());
  ASSERT_FALSE(is_blocked(server,MAKE_IPV4_ADDRESS(1,2,3,4)));
  ASSERT_FALSE(is_blocked(server,MAKE_IPV4_ADDRESS(1,2,3,5)));

  // add an IP
  ASSERT_TRUE(server.block_host(MAKE_IPV4_ADDRESS(1,2,3,4)));
  ASSERT_TRUE(server.get_blocked_hosts().size() == 1);
  ASSERT_TRUE(is_blocked(server,MAKE_IPV4_ADDRESS(1,2,3,4)));
  ASSERT_FALSE(is_blocked(server,MAKE_IPV4_ADDRESS(1,2,3,5)));

  // add the same, should not change
  ASSERT_TRUE(server.block_host(MAKE_IPV4_ADDRESS(1,2,3,4)));
  ASSERT_TRUE(server.get_blocked_hosts().size() == 1);
  ASSERT_TRUE(is_blocked(server,MAKE_IPV4_ADDRESS(1,2,3,4)));
  ASSERT_FALSE(is_blocked(server,MAKE_IPV4_ADDRESS(1,2,3,5)));

  // remove an unblocked IP, should not change
  ASSERT_FALSE(server.unblock_host(MAKE_IPV4_ADDRESS(1,2,3,5)));
  ASSERT_TRUE(server.get_blocked_hosts().size() == 1);
  ASSERT_TRUE(is_blocked(server,MAKE_IPV4_ADDRESS(1,2,3,4)));
  ASSERT_FALSE(is_blocked(server,MAKE_IPV4_ADDRESS(1,2,3,5)));

  // remove the IP, ends up empty
  ASSERT_TRUE(server.unblock_host(MAKE_IPV4_ADDRESS(1,2,3,4)));
  ASSERT_TRUE(server.get_blocked_hosts().size() == 0);
  ASSERT_FALSE(is_blocked(server,MAKE_IPV4_ADDRESS(1,2,3,4)));
  ASSERT_FALSE(is_blocked(server,MAKE_IPV4_ADDRESS(1,2,3,5)));

  // remove the IP from an empty list, still empty
  ASSERT_FALSE(server.unblock_host(MAKE_IPV4_ADDRESS(1,2,3,4)));
  ASSERT_TRUE(server.get_blocked_hosts().size() == 0);
  ASSERT_FALSE(is_blocked(server,MAKE_IPV4_ADDRESS(1,2,3,4)));
  ASSERT_FALSE(is_blocked(server,MAKE_IPV4_ADDRESS(1,2,3,5)));

  // add two for known amounts of time, they're both blocked
  ASSERT_TRUE(server.block_host(MAKE_IPV4_ADDRESS(1,2,3,4), 1));
  ASSERT_TRUE(server.block_host(MAKE_IPV4_ADDRESS(1,2,3,5), 3));
  ASSERT_TRUE(server.get_blocked_hosts().size() == 2);
  ASSERT_TRUE(is_blocked(server,MAKE_IPV4_ADDRESS(1,2,3,4)));
  ASSERT_TRUE(is_blocked(server,MAKE_IPV4_ADDRESS(1,2,3,5)));
  ASSERT_TRUE(server.unblock_host(MAKE_IPV4_ADDRESS(1,2,3,4)));
  ASSERT_TRUE(server.unblock_host(MAKE_IPV4_ADDRESS(1,2,3,5)));

  // these tests would need to call is_remote_ip_allowed, which is private
#if 0
  // after two seconds, the first IP is unblocked, but not the second yet
  sleep(2);
  ASSERT_TRUE(server.get_blocked_hosts().size() == 1);
  ASSERT_FALSE(is_blocked(server,MAKE_IPV4_ADDRESS(1,2,3,4)));
  ASSERT_TRUE(is_blocked(server,MAKE_IPV4_ADDRESS(1,2,3,5)));

  // after two more seconds, the second IP is also unblocked
  sleep(2);
  ASSERT_TRUE(server.get_blocked_hosts().size() == 0);
  ASSERT_FALSE(is_blocked(server,MAKE_IPV4_ADDRESS(1,2,3,4)));
  ASSERT_FALSE(is_blocked(server,MAKE_IPV4_ADDRESS(1,2,3,5)));
#endif

  // add an IP again, then re-ban for longer, then shorter
  time_t t;
  ASSERT_TRUE(server.block_host(MAKE_IPV4_ADDRESS(1,2,3,4), 2));
  ASSERT_TRUE(server.get_blocked_hosts().size() == 1);
  ASSERT_TRUE(is_blocked(server,MAKE_IPV4_ADDRESS(1,2,3,4), &t));
  ASSERT_FALSE(is_blocked(server,MAKE_IPV4_ADDRESS(1,2,3,5)));
  ASSERT_TRUE(t >= 1);
  ASSERT_TRUE(server.block_host(MAKE_IPV4_ADDRESS(1,2,3,4), 9));
  ASSERT_TRUE(server.get_blocked_hosts().size() == 1);
  ASSERT_TRUE(is_blocked(server,MAKE_IPV4_ADDRESS(1,2,3,4), &t));
  ASSERT_FALSE(is_blocked(server,MAKE_IPV4_ADDRESS(1,2,3,5)));
  ASSERT_TRUE(t >= 8);
  ASSERT_TRUE(server.block_host(MAKE_IPV4_ADDRESS(1,2,3,4), 5));
  ASSERT_TRUE(server.get_blocked_hosts().size() == 1);
  ASSERT_TRUE(is_blocked(server,MAKE_IPV4_ADDRESS(1,2,3,4), &t));
  ASSERT_FALSE(is_blocked(server,MAKE_IPV4_ADDRESS(1,2,3,5)));
  ASSERT_TRUE(t >= 4);
}

TEST(ban, subnet)
{
  time_t seconds;
  test_core pr_core;
  cryptonote::t_cryptonote_protocol_handler<test_core> cprotocol(pr_core, NULL);
  Server server(cprotocol);
  cprotocol.set_p2p_endpoint(&server);

  ASSERT_TRUE(server.block_subnet(MAKE_IPV4_SUBNET(1,2,3,4,24), 10));
  ASSERT_TRUE(server.get_blocked_subnets().size() == 1);
  ASSERT_TRUE(server.is_host_blocked(MAKE_IPV4_ADDRESS(1,2,3,4), &seconds));
  ASSERT_TRUE(seconds >= 9);
  ASSERT_TRUE(server.is_host_blocked(MAKE_IPV4_ADDRESS(1,2,3,255), &seconds));
  ASSERT_TRUE(server.is_host_blocked(MAKE_IPV4_ADDRESS(1,2,3,0), &seconds));
  ASSERT_FALSE(server.is_host_blocked(MAKE_IPV4_ADDRESS(1,2,4,0), &seconds));
  ASSERT_FALSE(server.is_host_blocked(MAKE_IPV4_ADDRESS(1,2,2,0), &seconds));
  ASSERT_TRUE(server.unblock_subnet(MAKE_IPV4_SUBNET(1,2,3,8,24)));
  ASSERT_TRUE(server.get_blocked_subnets().size() == 0);
  ASSERT_FALSE(server.is_host_blocked(MAKE_IPV4_ADDRESS(1,2,3,255), &seconds));
  ASSERT_FALSE(server.is_host_blocked(MAKE_IPV4_ADDRESS(1,2,3,0), &seconds));
  ASSERT_TRUE(server.block_subnet(MAKE_IPV4_SUBNET(1,2,3,4,8), 10));
  ASSERT_TRUE(server.get_blocked_subnets().size() == 1);
  ASSERT_TRUE(server.is_host_blocked(MAKE_IPV4_ADDRESS(1,255,3,255), &seconds));
  ASSERT_TRUE(server.is_host_blocked(MAKE_IPV4_ADDRESS(1,0,3,255), &seconds));
  ASSERT_FALSE(server.is_host_blocked(MAKE_IPV4_ADDRESS(2,0,3,255), &seconds));
  ASSERT_TRUE(server.block_host(MAKE_IPV4_ADDRESS(10,1,2,3), 10));
  ASSERT_TRUE(server.clear_bans());
  ASSERT_TRUE(server.get_blocked_subnets().empty());
  ASSERT_TRUE(server.get_blocked_hosts().empty());
  ASSERT_FALSE(server.is_host_blocked(MAKE_IPV4_ADDRESS(1,0,3,255), &seconds));
  ASSERT_FALSE(server.clear_bans());
}

TEST(ban, ignores_port)
{
  test_core pr_core;
  cryptonote::t_cryptonote_protocol_handler<test_core> cprotocol(pr_core, NULL);
  Server server(cprotocol);
  cprotocol.set_p2p_endpoint(&server);

  ASSERT_FALSE(is_blocked(server,MAKE_IPV4_ADDRESS_PORT(1,2,3,4,5)));
  ASSERT_TRUE(server.block_host(MAKE_IPV4_ADDRESS_PORT(1,2,3,4,5), std::numeric_limits<time_t>::max() - 1));
  ASSERT_TRUE(is_blocked(server,MAKE_IPV4_ADDRESS_PORT(1,2,3,4,5)));
  ASSERT_TRUE(is_blocked(server,MAKE_IPV4_ADDRESS_PORT(1,2,3,4,6)));
  ASSERT_TRUE(server.unblock_host(MAKE_IPV4_ADDRESS_PORT(1,2,3,4,5)));
  ASSERT_FALSE(is_blocked(server,MAKE_IPV4_ADDRESS_PORT(1,2,3,4,5)));
  ASSERT_FALSE(is_blocked(server,MAKE_IPV4_ADDRESS_PORT(1,2,3,4,6)));
}

TEST(ban, subnet_parse)
{
  ASSERT_TRUE(net::get_ipv4_subnet_address("1.2.3.4/24"));
  ASSERT_TRUE(net::get_ipv4_subnet_address("1.2.3.4/32"));
  ASSERT_TRUE(net::get_ipv4_subnet_address("1.2.3.4/0"));
  ASSERT_FALSE(net::get_ipv4_subnet_address("1.2.3.4/33"));
  ASSERT_FALSE(net::get_ipv4_subnet_address("1.2.3.4"));
  ASSERT_TRUE(net::get_ipv4_subnet_address("1.2.3.4", true));
  ASSERT_FALSE(net::get_ipv4_subnet_address("1.2.3.4/x"));
  ASSERT_TRUE(net::get_ipv4_subnet_address("1.2.3.4/24")->matches(MAKE_IPV4_ADDRESS_PORT(1,2,3,200,1)));
  ASSERT_FALSE(net::get_ipv4_subnet_address("1.2.3.4/24")->matches(MAKE_IPV4_ADDRESS_PORT(1,2,4,4,1)));
  ASSERT_EQ(net::get_ipv4_subnet_address("1.2.3.4/24")->host_str(), "1.2.3.0/24");
}

TEST(ban, ipv6_host_bans_its_prefix)
{
  time_t seconds;
  test_core pr_core;
  cryptonote::t_cryptonote_protocol_handler<test_core> cprotocol(pr_core, NULL);
  Server server(cprotocol);
  cprotocol.set_p2p_endpoint(&server);

  // a public IPv6 host owns a /64 as a rule, banning one address bans the prefix
  ASSERT_TRUE(server.block_host(MAKE_IPV6_ADDRESS("2001:db8:1:2::1", 38303), 10));
  ASSERT_TRUE(server.get_blocked_hosts().empty());
  ASSERT_EQ(1, server.get_blocked_subnets6().size());
  ASSERT_EQ("2001:db8:1:2::/64", server.get_blocked_subnets6().begin()->first.host_str());
  ASSERT_TRUE(server.is_host_blocked(MAKE_IPV6_ADDRESS("2001:db8:1:2::1", 0), &seconds));
  ASSERT_TRUE(seconds >= 9);
  ASSERT_TRUE(server.is_host_blocked(MAKE_IPV6_ADDRESS("2001:db8:1:2:ffff:ffff:ffff:ffff", 1), &seconds));
  ASSERT_FALSE(server.is_host_blocked(MAKE_IPV6_ADDRESS("2001:db8:1:3::1", 0), &seconds));
  ASSERT_FALSE(server.is_host_blocked(MAKE_IPV4_ADDRESS(1,2,3,4), &seconds));
  // add_only never shortens a ban, a plain ban sets it
  ASSERT_TRUE(server.block_host(MAKE_IPV6_ADDRESS("2001:db8:1:2::7", 0), 100));
  ASSERT_TRUE(server.block_host(MAKE_IPV6_ADDRESS("2001:db8:1:2::8", 0), 5, true));
  ASSERT_TRUE(server.is_host_blocked(MAKE_IPV6_ADDRESS("2001:db8:1:2::1", 0), &seconds));
  ASSERT_TRUE(seconds >= 99);
  ASSERT_TRUE(server.block_host(MAKE_IPV6_ADDRESS("2001:db8:1:2::8", 0), 10));
  ASSERT_TRUE(server.is_host_blocked(MAKE_IPV6_ADDRESS("2001:db8:1:2::1", 0), &seconds));
  ASSERT_TRUE(seconds <= 10);
  ASSERT_EQ(1, server.get_blocked_subnets6().size());
  // unbanning any address of the prefix lifts it
  ASSERT_TRUE(server.unblock_host(MAKE_IPV6_ADDRESS("2001:db8:1:2::5", 0)));
  ASSERT_TRUE(server.get_blocked_subnets6().empty());
  ASSERT_FALSE(server.is_host_blocked(MAKE_IPV6_ADDRESS("2001:db8:1:2::1", 0), &seconds));
  ASSERT_FALSE(server.unblock_host(MAKE_IPV6_ADDRESS("2001:db8:1:2::5", 0)));

  // loopback and local addresses are banned individually
  ASSERT_TRUE(server.block_host(MAKE_IPV6_ADDRESS("::1", 0), 10));
  ASSERT_TRUE(server.block_host(MAKE_IPV6_ADDRESS("fd00::1", 0), 10));
  ASSERT_EQ(2, server.get_blocked_hosts().size());
  ASSERT_TRUE(server.get_blocked_subnets6().empty());
  ASSERT_TRUE(server.is_host_blocked(MAKE_IPV6_ADDRESS("::1", 5), &seconds));
  ASSERT_FALSE(server.is_host_blocked(MAKE_IPV6_ADDRESS("fd00::2", 5), &seconds));
  ASSERT_TRUE(server.unblock_host(MAKE_IPV6_ADDRESS("::1", 0)));
  ASSERT_TRUE(server.clear_bans());
  ASSERT_TRUE(server.get_blocked_hosts().empty());
}

TEST(ban, ipv6_subnet)
{
  time_t seconds;
  test_core pr_core;
  cryptonote::t_cryptonote_protocol_handler<test_core> cprotocol(pr_core, NULL);
  Server server(cprotocol);
  cprotocol.set_p2p_endpoint(&server);

  ASSERT_TRUE(server.block_subnet(*net::get_ipv6_subnet_address("2001:db8::/32"), 10));
  ASSERT_EQ(1, server.get_blocked_subnets6().size());
  ASSERT_TRUE(server.is_host_blocked(MAKE_IPV6_ADDRESS("2001:db8:ffff::1", 0), &seconds));
  ASSERT_TRUE(seconds >= 9);
  ASSERT_FALSE(server.is_host_blocked(MAKE_IPV6_ADDRESS("2001:db9::1", 0), &seconds));
  ASSERT_FALSE(server.is_host_blocked(MAKE_IPV4_ADDRESS(32,1,13,184), &seconds));
  ASSERT_TRUE(server.unblock_subnet(*net::get_ipv6_subnet_address("2001:db8::/32")));
  ASSERT_FALSE(server.unblock_subnet(*net::get_ipv6_subnet_address("2001:db8::/32")));
  ASSERT_TRUE(server.get_blocked_subnets6().empty());
  ASSERT_FALSE(server.is_host_blocked(MAKE_IPV6_ADDRESS("2001:db8:ffff::1", 0), &seconds));
  ASSERT_TRUE(server.block_subnet(*net::get_ipv6_subnet_address("2001:db8::/32"), 10));
  ASSERT_TRUE(server.clear_bans());
  ASSERT_TRUE(server.get_blocked_subnets6().empty());
}

TEST(ban, ipv6_fail_score_by_prefix)
{
  time_t seconds;
  test_core pr_core;
  cryptonote::t_cryptonote_protocol_handler<test_core> cprotocol(pr_core, NULL);
  Server server(cprotocol);
  cprotocol.set_p2p_endpoint(&server);

  // fails of the addresses of one /64 add up and the block covers the whole prefix
  nodetool::i_p2p_endpoint<cryptonote::t_cryptonote_protocol_handler<test_core>::connection_context>& endpoint = server;
  for (unsigned i = 0; i <= P2P_IP_FAILS_BEFORE_BLOCK; ++i)
    ASSERT_TRUE(endpoint.add_host_fail(MAKE_IPV6_ADDRESS(i % 2 ? "2001:db8:1:2::1" : "2001:db8:1:2::2", 0)));
  ASSERT_TRUE(server.is_host_blocked(MAKE_IPV6_ADDRESS("2001:db8:1:2::3", 0), &seconds));
  ASSERT_FALSE(server.is_host_blocked(MAKE_IPV6_ADDRESS("2001:db8:1:3::3", 0), &seconds));
}

namespace
{
  nodetool::peerlist_entry make_peer(const epee::net_utils::network_address& adr, uint64_t id)
  {
    nodetool::peerlist_entry pe{};
    pe.adr = adr;
    pe.id = id;
    pe.last_seen = 1000;
    pe.rpc_port = 38302;
    return pe;
  }

  bool contains(const std::vector<nodetool::peerlist_entry>& peers, const epee::net_utils::network_address& adr)
  {
    return std::any_of(peers.begin(), peers.end(), [&adr](const nodetool::peerlist_entry& pe) { return pe.adr == adr; });
  }

  const epee::net_utils::network_address mapped_v6{MAKE_IPV6_ADDRESS("::ffff:45.155.69.10", 38303)};
  const epee::net_utils::network_address unspecified_v6{MAKE_IPV6_ADDRESS("::", 38303)};
  const epee::net_utils::network_address multicast_v6{MAKE_IPV6_ADDRESS("ff02::1", 38303)};
  const epee::net_utils::network_address public_v6{MAKE_IPV6_ADDRESS("2001:db8::1", 38303)};
  const epee::net_utils::network_address public_v4{MAKE_IPV4_ADDRESS_PORT(11,22,33,44, 38303)};
}

TEST(node_server, forbidden_ipv6)
{
  EXPECT_TRUE(nodetool::is_forbidden_ipv6_address(mapped_v6));
  EXPECT_TRUE(nodetool::is_forbidden_ipv6_address(unspecified_v6));
  EXPECT_TRUE(nodetool::is_forbidden_ipv6_address(multicast_v6));
  EXPECT_FALSE(nodetool::is_forbidden_ipv6_address(public_v6));
  EXPECT_FALSE(nodetool::is_forbidden_ipv6_address(public_v4));

  EXPECT_TRUE(nodetool::should_skip_connect_address(public_v6, false));
  EXPECT_FALSE(nodetool::should_skip_connect_address(public_v6, true));
  EXPECT_TRUE(nodetool::should_skip_connect_address(mapped_v6, true));
  EXPECT_TRUE(nodetool::should_skip_connect_address(unspecified_v6, true));
  EXPECT_FALSE(nodetool::should_skip_connect_address(public_v4, false));
}

TEST(node_server, peerlist_filter_by_support_flags)
{
  // a peer without P2P_SUPPORT_FLAG_IPV6 (any deployed node) must never see an IPv6 entry
  for (const uint32_t flags : {0u, (uint32_t)P2P_SUPPORT_FLAG_FLUFFY_BLOCKS})
  {
    EXPECT_TRUE(nodetool::can_send_peer(make_peer(public_v4, 1), flags));
    EXPECT_FALSE(nodetool::can_send_peer(make_peer(public_v6, 2), flags));
    EXPECT_FALSE(nodetool::can_send_peer(make_peer(mapped_v6, 3), flags));
  }

  // an IPv6 capable peer gets them, forbidden addresses are never sent
  const uint32_t flags = P2P_SUPPORT_FLAG_FLUFFY_BLOCKS | P2P_SUPPORT_FLAG_IPV6;
  EXPECT_TRUE(nodetool::can_send_peer(make_peer(public_v4, 1), flags));
  EXPECT_TRUE(nodetool::can_send_peer(make_peer(public_v6, 2), flags));
  EXPECT_FALSE(nodetool::can_send_peer(make_peer(mapped_v6, 3), flags));
  EXPECT_FALSE(nodetool::can_send_peer(make_peer(unspecified_v6, 4), flags));
  EXPECT_FALSE(nodetool::can_send_peer(make_peer(multicast_v6, 5), flags));
}

TEST(node_server, sanitize_peerlist)
{
  test_core pr_core;
  cryptonote::t_cryptonote_protocol_handler<test_core> cprotocol(pr_core, NULL);
  Server server(cprotocol); // IPv6 is off by default
  cprotocol.set_p2p_endpoint(&server);

  std::vector<nodetool::peerlist_entry> list;
  list.push_back(make_peer(epee::net_utils::network_address{}, 1)); // unknown type, as an unparseable entry ends up
  list.push_back(make_peer(public_v6, 2));
  list.push_back(make_peer(mapped_v6, 3));
  list.push_back(make_peer(unspecified_v6, 4));
  list.push_back(make_peer(MAKE_IPV6_ADDRESS("fe80::1", 38303), 5));
  list.push_back(make_peer(MAKE_IPV6_ADDRESS("::1", 38303), 6));
  list.push_back(make_peer(MAKE_IPV4_ADDRESS_PORT(127,0,0,1, 38303), 7));
  list.push_back(make_peer(MAKE_IPV4_ADDRESS_PORT(10,0,0,1, 38303), 8));
  list.push_back(make_peer(MAKE_IPV4_ADDRESS_PORT(0,0,0,0, 38303), 9));
  list.push_back(make_peer(MAKE_IPV4_ADDRESS_PORT(11,22,33,44, 38302), 10)); // p2p port equal to its rpc port
  list.push_back(make_peer(public_v4, 11));

  ASSERT_TRUE(server.sanitize_peerlist(list));
  ASSERT_EQ(1, list.size());
  EXPECT_EQ(public_v4, list[0].adr);
  EXPECT_EQ(11, list[0].id);
  EXPECT_EQ(0, list[0].last_seen); // a remote last seen is never trusted
}

namespace
{
  // brings `server` up from command line options, offline so that nothing is bound
  bool init_offline(Server& server, const boost::filesystem::path& data_dir, std::vector<std::string> args)
  {
    boost::program_options::options_description desc;
    cryptonote::core::init_options(desc);
    Server::init_options(desc);
    args.push_back("--offline");
    args.push_back("--data-dir=" + data_dir.string());
    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::command_line_parser(args).options(desc).run(), vm);
    boost::program_options::notify(vm);
    return server.init(vm);
  }
}

TEST(node_server, ipv6_peers_need_ipv6)
{
  const boost::filesystem::path dir = boost::filesystem::temp_directory_path() / boost::filesystem::unique_path();
  ASSERT_TRUE(boost::filesystem::create_directories(dir));
  nodetool::peerlist_types stored{};
  stored.white.push_back(make_peer(public_v4, 1));
  stored.white.push_back(make_peer(public_v6, 2));
  stored.gray.push_back(make_peer(MAKE_IPV6_ADDRESS("2001:db8::2", 38303), 3));
  ASSERT_TRUE(nodetool::peerlist_storage{}.store((dir / P2P_NET_DATA_FILENAME).string(), stored));

  for (const bool use_ipv6 : {false, true})
  {
    test_core pr_core;
    cryptonote::t_cryptonote_protocol_handler<test_core> cprotocol(pr_core, NULL);
    Server server(cprotocol);
    cprotocol.set_p2p_endpoint(&server);
    std::vector<std::string> args{"--add-peer=[2001:db8::5]:38303"};
    if (use_ipv6)
      args.push_back("--p2p-use-ipv6");
    ASSERT_TRUE(init_offline(server, dir, args));

    // with IPv6 off, stored and command line IPv6 peers are dropped, they would only keep
    // p2pstate.bin unreadable for older versions
    std::vector<nodetool::peerlist_entry> gray, white;
    server.get_public_peerlist(gray, white);
    EXPECT_TRUE(contains(white, public_v4));
    EXPECT_EQ(use_ipv6, contains(white, public_v6));
    EXPECT_EQ(use_ipv6, contains(white, MAKE_IPV6_ADDRESS("2001:db8::5", 38303)));
    EXPECT_EQ(use_ipv6 ? 1u : 0u, gray.size());

    // received IPv6 peers are only kept with IPv6 on, and never local or forbidden ones
    std::vector<nodetool::peerlist_entry> list;
    list.push_back(make_peer(public_v6, 4));
    list.push_back(make_peer(MAKE_IPV6_ADDRESS("fe80::1", 38303), 5));
    list.push_back(make_peer(MAKE_IPV6_ADDRESS("fd00::1", 38303), 6));
    list.push_back(make_peer(MAKE_IPV6_ADDRESS("::1", 38303), 7));
    list.push_back(make_peer(mapped_v6, 8));
    list.push_back(make_peer(unspecified_v6, 9));
    list.push_back(make_peer(multicast_v6, 10));
    list.push_back(make_peer(MAKE_IPV6_ADDRESS("2001:db8::3", 38302), 11)); // p2p port equal to its rpc port
    list.push_back(make_peer(public_v4, 12));
    ASSERT_TRUE(server.sanitize_peerlist(list));
    ASSERT_EQ(use_ipv6 ? 2u : 1u, list.size());
    EXPECT_TRUE(contains(list, public_v4));
    EXPECT_EQ(use_ipv6, contains(list, public_v6));
  }
  boost::filesystem::remove_all(dir);
}

namespace nodetool { template class node_server<cryptonote::t_cryptonote_protocol_handler<test_core>>; }
namespace cryptonote { template class t_cryptonote_protocol_handler<test_core>; }
