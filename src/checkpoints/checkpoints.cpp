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

#undef MORELO_DEFAULT_LOG_CATEGORY
#define MORELO_DEFAULT_LOG_CATEGORY "checkpoints"

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
    ADD_CHECKPOINT(235000, "94d05047dcb2f4db0dbd29fc6030302e4b89c0dc45d248f0f652317184f108dc");
    ADD_CHECKPOINT(237500, "19ec55e56c7a31a30e6d485f7aab54f18f62a412f17519077c849c9bb89036ed");
    ADD_CHECKPOINT(240000, "46f02bc205a4a79c88a9b83e574e8a3fc2f6dfee57e00db882db87fcb99b43ae");
    ADD_CHECKPOINT(242500, "be151432089eb5a94ee23d755f2d918839d77d53f39d6f754629105c1da52903");
    ADD_CHECKPOINT(245000, "29c1f9b1bebdd71315b5da13bbd4dc4a9f3999236a637b82edb34bd66df1ad59");
    ADD_CHECKPOINT(247500, "30bcd5f26a7be1e40d08f859977ed1d7ffe7ba8de1eb7742e731f7140f358d3e");
    ADD_CHECKPOINT(250000, "4f5bc71d70a9614f5396960ffd8f0765b567e1083258285099d1efbe05f67bbc");
    ADD_CHECKPOINT(252500, "5ebdffe032f53d5fd9dcf0d626f61ebff90e6a5599817a92aea9237f0390773a");
    ADD_CHECKPOINT(255000, "2df23914a6305e44e788fc88b22fa8b4fa8d41fcc52e32f7b757c5a8350fead9");
    ADD_CHECKPOINT(257500, "fa84f401eb11f832eefee81dedc6ebbd597dc01403b73fea94a115d312850bf9");
    ADD_CHECKPOINT(260000, "b61e5b1cca5ad45b737afe07ef251dae1ad4a0772553a2038229c2bf1ba3d408");
    ADD_CHECKPOINT(262500, "2cea59e0ebac8e37da8dd6905d251b611f93e45f4915a81af26d117da4845424");
    ADD_CHECKPOINT(265000, "d533d3c2cec3a5c558b76cc31d22c3f8eca0c5c11847fdc5f906f7c342f32e85");
    ADD_CHECKPOINT(267500, "9a3fba9a3e8a39ffa6d6d8820d9683595366d5ad9ad5b6170e427dab4939518e");
    ADD_CHECKPOINT(270000, "752d76da077599187a18c7ef5555a9821d9776fcbd858a0fff80d0e92fe4333d");
    ADD_CHECKPOINT(272500, "f27fb7268694bd358076256cbb83b40956544b97ed9105e6316e2a3158ce5acb");
    ADD_CHECKPOINT(275000, "d7d1ba696e3732fc0526741b50bad1933036a5e1927f74c17b7aad5e61e3a9b7");
    ADD_CHECKPOINT(277500, "3a3de93de1283b1654fe76c255e508e62c1d3caa3a6799795eee9ef049aec577");
    ADD_CHECKPOINT(280000, "01f76c509d15cb6b6d0b4c4e9ecb2efd49dfc7f34b176d679290677a692c0d6e");
    ADD_CHECKPOINT(282500, "004ed2b6e5b64702495426094c1cf6fd206cf88828021796e7e59c55ac2baf48");
    ADD_CHECKPOINT(285000, "a71ca560430d9715d47b7b0191b29b85eb41f3d8caefb928802dffc64d35284c");
    ADD_CHECKPOINT(287500, "02ed866bafd9d315a14686afbf5c26cc7c6d6e34769c39f5829762a7524f0f17");
    ADD_CHECKPOINT(290000, "3b9eed7641d7417ac8f3f3f401bd590354c287e727411bb9fcf1fb6faec97031");
    ADD_CHECKPOINT(292500, "d0c0ec7ab52d823fd947ec1080df16bdd75f792d5e9cf00f4312018a1c94641a");
    ADD_CHECKPOINT(295000, "78409406a3f70ae134064554d9ffa3a40ebbaeef3230aaa06af9bf27cc47471a");
    ADD_CHECKPOINT(297500, "d8820a618a7ab7bb40c33d357d6fb8f6c77b1b988cfa42c9de211839bd55a699");

    ADD_CHECKPOINT(300000, "ccc1a087d01f2f73b3c2bdfa97699f5691dc1c5daa95cc3300f870d4f80d8745");
    ADD_CHECKPOINT(302500, "6f593731357cc09b3e88a5e4bde11a1e6df5e6b5775a9b263f53e16b4fda4e84");
    ADD_CHECKPOINT(305000, "d634f5f890746f9a41b13534cb58b37ce7a5d36438f207204fb34f64e05331d6");
    ADD_CHECKPOINT(307500, "544bb927bbabd9405244b51b51a8cddb4c0db92ed8a56244b804492eff785b97");
    ADD_CHECKPOINT(310000, "517cb6c2c1be9d04bf7069c21fb1d3958be5d90cd05e9d15652f039561dae4e9");
    ADD_CHECKPOINT(312500, "53bee1e716209acda8a90f305c50b6f5cd29b93c2e00d59fade7533bb5c225d7");
    ADD_CHECKPOINT(315000, "faf66abac8c6817e43709a37d7184206a14e5860691fdadbe223401b7e15fcd6");
    ADD_CHECKPOINT(317500, "407215c7aae9ef55ea06fbf5c674f76f96a2018df3614b1a7a19d57f44cd66ad");
    ADD_CHECKPOINT(320000, "3ae70616fd7c466c3425049147d74dec860ef812f7902f29e3e2d9a47eaa879e");
    ADD_CHECKPOINT(322500, "b1ef5cbbc348ef864b4cfc0a64f35f8e34bf72b03c31def64935836a47afb4ab");
    ADD_CHECKPOINT(325000, "59beb3aae268c1dbd6184cfb015b864d1a504399f95c0bda4530f9800f3fa2ac");
    ADD_CHECKPOINT(327500, "a351d877de57b8019bb9fc42917defdc59ce292e48bf6c7a571a5d3fe71762e2");
    ADD_CHECKPOINT(330000, "70f8d56c6ef666ace1288f5e368c3b75d4e0c92eeea8b3e00a41b409b23115a1");
    ADD_CHECKPOINT(332500, "61a7068a67dba6a0dec586bd31211d6ca8173bda90875ebff9ce02667386d2fd");
    ADD_CHECKPOINT(335000, "0916c35cddcf57c44ad8d60fc543a918e333b55ceafc95629b262c5637d50166");
    ADD_CHECKPOINT(337500, "4d2d303809e0e92457667303c1909fbfbbe2486269ebae14c8006fb3a4a4a69d");
    ADD_CHECKPOINT(340000, "9a19c20e1430290961456987e556f26c3d257605537de987aef73bb285e5f207");
    ADD_CHECKPOINT(342500, "3b40472d9d18fa6667c2f44905fd1b00c1e23c737ad23ffca7b3b8f5ffed6d7f");
    ADD_CHECKPOINT(345000, "f64ad51df072ff4e834765a79531267aed111b54add8a2870fdfdb2332ab8c3c");
    ADD_CHECKPOINT(347500, "f6a7a5b2011cb75b2d525e1f93be247c6e64005dae63ccbff7558c718f18178d");
    ADD_CHECKPOINT(350000, "5cd97b56eeeb48bc977778599d6502a28e18d268befe3d79f8ea48672a4b40c9");
    ADD_CHECKPOINT(352500, "209b676fb774481ed256c6777d2090fc0f9f7365fb36ec53fec1c633d5e48a2e");
    ADD_CHECKPOINT(355000, "69e910685dc08629402f6fb8fa08685f5bb9f2c14680e0675496758a9d8cc609");
    ADD_CHECKPOINT(357500, "10df470466abf2eb8229fea6e6aec55a74e96a6d177db20c8fcc2cb491e7b557");
    ADD_CHECKPOINT(360000, "6919a0e88660616eecef713ed580a9a1c2a4a8abe47f61e39515afb62ae4f9ac");
    ADD_CHECKPOINT(362500, "260ac660eab141fb0ec4cab3d47b70144ce2bf5af4867f92aa4a7d3ae424fc9f");
    ADD_CHECKPOINT(365000, "a120e524314363b96e8e5d63405a0b83b5832f671688f3cae6d1b2308b533e28");
    ADD_CHECKPOINT(367500, "3d6bef47531dacae0ca583a8dd9bc352d2ff1630741c2fcaed03efb7a3503e54");
    ADD_CHECKPOINT(370000, "2ab9ea733ed4054b462015eb16ad8e07bfb6fa97844a5fc6262dd463251ce913");
    ADD_CHECKPOINT(372500, "d4c6906bff256512298784bc147ed0df09eca4697da1266755092a1a0b669e2d");
    ADD_CHECKPOINT(375000, "ffa3f5b006a83f0c0dbd51b45624b719739dfcd9281d6f88c1a035ea716ca6eb");
    ADD_CHECKPOINT(377500, "89ba7aa9732e4ed8975850be751fb75dcd6cc31b3f4f2aeea6dea10a6bf30f1e");
    ADD_CHECKPOINT(380000, "3195b8dc24e25506f3f2736b7279b2b5a605a731ef7c2f115f82eea085b0e71b");
    ADD_CHECKPOINT(382500, "ce8add9856c72da94aa074a4bc48a80c01860394af567d432f3132a6609ae32f");
    ADD_CHECKPOINT(385000, "8105c30b366ea252ff3cb4627f48fa078f5271ae6a6719f3bf057e6ee2940901");
    ADD_CHECKPOINT(387500, "d9ab286605b7b9688d6c8608a24a11709ef2457c2631219dec1153cf671eca27");
    ADD_CHECKPOINT(390000, "0874d09cddb16f9433d35cd4666ce55111f3af1b7e217fe330f88c65c3c76b61");
    ADD_CHECKPOINT(392500, "cceec08eda32cec641c19870c00add9caf09229367385554d02b04806bdcf331");
    ADD_CHECKPOINT(395000, "71cea80600e39012bd074bd42be4bb55583683b0aae867d7a3c24ea4cfdde7bf");
    ADD_CHECKPOINT(397500, "d831b64ca3457d45ff344a39e564352a2c0278973fb084bba09a4f9734ee0edf");

    ADD_CHECKPOINT(400000, "ccb26eb66d7d447ad896070da280fdf15cc7603583f1a9a96e62950f9d8a4aa9");
    ADD_CHECKPOINT(402500, "179ca4591cc3e976ea0d7423dc7e51ddf2da541b3ffff9f2156ef5d82d168ca6");
    ADD_CHECKPOINT(405000, "a073db7b74721e21f86b52bfc0b5b663025e65dda5adfa95083421b2f0158589");
    ADD_CHECKPOINT(407500, "31ccf1c6c206c31fdff0fe4cbcb9a65e9beb4e6b7cbc6f62a2fa734d5092a510");
    ADD_CHECKPOINT(410000, "eb8599df0deea0ca24abba8c18c00e48d756640c1ecd383a2b5535ec6f1c8c5a");
    ADD_CHECKPOINT(412500, "2ddcfe3ca161a251600fe48d7d951c9170e99f52bcc1d4d80957562b7a779097");
    ADD_CHECKPOINT(415000, "0429e40739211512176c96bbf43fee3ad242f82a63380806e7d5572eb7178fd3");
    ADD_CHECKPOINT(417500, "2342a14de6ec575fd0f4ac4951a44de90b7cd23d14143e4db8fe1fbde56c45da");
    ADD_CHECKPOINT(420000, "aa84582118b423adb16f616ada8a1663bd67ca833d84c30c5e9fc5b17929aa49");
    ADD_CHECKPOINT(422500, "d9f96282992e0b5a3659fdabfeb8ec8cc1e60d254112490f2c74077db5876609");
    ADD_CHECKPOINT(425000, "e4feacc82c91f48fcaa17008b57dd180b3012589c675b58fe8feedac0053cd8a");
    ADD_CHECKPOINT(427500, "68d1fcba00784ab319b5d5c32849a80128f2bc6830c0cef689939c6c80df92e3");
    ADD_CHECKPOINT(430000, "2f81020d87e3a1d9c4cd5d2150eaec92a3b52f94d3cd13a697605430e2e04aef");
    ADD_CHECKPOINT(432500, "c88fc32c9b02fc2a5e1c23f5932ab13fdc885392f413f9d6d60d1c3870e169f3");
    ADD_CHECKPOINT(435000, "d9bc7497d3ac95f06bab216c5189c97ff516054e0a0a38b9479569bda7202e64");
    ADD_CHECKPOINT(437500, "7fec5b60dd04cb1dfdefa004b959a63eb4c8f669c1c626f09af9535c60b3fb7a");
    ADD_CHECKPOINT(440000, "9147f65ea65a7fe6419a9a4e62f55ac0b89b032642b093514daa89cfafea3860");
    ADD_CHECKPOINT(442500, "49fdd0b95adcb2bd854f8ff6c44e02cafcda7cc7d75312a29d27592ce0dce266");
    ADD_CHECKPOINT(445000, "4480bf88b87fc63b3707826574efec5bfbb207b21ffc42daa657962892927869");
    ADD_CHECKPOINT(447500, "5cec41f53beb24194d2f9991ddb85e11bcea2dbca103e6283e2c3dc9dec49fc1");
    ADD_CHECKPOINT(450000, "4d7f62cd4c6f2cd76f073f07283bde49b74a322126ed1edd0e812bf2beb6e3cb");
    ADD_CHECKPOINT(452500, "c8b6af24bffb813179c9716ef486a9981a97c6106892af476139eb873977ad81");
    ADD_CHECKPOINT(455000, "414a5e8d058b5b06ccc2d5e887e65a25a85c9174b879c7809f30ef77d29beec1");
    ADD_CHECKPOINT(457500, "c7952b280197a63a315de33f8d10dce317dd756e63d35deffcce242c371ff961");
    ADD_CHECKPOINT(460000, "2d02807725a3ff2acfa9b0f4b7d4839141518fc37d3d47a0e160da1083a71676");
    ADD_CHECKPOINT(462500, "60fa4d47c8899e1181aff6efef386d6f34a8d26603419739b41eed045305d365");
    ADD_CHECKPOINT(465000, "35f6506e4aa1cdd40c8f6a98307c72e6ec4b7e966891d8fc182c442e2063103d");
    ADD_CHECKPOINT(467500, "c166041077889183fea446e0b52c0e0776e78e5f7b377b41b3818fcc6f5b6031");
    ADD_CHECKPOINT(470000, "fe8abb3cac6fb2faa3d01b6a71787376f4c8855e70711a96c1a20007376f52ce");
    ADD_CHECKPOINT(472500, "25df0cfd6cf6f8c20a7db621df75aa2885681302e184c36a4f57672a9bbf7ffd");
    ADD_CHECKPOINT(475000, "97ac3228defb8b405ec173da24b06d824174974c77c0badade3a5c5ce3f1cc2d");
    ADD_CHECKPOINT(477500, "af38ab4479859d0c28d3d86c1f4be47f5c333f8dbdea78c452484135913e055f");
    ADD_CHECKPOINT(480000, "9d9a72fdc7921b24112de25f0181e4101930bcf8c1c782b04639d2b473427284");
    ADD_CHECKPOINT(482500, "997a9523d64663a78d3891c29d73d0dfc126f9734af1acc9d49e65b2eba9d673");
    ADD_CHECKPOINT(485000, "6e9eec1571f85fef2d3570c787692264d8432f4b5ed66211f40be5f777567bcc");
    ADD_CHECKPOINT(487500, "752f3f7170be5a9f312736b751230adc8e7605cfbc49ea0e29869da1d00088db");
    ADD_CHECKPOINT(490000, "bd40862c8cf84738338f2ea9943b9d30a922ad0d1b97640eb5d67590ab1ce651");
    ADD_CHECKPOINT(492500, "809b16f38166835494816ea8b4a218ec9a8036515ff2ae407d0027239be9acf2");
    ADD_CHECKPOINT(495000, "b943f417ad2d76d2a67cc47c5042c96724e7eee92d7c07aa5a8787f57837f135");
    ADD_CHECKPOINT(497500, "199afe78cb49b2a4c67c344aebb2b0481a8324293cef7ab7f8e569a8f17c0822");

    ADD_CHECKPOINT(500000, "256a4996a8b75fd7c432b7acc904afa5a861209d952fb7cfe26c50bf4da7c689");
    ADD_CHECKPOINT(502500, "b2e43e3b52cefe79425ebd9aa483f0fab0a2d7985bdea22882183ff3209ed94a");
    ADD_CHECKPOINT(505000, "1e412a7e3b0b1e28b78a99b9709c5d5101ffd112193c717a7884a38cfd8ab24a");
    ADD_CHECKPOINT(507500, "6a4702828ebbdbc0e57ffcd2fe15f436d75fbb70e7bfbe6843b464a162800073");
    ADD_CHECKPOINT(510000, "dcdfad5456e8f5c71efc991fec02835deccb5850efa7ef53754d9b47d0150337");
    ADD_CHECKPOINT(512500, "e97de4578cb70530a2c4853d758ce74cf353a46fd4a4eddc2368d4d4aec2d46a");
    ADD_CHECKPOINT(515000, "ec6a13930c9ff431d43458a3e9557c82fe20dac3fcde538c4f2eb4b3e6111613");
    ADD_CHECKPOINT(517500, "e38fc3a86e42a7f2ff5b3d56352741d2fccf235e711e018aed50b37a0ed0fb23");
    ADD_CHECKPOINT(520000, "8800afcc20f5e9c4c59e3c2105a867e7461b207111da15a8a0cde4a127e0429d");
    ADD_CHECKPOINT(522500, "0551c7a5e2b2f625444c3286df215a7efa27bef36d65e19ee2d4356d35547796");

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
