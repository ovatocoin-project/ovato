// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include "chainparamsseeds.h"

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
 *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: 4a5e1e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "Bloomberg 15/Jan/2019 Global Debt of 244 Trillion USD Nears Record Despite Faster Growth";
    const CScript genesisOutputScript = CScript() << ParseHex("040184710fa689ad5023690c80f3a49c8f13f8d45b8c857fbcbc8bc4a8e4d3eb4b10f4d4604fa08dce601aaf0f470216fe1b51850b4acf21b179c45070ac7b03a9") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

void CChainParams::UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    consensus.vDeployments[d].nStartTime = nStartTime;
    consensus.vDeployments[d].nTimeout = nTimeout;
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 100000;
        consensus.BIP34Height = 1;
        consensus.BIP34Hash = uint256S("0xc1cc247ce8037c73260a498fd0295958619808cb55a88e57d538fd42b4404165");
        consensus.BIP65Height = 1; // bab3041e8977e0dc3eeff63fe707b92bde1dd449d8efafb248c27c8264cc311a
        consensus.BIP66Height = 1; // 7aceee012833fa8952f8835d8b1b3ae233cd6ab08fdb27a771d2bd7bdc491894
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); 
        consensus.nPowTargetTimespan = 1 * 24 * 60 * 60; // Ovato: 1 days
        consensus.nPowTargetSpacing = 30;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 2160; // 75% of 8064
        consensus.nMinerConfirmationWindow = 2880;       // nPowTargetTimespan / nPowTargetSpacing * 4
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1485561600; // January 28, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1729079471;   // January 31st, 2018

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1485561600; // January 28, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1529079471;   // January 31st, 2018

        // Check transactions for duplicated inputs from this height
        consensus.nDuplicateInputHeight = std::numeric_limits<int>::max();

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000000000000000");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("409c01e87729e8d899d2c215231af1fe4f460ae9b1573c9b0b4ad87dca3c504a"); //1100000

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xd0;
        pchMessageStart[1] = 0xe1;
        pchMessageStart[2] = 0xf5;
        pchMessageStart[3] = 0xec;
        nDefaultPort = 11003;
        nPruneAfterHeight = 900000;

        genesis = CreateGenesisBlock(1565951079, 1085399677, 0x1e0ffff0, 1, 25 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0xc1cc247ce8037c73260a498fd0295958619808cb55a88e57d538fd42b4404165"));
        assert(genesis.hashMerkleRoot == uint256S("0x0a869b8a4665ff6d7cba425e26d3b1954382c78be1896e12fb64d9f2e77d6c46"));

        // Note that of those with the service bits flag, most only support a subset of possible options
        vSeeds.emplace_back("dnsseed.ovato.com", true);

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 115);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,5);
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,50);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,243);
        base58Prefixes[EXT_PUBLIC_KEY] = {0xef, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0xef, 0x88, 0xAD, 0xE4};

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;

        checkpointData = (CCheckpointData) {
            {
                {0, uint256S("0xc1cc247ce8037c73260a498fd0295958619808cb55a88e57d538fd42b4404165")},
                {50, uint256S("0e7a4ec33ab2c88806504d78fac8b5061446d594c61238b42624cc56e5d15fac")},
                {500, uint256S("9a3ee4f3c3f66bbdf4a4ef0c1707a1a50c37acfd3ab57e613704ac06c045dfc7")},
                {1500, uint256S("8bd8f4622a5356746dbd6aed3b39cec6a9d0ca1a2681d2e898b31475dc5b5cfc")},
                {5000, uint256S("748b80b32c902bce18fc2a0da81661f669c54351c03312bd224dc9aefaa02f42")},
                {15000, uint256S("3ac267065d40d6ccc59ed8ca83eddc315303312789b96ce31a0887c9a39a81ad")},
                {50000, uint256S("c540c9ffe890eaff532372156c2731e4e083f42b78cf2c38e40a853564ce6299")},
                {100000, uint256S("9697618b1e8894cd1cc1eff4cf3b1304826f2c6c3e5ec08a339a259750cc65e3")},
                {200000, uint256S("d1c2c6675911a9b7bc681fb4ffb82775dc392159f10d0f62e6f07b1908b75139")},
                {358664, uint256S("98fbf2263eb467c862717f7939cdff23f92d0db98622ce8bc0e5953ceabc4ff9")},
                {500000, uint256S("59e46f597450f447e81d53bb1cf531060176b79905d47e088cb097d7028acac7")},
                {600000, uint256S("6d9d974a6417387b39e73368988b351d5565f664f51bff42b9a42c0ec50fe9a1")},
                {700007, uint256S("1211c542380fea16298cd84261101e0a5c06dec4b7d3163e3d0a7e3dbac6d78d")},
                {909090, uint256S("5d3a2978ad89da1e1576c5d9eaee8ecef8cf50b9da2b805db363e38fd4b5cfa2")},
                {1111111, uint256S("612fcc1fb5339b9b3e322cfeb1f75e9082aeca6cd82005e7de4c737ddc54672d")},


            }};

        chainTxData = ChainTxData{
            // Data as of block 59c9b9d3fec105bdc716d84caa7579503d5b05b73618d0bf2d5fa639f780a011 (height 1353397).
            1603683929, // * UNIX timestamp of last known number of transactions
            388703,     // * total number of transactions between genesis and that timestamp
                        //   (the tx=... number in the SetBestChain debug.log lines)
            0           // * estimated number of transactions per second after that timestamp
        };
    }
};

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 100000;
        consensus.BIP34Height = 1;
        consensus.BIP34Hash = uint256S("0xc1cc247ce8037c73260a498fd0295958619808cb55a88e57d538fd42b4404165");
        consensus.BIP65Height = 72; // 8075c771ed8b495ffd943980a95f702ab34fce3c8c54e379548bda33cc8c0573
        consensus.BIP66Height = 74; // 8075c771ed8b495ffd943980a95f702ab34fce3c8c54e379548bda33cc8c0573
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 1 * 24 * 60 * 60; // 3.5 days
        consensus.nPowTargetSpacing = 30;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1483228800; // January 1, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1529079471;   // January 31st, 2018

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1483228800; // January 1, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1529079471;   // January 31st, 2018

        // Check transactions for duplicated inputs from this height
        consensus.nDuplicateInputHeight = std::numeric_limits<int>::max();

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000000000000000");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0xa0afbded94d4be233e191525dc2d467af5c7eab3143c852c3cd549831022aad6"); //343833

        pchMessageStart[0] = 0xfe;
        pchMessageStart[1] = 0xd3;
        pchMessageStart[2] = 0xc9;
        pchMessageStart[3] = 0xf1;
        nDefaultPort = 19335;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1565951079, 1085399677, 0x1e0ffff0, 1, 25 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0xc1cc247ce8037c73260a498fd0295958619808cb55a88e57d538fd42b4404165"));
        assert(genesis.hashMerkleRoot == uint256S("0x0a869b8a4665ff6d7cba425e26d3b1954382c78be1896e12fb64d9f2e77d6c46"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        vSeeds.emplace_back("dnsseed-testnet.ovato.com", true);

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,58);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0xff, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0xff, 0x35, 0x83, 0x94};

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;

        checkpointData = (CCheckpointData) {
            {
                {0, uint256S("0xc1cc247ce8037c73260a498fd0295958619808cb55a88e57d538fd42b4404165")},
                {50, uint256S("91679e7d25a160298a87a18b1ce5a2b256ea95ea76cc8c5bada6014ecf00ec4a")},
                {1500, uint256S("aa78f726598b7c7af752ae7c546a80426b07003f332429846d8cc27330f90063")},
                {3000, uint256S("94bd02be9025976cd2ba9b20853f15891d6d731b3cf131f1ff6c99a96a870041")},
                {5000, uint256S("748b80b32c902bce18fc2a0da81661f669c54351c03312bd224dc9aefaa02f42")},
                {15000, uint256S("3ac267065d40d6ccc59ed8ca83eddc315303312789b96ce31a0887c9a39a81ad")},
                {30000, uint256S("d31139f80970e60fb225c67dedc8f51ca9959361de2aa10c4e886be69243af00")},
                {51970, uint256S("320bbb06bd14db0ad24333d3ca9e132f5395132d25731827f47057e2288fef70")},
                {100000, uint256S("97ef2b85e088c7fffa958b7c9db30d71651963bff5661f7f1d0a5ffa167a99d4")},
                {200000, uint256S("64d515d74cdf97c8fafee8eec717a0011ea677227b12bff3bd045c9408356dd0")},
                {300000, uint256S("02679ca85973ed70b8c38df1ec93f05e59320f6a17012429ab3c192dcccbec84")},
                {505050, uint256S("c7857c2da18de047b225696ffff080aa573bf2f21b2c10adc4fe500fbc145d3d")},
                {803803, uint256S("7fe92d0f25acfe63e2bda855a8cca6dd0de981328fc698e0d1f2e5fc6b366b9f")},


            }};

        chainTxData = ChainTxData{
            // Data as of block a0afbded94d4be233e191525dc2d467af5c7eab3143c852c3cd549831022aad6 (height 343833)
            1603685347,
            52446,
            0};

    }
};

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 100000;
        consensus.BIP34Height = 1;
        consensus.BIP34Hash = uint256S("0xc1cc247ce8037c73260a498fd0295958619808cb55a88e57d538fd42b4404165");
        consensus.BIP65Height = 1; // 8075c771ed8b495ffd943980a95f702ab34fce3c8c54e379548bda33cc8c0573
        consensus.BIP66Height = 1; // 8075c771ed8b495ffd943980a95f702ab34fce3c8c54e379548bda33cc8c0573
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 1 * 24 * 60 * 60; // 3.5 days
        consensus.nPowTargetSpacing = 30;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016;       // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999;   // December 31, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1483228800; // January 1, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1529079471;   // January 31st, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1483228800; // January 1, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1529079471;   // January 31st, 2018

        // Check transactions for duplicated inputs from this height
        consensus.nDuplicateInputHeight = std::numeric_limits<int>::max();

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000000000000000");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x0000000000000000000000000000000000000000000000000000000000000000"); //343833

        pchMessageStart[0] = 0xfe;
        pchMessageStart[1] = 0xd3;
        pchMessageStart[2] = 0xc9;
        pchMessageStart[3] = 0xf1;
        nDefaultPort = 19336;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1565951079, 1085399677, 0x1e0ffff0, 1, 25 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0xc1cc247ce8037c73260a498fd0295958619808cb55a88e57d538fd42b4404165"));
        assert(genesis.hashMerkleRoot == uint256S("0x0a869b8a4665ff6d7cba425e26d3b1954382c78be1896e12fb64d9f2e77d6c46"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,58);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0xff, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0xff, 0x35, 0x83, 0x94};

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true; 

        checkpointData = (CCheckpointData) {
            {
                {0, uint256S("0xc1cc247ce8037c73260a498fd0295958619808cb55a88e57d538fd42b4404165")},
            }
        };

        chainTxData = ChainTxData{
            // Data as of block 320bbb06bd14db0ad24333d3ca9e132f5395132d25731827f47057e2288fef70 (height 51970)
            1603685347,
            52446,
            0
        };
    }
};

static std::unique_ptr<CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<CChainParams> CreateChainParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return std::unique_ptr<CChainParams>(new CMainParams());
    else if (chain == CBaseChainParams::TESTNET)
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    else if (chain == CBaseChainParams::REGTEST)
        return std::unique_ptr<CChainParams>(new CRegTestParams());
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(network);
}

void UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    globalChainParams->UpdateVersionBitsParameters(d, nStartTime, nTimeout);
}
