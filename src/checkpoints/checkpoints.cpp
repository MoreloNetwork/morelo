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

#include "checkpoints.h"

#include "common/dns_utils.h"
#include "string_tools.h"
#include "storages/portable_storage_template_helper.h" // epee json include
#include "serialization/keyvalue_serialization.h"

using namespace epee;

#undef ARQMA_DEFAULT_LOG_CATEGORY
#define ARQMA_DEFAULT_LOG_CATEGORY "checkpoints"

namespace cryptonote
{
  /**
   * @brief struct for loading a checkpoint from json
   */
  struct t_hashline
  {
    uint64_t height; //!< the height of the checkpoint
    std::string hash; //!< the hash for the checkpoint
        BEGIN_KV_SERIALIZE_MAP()
          KV_SERIALIZE(height)
          KV_SERIALIZE(hash)
        END_KV_SERIALIZE_MAP()
  };

  /**
   * @brief struct for loading many checkpoints from json
   */
  struct t_hash_json {
    std::vector<t_hashline> hashlines; //!< the checkpoint lines from the file
        BEGIN_KV_SERIALIZE_MAP()
          KV_SERIALIZE(hashlines)
        END_KV_SERIALIZE_MAP()
  };

  //---------------------------------------------------------------------------
  checkpoints::checkpoints()
  {
  }
  //---------------------------------------------------------------------------
  bool checkpoints::add_checkpoint(uint64_t height, const std::string& hash_str)
  {
    crypto::hash h = crypto::null_hash;
    bool r = epee::string_tools::hex_to_pod(hash_str, h);
    CHECK_AND_ASSERT_MES(r, false, "Failed to parse checkpoint hash string into binary representation!");

    // return false if adding at a height we already have AND the hash is different
    if (m_points.count(height))
    {
      CHECK_AND_ASSERT_MES(h == m_points[height], false, "Checkpoint at given height already exists, and hash for new checkpoint was different!");
    }
    m_points[height] = h;
    return true;
  }
  //---------------------------------------------------------------------------
  bool checkpoints::is_in_checkpoint_zone(uint64_t height) const
  {
    return !m_points.empty() && (height <= (--m_points.end())->first);
  }
  //---------------------------------------------------------------------------
  bool checkpoints::check_block(uint64_t height, const crypto::hash& h, bool& is_a_checkpoint) const
  {
    auto it = m_points.find(height);
    is_a_checkpoint = it != m_points.end();
    if(!is_a_checkpoint)
      return true;

    if(it->second == h)
    {
      MINFO("CHECKPOINT PASSED FOR HEIGHT " << height << " " << h);
      return true;
    }
    else
    {
      MWARNING("CHECKPOINT FAILED FOR HEIGHT " << height << ". EXPECTED HASH: " << it->second << ", FETCHED HASH: " << h);
      return false;
    }
  }
  //---------------------------------------------------------------------------
  bool checkpoints::check_block(uint64_t height, const crypto::hash& h) const
  {
    bool ignored;
    return check_block(height, h, ignored);
  }
  //---------------------------------------------------------------------------
  //FIXME: is this the desired behavior?
  bool checkpoints::is_alternative_block_allowed(uint64_t blockchain_height, uint64_t block_height) const
  {
    if (0 == block_height)
      return false;

    auto it = m_points.upper_bound(blockchain_height);
    // Is blockchain_height before the first checkpoint?
    if (it == m_points.begin())
      return true;

    --it;
    uint64_t checkpoint_height = it->first;
    return checkpoint_height < block_height;
  }
  //---------------------------------------------------------------------------
  uint64_t checkpoints::get_max_height() const
  {
    std::map<uint64_t, crypto::hash>::const_iterator highest = std::max_element(m_points.begin(), m_points.end(),
             (boost::bind(&std::map<uint64_t, crypto::hash>::value_type::first, boost::placeholders::_1) <
              boost::bind(&std::map<uint64_t, crypto::hash>::value_type::first, boost::placeholders::_2)));

    return highest->first;
  }
  //---------------------------------------------------------------------------
  const std::map<uint64_t, crypto::hash>& checkpoints::get_points() const
  {
    return m_points;
  }

  bool checkpoints::check_for_conflicts(const checkpoints& other) const
  {
    for (auto& pt : other.get_points())
    {
      if (m_points.count(pt.first))
      {
        CHECK_AND_ASSERT_MES(pt.second == m_points.at(pt.first), false, "Checkpoint at given height already exists, and hash for new checkpoint was different!");
      }
    }
    return true;
  }

  bool checkpoints::init_default_checkpoints(network_type nettype)
  {
    if (nettype == TESTNET)
    {
      ADD_CHECKPOINT(0, "60077b4d5cd49a1278d448c58b6854993d127fcaedbdeab82acff7f7fd86e328");
      return true;
    }
    if (nettype == STAGENET)
    {
      ADD_CHECKPOINT(0, "60077b4d5cd49a1278d448c58b6854993d127fcaedbdeab82acff7f7fd86e328");
      return true;
    }
	//checkpoints here
    ADD_CHECKPOINT(0, "4cfad7c7554f6877d266fc7e0883bc46d01b30c1d7c45a30fb439cdd275aa4bc");
	ADD_CHECKPOINT(2500, "ac7a0e1fcd548959fe8a2f66b8555ac708398d8ba5a9e3748d9f80c442309a98");
	ADD_CHECKPOINT(5000, "8baad678b806fdaa69be49c12843573af75dec5b436a61e29fd3c92835166f6a");
	ADD_CHECKPOINT(7500, "1812c369e94088150198e66473bb2451f5b82aa9ecf9992faae2d958e3a760c5");
	ADD_CHECKPOINT(10000, "65506114b734ad058aedb8bc667f076816f71cedee8a8e4db4d0e1a5e1177181");
	ADD_CHECKPOINT(12500, "f285c4fa9e555fa942f7e06b914fbe77063939e1144d1244a14118004e49ec13");
	ADD_CHECKPOINT(15000, "4b892f8bb7989ce9916107c7374a5fd36ac81c859dec17cc65e4f0b04ab222ec");
	ADD_CHECKPOINT(17500, "fa785d07beed7f165fa8aaa865152b9992113a638b14ba6eca9de9bd40a2e967");
	ADD_CHECKPOINT(20000, "fcceb32b81243eb34901bd7944699d625d0a73285140de3ea6df95631d9cf87a");
	ADD_CHECKPOINT(22500, "6bb154403d75b531904a43457d39a5020f8dbd8562a1d4739d8423a91ca3341f");
	ADD_CHECKPOINT(25000, "44b8ae0f09b3c1277b11dbb395bd0080c1dbe1b11bdce46179358a74b501afeb");
	ADD_CHECKPOINT(27500, "d315c1b2edab2028a0128145c299a42b7c7736672d4085ba3cc16722b241be74");
	ADD_CHECKPOINT(30000, "379f7b13a42aa21950a8f7ba9710db92949755df41b4830f158487f387d4bb99");
	ADD_CHECKPOINT(32500, "4cffdc4eea54a7c56c0842a6cfd9c4be88d5ca7990451164e09118a05c66bde2");
	ADD_CHECKPOINT(35000, "2097a0c15a074d7835d347f54dfbdeb51730fd46a931642938b25d2d889e2edf");
	ADD_CHECKPOINT(37500, "640a36e99fe878cca076cebf7a6ec4cfea64ccc02b39fab9f516017302381c40");
	ADD_CHECKPOINT(40000, "8ead4ba3d483edbf57492bbd6f6bed294dc0c612d1fa78782c00e0fa0fbdd4e3");
	ADD_CHECKPOINT(42500, "f195a23e6bb04e7b963597c8eca7ee587f281518146981d11e2f7c77f5cb0ff7");
	ADD_CHECKPOINT(45000, "4230db32ae1f2f0fc7fe3e2554d239de43754e9ef75aa27c31fc1b60ee885120");
	ADD_CHECKPOINT(47500, "5d9a8b559c1710fafaea5dc47fef39648b99f0b75343a3bbb9867976ab0a07da");
	ADD_CHECKPOINT(50000, "78c45a7d58a4e57c1c09f35a38470e996f44593420f76bcbb745acca69b052bb");
	ADD_CHECKPOINT(52500, "fb7627801c7f5184af2c278513b269c01ab1382f4aef094da5f33a3eb3618d50");
	ADD_CHECKPOINT(55000, "2d8d7a0f0ad21e09e0a7fb1e41be0fb509794e08a8625a4df69d42dcac1471f6");
	ADD_CHECKPOINT(57500, "0fe317130a4d47f1df023999b85859115519ba5df360c1c82ec9fcacc129b4ea");
	ADD_CHECKPOINT(60000, "c37bd69c681e3cfb99a9dd400460e8ef549f99a78b93156a05f1526a66d4c352");
	ADD_CHECKPOINT(62500, "1a6cd28d258a600d54c364ff7560dee331a95ee95ee83574b895c004341b9db4");
	ADD_CHECKPOINT(65000, "45facf9aeef83df5b96502cf3d7de6f9dc538b589e4f021401cf72ca509192b0");
	ADD_CHECKPOINT(67500, "370baecb71410a28d0d72ffc2db3dc8037ea115a112d45578487812e452368af");
	ADD_CHECKPOINT(70000, "3e89ee0c10f556edda3d84f4b93dd721ce882ad2e8f682041b5d40e3d801e67b");
	ADD_CHECKPOINT(72500, "77271451b405740690ed752e959b53b273497b31fb143644e49891a0c35caa5b");
	ADD_CHECKPOINT(75000, "a4e640741f4284afe5220aeef627a20ff2a1a00d17320ce37f5223e8bcdf4d94");
	ADD_CHECKPOINT(77500, "92afe546ef2e392b6295b19eb0e87ab06c117ed0c9a176ebfb0c852753b3bc88");
	ADD_CHECKPOINT(80000, "27c33674cf87ea36bf4953db4ebafcdf2ea0d5bb7bf1f536a7b55467df1c7f5f");
	ADD_CHECKPOINT(82500, "5d234a625e35efd8fc6374f02c9c2c983c9eb05f6a50348a1656bad532a93748");
	ADD_CHECKPOINT(85000, "aa32e8a70a6de0cba95cf712cdfe21eafe0748b1100b48bfd3a3681bf595f1fa");
	ADD_CHECKPOINT(87500, "b27c8093a3cf6facceac85f144c913976303bd7e834ccfb4aee053eb9bc242ba");
	ADD_CHECKPOINT(90000, "cc78524dc4fbf221fb20eae66652458b96447b0fd67d669528cff13419ed762e");
	ADD_CHECKPOINT(92500, "595f3c313edf047cab9c0e78ca94b34799fac4d78dd0fbbc0bfc52def1d872ea");
	ADD_CHECKPOINT(95000, "e4ad230274b9849548d7279fef043fcb8183dc944342b59d53af24ab4b00fc63");
	ADD_CHECKPOINT(97500, "6e2e4b998c256a3f52bbf40298a55389d2eeded17d198541ff04f88b4af8e1a6");
	ADD_CHECKPOINT(100000, "5b772d0f1808713af691dc66467cd3030fe3c5004be0dd56a313a28b9760c77a");
	ADD_CHECKPOINT(102500, "35ac5535317cf266fb306a6d029f687f5786c3565bf14c9735586e98f4d1cffc");
	ADD_CHECKPOINT(105000, "c90024b241671d22a9232c463efa37d6c23de025457eb5f4372722b7982f6f0c");
	ADD_CHECKPOINT(107500, "2c6b4b4484647f9109ec7e838da70b0cffbc5fbd8f43a8290632054c84f5b17e");
	ADD_CHECKPOINT(110000, "d30e67ffa5d44ef9ad6ef2ce56f240c007f01299c60b8b37c8c73a94c4ea992f");
	ADD_CHECKPOINT(112500, "e059723f07fa27cda8a76ef7f4f9c079969fcdbe9ba492a317db8239b8e0d1b7");
	ADD_CHECKPOINT(115000, "7ccc6c784708a232929066cd6ae67ba1520d94333a4f9b91b4801b1284aa26ba");
	ADD_CHECKPOINT(117500, "88415999bdc0883ed31a4e99edf99cccdb7c9723db04c6268077721808549f4c");
	ADD_CHECKPOINT(120000, "2d2b9918234b90ff70f84b199d7dc92c729e03cb5efc824f3dfbc95e386596f0");
	ADD_CHECKPOINT(122500, "84ec6471a7975edab2e7565ad44d4e0ef64328aa89cbacb9089ebb96592dfb9d");
	ADD_CHECKPOINT(125000, "51974b84f62dd01dc356941188d5d883e8a9afda62e0f636c6ae91f3b427628d");
	ADD_CHECKPOINT(127500, "5b017b052e01a5edbc261d190ceaa1ef533a21979b61383e07a62a2a7ce15c0e");
	ADD_CHECKPOINT(130000, "f866f24072c9f61f819c06581be53edd3ee289de86992bbc0d0fda6fa7f6befa");
	ADD_CHECKPOINT(132500, "2cddcc0a3c6f029f745d25b85451a6599764780127dedcfdafa79a9cb6ca17a3");
	ADD_CHECKPOINT(135000, "7fae6f615a93ebea8ed0d476b1aad19efe0b773afda786ae27f8e19e66219b8f");
	ADD_CHECKPOINT(137500, "a3a2141eb4c78d946a8e85defb644461b131fd726662057244602b39770b8c78");
	ADD_CHECKPOINT(140000, "e26e8fbc8761b66cd7500ade8e8f242f8e038b2ea5c0fa7def9301781169d4e5");
	ADD_CHECKPOINT(142500, "75c391192f8676f4b98dbd05fd6a31d6cf413c3462d71aadf81557c3210c9b33");
	ADD_CHECKPOINT(145000, "4582098c17a77069c26308f62e0c7715bcaf5dbcf2db17f58f753a6f8dc85c00");
	ADD_CHECKPOINT(147500, "53a6ccb2a8073836721411478c81e0649da25a03a6068dbcb835d33ae20dc556");
	ADD_CHECKPOINT(150000, "718d712acdd93df60b1c53772fba77de6b002ca8179182fd19a80b6a9edebffc");
	ADD_CHECKPOINT(152500, "c13dd4e2456306f659ca51db9c71b4a5b1b20576639208c65f444edceba99c5e");
	ADD_CHECKPOINT(155000, "731d453bef88c3c72cb0bfce4d235fcb2b268c49bf0af924fcbe7b31f33a0aa3");
	ADD_CHECKPOINT(157500, "e829005cf0e6c9b791a72cb14476dd00c3e63b8d5d05e312aa4fa89be02dd720");
	ADD_CHECKPOINT(160000, "dc3c2131802c50ceece86dc1b417e768b6e21168aaf906372ff22f70d8397b8d");
	ADD_CHECKPOINT(162500, "1b313a66f09034b2b268ff5f8fdb35c1a8c58f4c988d8c4ef597927ce76bb85d");
	ADD_CHECKPOINT(165000, "4343b87ffcac2b1e1a5219bcbef95336ed34abf121d4fd066a220cec2306418e");
	ADD_CHECKPOINT(167500, "7a93975429e84b9e7bb45fc94fa9cfa2719a8ac5db32f4d9d96ab8476c08338e");
	ADD_CHECKPOINT(170000, "d83e166f5dab955fe9dfc3a11f6fb2d331b29e29701efff6427a23abbf4fe42b");
	ADD_CHECKPOINT(172500, "c95a5e78e31d89da0157e6c81625bace05e19e85192e4fe30215547283eda4a6");
	ADD_CHECKPOINT(175000, "e3eccd93f58d503e315ec51d603133a52effae45f2abea65c3680b70e930e3bc");
	ADD_CHECKPOINT(177500, "0ac1fa7dcd40815c78f046cca90daa2274b5258c2ea22baee5bc1e91ed982662");
	ADD_CHECKPOINT(180000, "168557bb17b904e0cb8b30c5913446511b9788cd3d65dff835446d4c8b9ed40b");
	ADD_CHECKPOINT(182500, "f99cbdb22191a24797cec2e428e7dcb4afa5556c55f7324ec4c02f3ee2c0092b");
	ADD_CHECKPOINT(185000, "6a871d9176ad57042f40eb025ea06d7923168c335997d6134c78ccb161d04cfa");
	ADD_CHECKPOINT(187500, "9e43273c982a96c98eedc4ba13551b604899a267be1a31ac41312a2220362ae9");
	ADD_CHECKPOINT(190000, "ac0c0e8beb15bdc62320a2cd9df29eb05171159118ef86abbc214232effbffe8");
	ADD_CHECKPOINT(192500, "3a17db3b4b183fbabf61efd921fea1a6a4d0091d0010d931549db987afcfc436");
	ADD_CHECKPOINT(195000, "c1b5cf307b63cbcc448aabf01e8d8037177a96efdde45ef0663f427a0bf86f37");
	ADD_CHECKPOINT(197500, "f849005382fad61330c25bd39257b3bba14ee2612d164e762e4a2f94d9ff3de7");
	ADD_CHECKPOINT(200000, "bfce94b8798efd7f41e66591645fc7a69d6a07a60d16133e1495f6540dc595bf");
	ADD_CHECKPOINT(202500, "4dc354279fdb5d0f80d443c363786afce13fce342f70a96e6ac9052fc5c5e75d");
	ADD_CHECKPOINT(205000, "d8d56a5bd901a15981e5fa44ac14dee694f7ec068d06e917afedf083b7be890f");
	ADD_CHECKPOINT(207500, "434091d745f9444937a9326ed868d220099a7ad407fdaef60303cc525d6ad573");
	ADD_CHECKPOINT(210000, "5901895d388e5b53f339d4e29b83df0fb6efbaac840fe6220e47925326489262");
	ADD_CHECKPOINT(212500, "7a965790875ff31fde1a1915ffa58c83399f27313deebd100c184cc725f77974");
	ADD_CHECKPOINT(215000, "6b6984ad84ff7fc2a54811fe95a831f8ce81ea6b964f22506efd0310d1153caa");
	ADD_CHECKPOINT(217500, "cc59bb998f0dc4a1a726a8c887379a6c35b2ee66559c01a64395ccb22e0df546");
	ADD_CHECKPOINT(220000, "3b7e86d3f3e0bbf13b0d1031714218612ca4570444d6832bf5a769ec37525391");
	ADD_CHECKPOINT(222500, "4503e61101c5da56faaa82c359e58099dbb9979b5eca07b95a5f92586be8a555");
	ADD_CHECKPOINT(225000, "558e5eb63895993b49dbdc75d30135ad1eb800507715ad1bd3440aa61dccd6b1");
	ADD_CHECKPOINT(227500, "dafcc3794899566379d77ed9f104e16003a30945597d6617be356f6bce03d3df");
	ADD_CHECKPOINT(230000, "07182f3ca5ea2d34aab23141eb2c1246cd3005a7ef30c7fc63a46a32248eac3b");
	ADD_CHECKPOINT(232500, "809282c0531fe3fcbd3625a1009672289888110d85215e4fd071d99d2f54713a");

    return true;
  }

  bool checkpoints::load_checkpoints_from_json(const std::string &json_hashfile_fullpath)
  {
    boost::system::error_code errcode;
    if(!(boost::filesystem::exists(json_hashfile_fullpath, errcode)))
    {
      LOG_PRINT_L1("Blockchain checkpoints file not found");
      return true;
    }

    LOG_PRINT_L1("Adding checkpoints from blockchain hashfile");

    uint64_t prev_max_height = get_max_height();
    LOG_PRINT_L1("Hard-coded max checkpoint height is " << prev_max_height);
    t_hash_json hashes;
    if(!epee::serialization::load_t_from_json_file(hashes, json_hashfile_fullpath))
    {
      MERROR("Error loading checkpoints from " << json_hashfile_fullpath);
      return true;
    }
    for(std::vector<t_hashline>::const_iterator it = hashes.hashlines.begin(); it != hashes.hashlines.end(); )
    {
      uint64_t height;
      height = it->height;
      if (height <= prev_max_height) {
	      LOG_PRINT_L1("ignoring checkpoint height " << height);
      } else {
	      std::string blockhash = it->hash;
	      LOG_PRINT_L1("Adding checkpoint height " << height << ", hash=" << blockhash);
	      ADD_CHECKPOINT(height, blockhash);
      }
      ++it;
    }

    return true;
  }

  bool checkpoints::load_checkpoints_from_dns(network_type nettype)
  {
    std::vector<std::string> records;

    // All Morelo Network domains have DNSSEC on and valid
    static const std::vector<std::string> dns_urls = {
      "mainpoints.morelo.cc", "mainpoints.morelonetwork.pl", "mainpoints.morelo.vip"
    };
    static const std::vector<std::string> testnet_dns_urls = {
      "testpoints.morelo.cc", "testpoints.morelonetwork.pl", "testpoints.morelo.vip"
    };
    static const std::vector<std::string> stagenet_dns_urls = {
      "stagepoints.morelo.cc", "stagepoints.morelonetwork.pl", "stagepoints.morelo.vip"
    };

    if (!tools::dns_utils::load_txt_records_from_dns(records, nettype == TESTNET ? testnet_dns_urls : nettype == STAGENET ? stagenet_dns_urls : dns_urls))
      return true;

    for (const auto& record : records)
    {
      auto pos = record.find(":");
      if (pos != std::string::npos)
      {
        uint64_t height;
        crypto::hash hash;

        // parse the first part as uint64_t,
        // if this fails move on to the next record
        std::stringstream ss(record.substr(0, pos));
        if (!(ss >> height))
        {
          continue;
        }

        // parse the second part as crypto::hash,
        // if this fails move on to the next record
        std::string hashStr = record.substr(pos + 1);
        if (!epee::string_tools::hex_to_pod(hashStr, hash))
        {
          continue;
        }

        ADD_CHECKPOINT(height, hashStr);
      }
    }
    return true;
  }

  bool checkpoints::load_new_checkpoints(const std::string &json_hashfile_fullpath, network_type nettype, bool dns)
  {
    bool result;

    result = load_checkpoints_from_json(json_hashfile_fullpath);
    if (dns)
    {
      result &= load_checkpoints_from_dns(nettype);
    }

    return result;
  }
}
