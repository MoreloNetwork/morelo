// Copyright (c) 2019-2024, The Morelo Network
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

#include <stdexcept>
#include <string>
#include <boost/uuid/uuid.hpp>

#define CRYPTONOTE_DNS_TIMEOUT_MS												20000

#define CRYPTONOTE_MAX_BLOCK_NUMBER											500000000
#define CRYPTONOTE_GETBLOCKTEMPLATE_MAX_BLOCK_SIZE			196608		// size of block (bytes) that is the maximum that miners will produce
#define CRYPTONOTE_PUBLIC_ADDRESS_TEXTBLOB_VER					0
#define CRYPTONOTE_BLOCK_FUTURE_TIME_LIMIT_V2						300 * 2
#define CRYPTONOTE_BLOCK_FUTURE_TIME_LIMIT_V3						100 * 3
#define CRYPTONOTE_BLOCK_FUTURE_TIME_LIMIT_V4						CRYPTONOTE_BLOCK_FUTURE_TIME_LIMIT_V3
#define CRYPTONOTE_BLOCK_FUTURE_TIME_LIMIT							60 * 60 * 2

#define BLOCKCHAIN_TIMESTAMP_CHECK_WINDOW								60
#define BLOCKCHAIN_TIMESTAMP_CHECK_WINDOW_V2						11
#define BLOCKCHAIN_TIMESTAMP_CHECK_WINDOW_V9						BLOCKCHAIN_TIMESTAMP_CHECK_WINDOW_V2

// MONEY_SUPPLY - total number coins to be generated
#define MONEY_SUPPLY																		((uint64_t)75000000000000000)
#define EMISSION_SPEED_FACTOR_PER_MINUTE								(21)
#define EMISSION_SPEED_FACTOR_V16												(20)
#define FINAL_SUBSIDY_PER_MINUTE												((uint64_t)300000000)

#define BASE_REWARD_V17																	((uint64_t)55550000000)		// Static reward before first halving
/*
	Blocks per:
	
	24h:		720 BLOCKS (3600 * 24 / 120)
	1 Month:	21600 BLOCKS (30 * 720, not exaclly)
	1 Year: 	262,8K BLOCKS (365 * 720)
*/
#define HALVING_EVERY_X_BLOCKS													262800		// Halving every 1 year
#define DEVS_REWARD_V17																	((uint64_t)12000000000000000)


#define CRYPTONOTE_REWARD_BLOCKS_WINDOW									100
#define CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE_V2		60000		// size of block (bytes) after which reward for block calculated using block size
#define CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE_V1		20000		// size of block (bytes) after which reward for block calculated using block size - before first fork
#define CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE_V5		300000		// size of block (bytes) after which reward for block calculated using block size - second change, from v5
#define CRYPTONOTE_LONG_TERM_BLOCK_WEIGHT_WINDOW_SIZE		100000		// size in blocks of the long term block weight median window
#define CRYPTONOTE_SHORT_TERM_BLOCK_WEIGHT_SURGE_FACTOR	50
#define CRYPTONOTE_COINBASE_BLOB_RESERVED_SIZE					600
#define CRYPTONOTE_DISPLAY_DECIMAL_POINT								9
// COIN - number of smallest units in one coin
#define COIN																						((uint64_t)1000000000)

#define FEE_PER_KB_OLD																	((uint64_t)10000000)
#define FEE_PER_KB																			((uint64_t)20000)
#define FEE_PER_BYTE																		((uint64_t)3)
#define DYNAMIC_FEE_PER_KB_BASE_FEE											((uint64_t)20000)
#define DYNAMIC_FEE_PER_KB_BASE_BLOCK_REWARD						((uint64_t)10000000000)
#define DYNAMIC_FEE_PER_KB_BASE_FEE_V5									((uint64_t)20000 * (uint64_t)CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE_V2 / CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE_V5)
#define DYNAMIC_FEE_PER_BYTE_BASE_FEE_V13								((uint64_t)(DYNAMIC_FEE_PER_KB_BASE_FEE_V5) / 20)
#define DYNAMIC_FEE_REFERENCE_TRANSACTION_WEIGHT				((uint64_t)750)

#define NEW_FEE_V18																			((uint64_t)(DYNAMIC_FEE_PER_BYTE_BASE_FEE_V13 / 100))		// Divide it 100 times


#define ORPHANED_BLOCKS_MAX_COUNT												100


#define DIFFICULTY_TARGET_V3														DIFFICULTY_TARGET_V2
#define DIFFICULTY_TARGET_V2														240		// seconds
#define DIFFICULTY_TARGET_V1														120		// seconds - before first fork
#define DIFFICULTY_WINDOW																720		// blocks
#define DIFFICULTY_WINDOW_V2														30
#define DIFFICULTY_WINDOW_V3														17
#define DIFFICULTY_LAG																	15		// !!!
#define DIFFICULTY_CUT																	60		// timestamps to cut after sorting

#define DIFFICULTY_TARGET_V10														120
#define DIFFICULTY_BLOCKS_COUNT_V10											(DIFFICULTY_WINDOW_V10 + 1)
#define BLOCKCHAIN_TIMESTAMP_CHECK_WINDOW_V10						11
#define CRYPTONOTE_BLOCK_FUTURE_TIME_LIMIT_V10					360
#define DIFFICULTY_WINDOW_V10														90

#define DIFFICULTY_TARGET_V11														120
#define DIFFICULTY_BLOCKS_COUNT_V11											(DIFFICULTY_WINDOW_V11 + 1)
#define BLOCKCHAIN_TIMESTAMP_CHECK_WINDOW_V11						11
#define CRYPTONOTE_BLOCK_FUTURE_TIME_LIMIT_V11					360
#define DIFFICULTY_WINDOW_V11														90

#define DIFFICULTY_TARGET_V16														120
#define DIFFICULTY_WINDOW_V16														45
#define BLOCKCHAIN_TIMESTAMP_CHECK_WINDOW_V16						11
#define CRYPTONOTE_BLOCK_FUTURE_TIME_LIMIT_V16					180
#define DIFFICULTY_BLOCKS_COUNT_V16											(DIFFICULTY_WINDOW_V16 + 1)

#define DIFFICULTY_BLOCKS_COUNT_V3											(DIFFICULTY_WINDOW_V3 + 1)
#define DIFFICULTY_BLOCKS_COUNT_V2											(DIFFICULTY_WINDOW_V2 + 1)		// added to make N=N
#define DIFFICULTY_BLOCKS_COUNT													DIFFICULTY_WINDOW + DIFFICULTY_LAG

#define CRYPTONOTE_LOCKED_TX_ALLOWED_DELTA_SECONDS_V1		DIFFICULTY_TARGET_V1 * CRYPTONOTE_LOCKED_TX_ALLOWED_DELTA_BLOCKS
#define CRYPTONOTE_LOCKED_TX_ALLOWED_DELTA_SECONDS_V2		DIFFICULTY_TARGET_V2 * CRYPTONOTE_LOCKED_TX_ALLOWED_DELTA_BLOCKS
#define CRYPTONOTE_LOCKED_TX_ALLOWED_DELTA_BLOCKS				1

#define DIFFICULTY_BLOCKS_ESTIMATE_TIMESPAN							DIFFICULTY_TARGET_V2 //just alias; used by tests

#define BLOCKS_IDS_SYNCHRONIZING_DEFAULT_COUNT					10000		// by default, blocks ids count in synchronizing

#define BLOCKS_SYNCHRONIZING_MAX_COUNT									2048		// must be a power of 2, greater than 128, equal to SEEDHASH_EPOCH_BLOCKS

#define CRYPTONOTE_MEMPOOL_TX_LIVETIME									(86400 * 3)		// seconds, three days
#define CRYPTONOTE_MEMPOOL_TX_FROM_ALT_BLOCK_LIVETIME		604800		// seconds, one week

#define COMMAND_RPC_GET_BLOCKS_FAST_MAX_COUNT						100

#define P2P_LOCAL_WHITE_PEERLIST_LIMIT									1000
#define P2P_LOCAL_GRAY_PEERLIST_LIMIT										5000

#define P2P_DEFAULT_CONNECTIONS_COUNT										32
#define P2P_DEFAULT_HANDSHAKE_INTERVAL									60		// seconds
#define P2P_DEFAULT_PACKET_MAX_SIZE											50000000		// 50000000 bytes maximum packet size
#define P2P_DEFAULT_PEERS_IN_HANDSHAKE									250
#define P2P_DEFAULT_CONNECTION_TIMEOUT									5000		// 5 seconds
#define P2P_DEFAULT_SOCKS_CONNECT_TIMEOUT								45		// seconds
#define P2P_DEFAULT_PING_CONNECTION_TIMEOUT							2000		// 2 seconds
#define P2P_DEFAULT_INVOKE_TIMEOUT											60 * 2 * 1000		// 2 minutes
#define P2P_DEFAULT_HANDSHAKE_INVOKE_TIMEOUT						5000		// 5 seconds
#define P2P_DEFAULT_WHITELIST_CONNECTIONS_PERCENT				70
#define P2P_DEFAULT_ANCHOR_CONNECTIONS_COUNT						2
#define P2P_DEFAULT_SYNC_SEARCH_CONNECTIONS_COUNT				2

#define P2P_DEFAULT_LIMIT_RATE_UP												8192		// Kbps
#define P2P_DEFAULT_LIMIT_RATE_DOWN											32768		// Kbps

#define P2P_FAILED_ADDR_FORGET_SECONDS									(60 * 60)		// 1 hour
#define P2P_IP_BLOCKTIME																(60 * 60 * 24)		// 24 hour
#define P2P_IP_FAILS_BEFORE_BLOCK												10
#define P2P_IDLE_CONNECTION_KILL_INTERVAL								(5 * 60)		// 5 minutes

#define P2P_SUPPORT_FLAG_FLUFFY_BLOCKS									0x01
#define P2P_SUPPORT_FLAGS																P2P_SUPPORT_FLAG_FLUFFY_BLOCKS

#define ALLOW_DEBUG_COMMANDS

#define CRYPTONOTE_NAME																	"morelo"
#define CRYPTONOTE_POOLDATA_FILENAME										"poolstate.bin"
#define CRYPTONOTE_BLOCKCHAINDATA_FILENAME							"data.mdb"
#define CRYPTONOTE_BLOCKCHAINDATA_LOCK_FILENAME					"lock.mdb"
#define P2P_NET_DATA_FILENAME														"p2pstate.bin"
#define RPC_PAYMENTS_DATA_FILENAME											"rpcpayments.bin"
#define MINER_CONFIG_FILE_NAME													"miner_conf.json"

#define THREAD_STACK_SIZE																5 * 1024 * 1024

#define HF_VERSION_DYNAMIC_FEE													4
#define HF_VERSION_MIN_MIXIN_4												  6
#define HF_VERSION_MIN_MIXIN_6												  7
#define HF_VERSION_ENFORCE_RCT												  6

#define HF_VERSION_MIN_MIXIN_10													13

#define HF_VERSION_LOWER_FEE														10
#define HF_VERSION_PER_BYTE_FEE													13
#define HF_FORBID_BORROMEAN															13
#define HF_VERSION_LONG_TERM_BLOCK_WEIGHT								14

#define PER_KB_FEE_QUANTIZATION_DECIMALS								8

#define HASH_OF_HASHES_STEP															512

#define DEFAULT_TXPOOL_MAX_WEIGHT												648000000ull		// 3 days at 300000, in bytes

#define BULLETPROOF_MAX_OUTPUTS													16

#define CRYPTONOTE_PRUNING_STRIPE_SIZE									4096		// the smaller, the smoother the increase
#define CRYPTONOTE_PRUNING_LOG_STRIPES									3		// the higher, the more space saved
#define CRYPTONOTE_PRUNING_TIP_BLOCKS										5500		// the smaller, the more space saved
//#define CRYPTONOTE_PRUNING_DEBUG_SPOOF_SEED

#define RPC_CREDITS_PER_HASH_SCALE											((float)(1<<24))

static constexpr uint64_t POISSON_CHECK_TRIGGER = 5;  // Reorg size that triggers poisson timestamp check
static constexpr uint64_t POISSON_CHECK_DEPTH = 128;  // Main-chain depth of the poisson check. The attacker will have to tamper 50% of those blocks
static constexpr double POISSON_LOG_P_REJECT = -75.0; // Reject reorg if the probablity that the timestamps are genuine is below e^x, -75 = 10^-33

// New constants are intended to go here
namespace config
{
   uint64_t const DEFAULT_DUST_THRESHOLD = ((uint64_t)20000); // Deprecated
   uint64_t const BASE_REWARD_CLAMP_THRESHOLD = ((uint64_t)100000);
   std::string const P2P_REMOTE_DEBUG_TRUSTED_PUB_KEY = "0000000000000000000000000000000000000000000000000000000000000000";

   uint64_t const CRYPTONOTE_PUBLIC_ADDRESS_BASE58_PREFIX = 0x1a29e1; // Wallet prefix: emo MainNet
   uint64_t const CRYPTONOTE_PUBLIC_INTEGRATED_ADDRESS_BASE58_PREFIX = 0x8a8e1; // Wallet prefix: emi MainNet
   uint64_t const CRYPTONOTE_PUBLIC_SUBADDRESS_BASE58_PREFIX = 0xeaae1; // Wallet prefix: ems MainNet
   uint16_t const P2P_DEFAULT_PORT = 38301;
   uint16_t const RPC_DEFAULT_PORT = 38302;
   uint16_t const ZMQ_DEFAULT_PORT = 38303;
   boost::uuids::uuid const NETWORK_ID = { {
       0x01, 0x11, 0x11, 0x11, 0xFF, 0xFF, 0xFF, 0x11, 0x44, 0x13, 0xFF, 0xFF, 0xFF, 0x55, 0x11, 0x1A
     } };
   std::string const GENESIS_TX = "011201ff00011e026bc5c7db8a664f652d78adb587ac4d759c6757258b64ef9cba3c0354e64fb2e42101abca6a39c561d0897be183eb0143990eba201aa7d2c652ab0555d28bb4b70728";
   uint32_t const GENESIS_NONCE = 38311;

   namespace testnet
   {
     uint64_t const CRYPTONOTE_PUBLIC_ADDRESS_BASE58_PREFIX = 0x80662; // Wallet prefix: eto... // decimal prefix: 21450
     uint64_t const CRYPTONOTE_PUBLIC_INTEGRATED_ADDRESS_BASE58_PREFIX = 0x4562; // Wallet prefix: eti... // decimal prefix: 20554
     uint64_t const CRYPTONOTE_PUBLIC_SUBADDRESS_BASE58_PREFIX = 0x4762; // Wallet prefix: ets... // decimal prefix: 21066
     uint16_t const P2P_DEFAULT_PORT = 38311;
     uint16_t const RPC_DEFAULT_PORT = 38312;
     uint16_t const ZMQ_DEFAULT_PORT = 38313;
     boost::uuids::uuid const NETWORK_ID = { {
         0x01, 0x11, 0x11, 0x11, 0xFF, 0xFF, 0xFF, 0x11, 0x44, 0x13, 0xFF, 0xFF, 0xFF, 0x55, 0x11, 0x1B
       } }; // TEST
   }

   namespace stagenet
   {
     uint64_t const CRYPTONOTE_PUBLIC_ADDRESS_BASE58_PREFIX = 0x7b62; // Wallet prefix: exo... // decimal prefix: 14794
     uint64_t const CRYPTONOTE_PUBLIC_INTEGRATED_ADDRESS_BASE58_PREFIX = 0x19f9e2; // Wallet prefix: exi... // decimal prefix: 1524426
     uint64_t const CRYPTONOTE_PUBLIC_SUBADDRESS_BASE58_PREFIX = 0x1ffbe2; // Wallet prefix: exs... // decimal prefix: 1934538
     uint16_t const P2P_DEFAULT_PORT = 38321;
     uint16_t const RPC_DEFAULT_PORT = 38322;
     uint16_t const ZMQ_DEFAULT_PORT = 38323;
     boost::uuids::uuid const NETWORK_ID = { {
         0x01, 0x11, 0x11, 0x11, 0xFF, 0xFF, 0xFF, 0x11, 0x44, 0x13, 0xFF, 0xFF, 0xFF, 0x55, 0x11, 0x1C
       } }; // STAGE
   }

   namespace blockchain_settings
   {
     const uint64_t PREMINE_BURN = 47000000000000000; // Will need to be set after knowing exact amount.
     const uint64_t MONEY_PREMINE = 47000000000000000; // Premine amnount
     const uint64_t MAXIMUM_BLOCK_SIZE_LIMIT = 2 * 1024 * 1024; // It is set to 2048kB (2MB)
     const uint64_t MINIMUM_BLOCK_SIZE_LIMIT = 1 * 1024 * 1024; // It is set to 1024kB (1MB)
     const uint8_t ARQMA_GENESIS_BLOCK_MAJOR_VERSION = 1;
     const uint8_t ARQMA_GENESIS_BLOCK_MINOR_VERSION = 1;
     const uint8_t ARQMA_BLOCK_UNLOCK_CONFIRMATIONS = 18; // How many blocks mined are needed to unlock block_reward.
   }

   namespace tx_settings
   {
     const uint8_t MORELO_TX_CONFIRMATIONS_REQUIRED = 4; // How many blocks are needed to confirm transaction sent.
     const uint8_t MORELO_TX_VERSION = 2; // Current Transaction Version Valid on Morelo Network
     const uint64_t TRANSACTION_SIZE_LIMIT = 48 * 1024; // I did set it to 48kB for now but it need to be verified.
     const uint64_t MAX_TRANSACTIONS_IN_BLOCK = 1024; // Maximum allowed transactions in One Block
   }

   namespace sync
   {
     const uint64_t HIGHEST_CHECKPOINT = 0;
     const size_t NORMAL_SYNC = 20;
     const size_t FAST_SYNC = 100;
   }

   namespace governance_old
   {
     static constexpr const char* MAINNET_WALLET_ADDRESS = "emo1hsdY9BLhGUUZqFqNUgGowHwEcW9LzQ9kjN8GJrqbBhXe6L37gZ5Y2pa8eNRiyzabyP8ScDyi2hcbzH4akSvE8SuYr1dFeX";
     static constexpr const char* TESTNET_WALLET_ADDRESS = "eto1UJjpbxTgM2kzrQNgrkazZLCyhGvcHaoFvsHypZ8JdvsXFQ9kb6zEMAxxJrkzBWKx1ZgxArfgiTudcrUvFBya7EwgQiftGn";
     static constexpr const char* STAGENET_WALLET_ADDRESS = "exoiwUAF2AEDC8oniRiMXKEkmsLEtw7XNVm2Lc2NJsejU4nN2zWrCF9cjrPgEgUeiHLQ8gNkwyoHVRmkGpS87Eqh4G1yAjseHF";
   }
   
   namespace devs
   {
     static constexpr const char* MAINNET_WALLET_ADDRESS = "emo1gezt9GAV78y39PaeoqSyXtcMNTSJoTJ9Wm3ejFoUWzLwKbH8fN17xbLQS96LdBBrPF57b9Cuz4UU8cekbwju2pvFcMv42w";
     static constexpr const char* TESTNET_WALLET_ADDRESS = "eto1j9F9GdLV78y39PaeoqSyXtcMNTSJoTJ9Wm3ejFoUWzLwKbH8fN17xbLQS96LdBBrPF57b9Cuz4UU8cekbwju2pvFduQGnP";
     static constexpr const char* STAGENET_WALLET_ADDRESS = "exojE3iKAX8V78y39PaeoqSyXtcMNTSJoTJ9Wm3ejFoUWzLwKbH8fN17xbLQS96LdBBrPF57b9Cuz4UU8cekbwju2pvFd9hNsM";
   }
   
   namespace devs_new 
   {
     static constexpr const char* MAINNET_WALLET_ADDRESS = "emo1devscWJ4UkbM3zxdcW8ELQUQJ69dZTFWwiWwYRLvYb5mmAtkcUr58yU6MskfrjCBUfPKd3Kio9oY6gcKoKZF7PoR7hBu3i";
     static constexpr const char* TESTNET_WALLET_ADDRESS = "eto1devsuxxM2jVmx2166mJoSLssgTzbrYWMRBqhodtzGDPJizpZGQw3RN3gRNXidCH9rdto22uJe76Vr8Rfkdfa9DTqUtzQzi";
     static constexpr const char* STAGENET_WALLET_ADDRESS = "exojDEvSAzPJCS9wCFp5yERNPSsUNyDZRanK35iVxvxaNwdocEWRakZbYA81BhYD9yf7B7XDkBYS2E8WUv6zaqFT2mwhRqnmvt";
   }
   
}

namespace cryptonote
{
  enum network_type : uint8_t
    {
      MAINNET = 0,
      TESTNET,
      STAGENET,
      FAKECHAIN,
      UNDEFINED = 255
    };
    struct config_t
    {
      uint64_t const CRYPTONOTE_PUBLIC_ADDRESS_BASE58_PREFIX;
      uint64_t const CRYPTONOTE_PUBLIC_INTEGRATED_ADDRESS_BASE58_PREFIX;
      uint64_t const CRYPTONOTE_PUBLIC_SUBADDRESS_BASE58_PREFIX;
      uint16_t const P2P_DEFAULT_PORT;
      uint16_t const RPC_DEFAULT_PORT;
      uint16_t const ZMQ_DEFAULT_PORT;
      boost::uuids::uuid const NETWORK_ID;
      std::string const GENESIS_TX;
      uint32_t const GENESIS_NONCE;
    };
    inline const config_t& get_config(network_type nettype)
    {
      static const config_t mainnet = {
        ::config::CRYPTONOTE_PUBLIC_ADDRESS_BASE58_PREFIX,
        ::config::CRYPTONOTE_PUBLIC_INTEGRATED_ADDRESS_BASE58_PREFIX,
        ::config::CRYPTONOTE_PUBLIC_SUBADDRESS_BASE58_PREFIX,
        ::config::P2P_DEFAULT_PORT,
        ::config::RPC_DEFAULT_PORT,
        ::config::ZMQ_DEFAULT_PORT,
        ::config::NETWORK_ID,
        ::config::GENESIS_TX,
        ::config::GENESIS_NONCE
      };
      static const config_t testnet = {
        ::config::testnet::CRYPTONOTE_PUBLIC_ADDRESS_BASE58_PREFIX,
        ::config::testnet::CRYPTONOTE_PUBLIC_INTEGRATED_ADDRESS_BASE58_PREFIX,
        ::config::testnet::CRYPTONOTE_PUBLIC_SUBADDRESS_BASE58_PREFIX,
        ::config::testnet::P2P_DEFAULT_PORT,
        ::config::testnet::RPC_DEFAULT_PORT,
        ::config::testnet::ZMQ_DEFAULT_PORT,
        ::config::testnet::NETWORK_ID,
        ::config::GENESIS_TX,
        ::config::GENESIS_NONCE
      };
      static const config_t stagenet = {
        ::config::stagenet::CRYPTONOTE_PUBLIC_ADDRESS_BASE58_PREFIX,
        ::config::stagenet::CRYPTONOTE_PUBLIC_INTEGRATED_ADDRESS_BASE58_PREFIX,
        ::config::stagenet::CRYPTONOTE_PUBLIC_SUBADDRESS_BASE58_PREFIX,
        ::config::stagenet::P2P_DEFAULT_PORT,
        ::config::stagenet::RPC_DEFAULT_PORT,
        ::config::stagenet::ZMQ_DEFAULT_PORT,
        ::config::stagenet::NETWORK_ID,
        ::config::GENESIS_TX,
        ::config::GENESIS_NONCE
      };
      switch (nettype)
      {
        case MAINNET: return mainnet;
        case TESTNET: return testnet;
        case STAGENET: return stagenet;
        case FAKECHAIN: return mainnet;
        default: throw std::runtime_error("Invalid network type");
      }
   };
}
