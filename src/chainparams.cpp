// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Copyright (c) 2017-2021 The Raven Core developers
// Copyright (c) 2024 The TuriCoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"
#include "arith_uint256.h"

#include <assert.h>
#include "chainparamsseeds.h"

//TODO: Take these out
extern double algoHashTotal[16];
extern int algoHashHits[16];


static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << CScriptNum(0) << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
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
    const char* pszTimestamp = "TuriCoin 03/Nov/2024 A new era of decentralized digital currency begins";
    const CScript genesisOutputScript = CScript() << ParseHex("04678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5f") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

void CChainParams::UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    consensus.vDeployments[d].nStartTime = nStartTime;
    consensus.vDeployments[d].nTimeout = nTimeout;
}

void CChainParams::TurnOffSegwit() {
	consensus.nSegwitEnabled = false;
}

void CChainParams::TurnOffCSV() {
	consensus.nCSVEnabled = false;
}

void CChainParams::TurnOffBIP34() {
	consensus.nBIP34Enabled = false;
}

void CChainParams::TurnOffBIP65() {
	consensus.nBIP65Enabled = false;
}

void CChainParams::TurnOffBIP66() {
	consensus.nBIP66Enabled = false;
}

bool CChainParams::BIP34() {
	return consensus.nBIP34Enabled;
}

bool CChainParams::BIP65() {
	return consensus.nBIP34Enabled;
}

bool CChainParams::BIP66() {
	return consensus.nBIP34Enabled;
}

bool CChainParams::CSVEnabled() const{
	return consensus.nCSVEnabled;
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
        consensus.nSubsidyHalvingInterval = 5040000;  //~ 4 yrs at 25 sec block time
        consensus.nBIP34Enabled = true;
        consensus.nBIP65Enabled = true; // 000000000000000004c2b624ed5d7756c508d90fd0da2c7c679febfa6c4735f0
        consensus.nBIP66Enabled = true;
        consensus.nSegwitEnabled = true;
        consensus.nCSVEnabled = true;
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.kawpowLimit = uint256S("0000000000ffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // Estimated starting diff for first 180 kawpow blocks
        consensus.nPowTargetTimespan = 50 * 25; // 50 blocks * 25 seconds = ~20.8 minutes
        consensus.nPowTargetSpacing = 25;
		consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 40; // Approx 80% of 50
        consensus.nMinerConfirmationWindow = 50; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nOverrideRuleChangeActivationThreshold = 45;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nOverrideMinerConfirmationWindow = 50;
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].bit = 6;  //Assets (RIP2)
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nStartTime = 1730678400; // Nov 4, 2024
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nTimeout = 1762214400; // Nov 4, 2025
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nOverrideRuleChangeActivationThreshold = 45;
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nOverrideMinerConfirmationWindow = 50;
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].bit = 7;  // Assets (RIP5)
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nStartTime = 1730678400; // UTC: Nov 4, 2024
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nTimeout = 1762214400; // UTC: Nov 4, 2025
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nOverrideRuleChangeActivationThreshold = 42; // Approx 85% of 50
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nOverrideMinerConfirmationWindow = 50;
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].bit = 8;
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nStartTime = 1730678400; // UTC: Nov 4, 2024
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nTimeout = 1762214400; // UTC: Nov 4, 2025
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nOverrideRuleChangeActivationThreshold = 42; // Approx 85% of 50
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nOverrideMinerConfirmationWindow = 50;
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].bit = 9;
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nStartTime = 1730678400; // UTC: Nov 4, 2024
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nTimeout = 1762214400; // UTC: Nov 4, 2025
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nOverrideRuleChangeActivationThreshold = 35; // Approx 70% of 50
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nOverrideMinerConfirmationWindow = 50;
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].bit = 10;
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nStartTime = 1730678400; // UTC: Nov 4, 2024
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nTimeout = 1762214400; // UTC: Nov 4, 2025
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nOverrideRuleChangeActivationThreshold = 35; // Approx 70% of 50
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nOverrideMinerConfirmationWindow = 50;


        // The best chain should have at least this much work
        consensus.nMinimumChainWork = uint256S("0000000000000000000000000000000000000000000000355cd0ac1503c83052"); // Block 2383567

        // By default assume that the signatures in ancestors of this block are valid. Block# 1040000
        consensus.defaultAssumeValid = uint256S("0x0000000000018d2fdcf4ac8eaac8db059584bd2840be5629562bb8599d39998c"); // Block 2383560

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0x54; // T
        pchMessageStart[1] = 0x55; // U
        pchMessageStart[2] = 0x52; // R
        pchMessageStart[3] = 0x49; // I
        nDefaultPort = 8777;
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlock(1730678400, 1, 0x207fffff, 4, 5000 * COIN); // Nov 4, 2024

        consensus.hashGenesisBlock = genesis.GetX16RHash();

        // Genesis block mined for TuriCoin mainnet
        assert(consensus.hashGenesisBlock == uint256S("0x07473470b091eda188fe47dc9dbf6e8e25e417c924892c61f88a0332c70021e9"));
        assert(genesis.hashMerkleRoot == uint256S("0x28ff00a867739a352523808d301f504bc4547699398d70faf2266a8bae5f3516"));

        // No seeds initially - will be added after network launch
        // vSeeds.emplace_back("seed1.turicoin.org", false);
        // vSeeds.emplace_back("seed2.turicoin.org", false);

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,65);  // 'tc' prefix
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,127); // 'tc' script prefix
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,193); // 'tc' private key prefix
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};

        // TuriCoin BIP44 cointype in mainnet is '777'
        nExtCoinType = 777;

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fMiningRequiresPeers = true;

        checkpointData = (CCheckpointData) {
            {
                // No checkpoints initially - will be added as network grows
            }
        };

        chainTxData = ChainTxData{
            // TuriCoin chain data - starting fresh
            1730678400, // * UNIX timestamp of genesis block
            1,          // * total number of transactions (genesis only)
            0.0         // * estimated number of transactions per second
        };

        /** RVN Start **/
        // Burn Amounts
        nIssueAssetBurnAmount = 500 * COIN;
        nReissueAssetBurnAmount = 100 * COIN;
        nIssueSubAssetBurnAmount = 100 * COIN;
        nIssueUniqueAssetBurnAmount = 5 * COIN;
        nIssueMsgChannelAssetBurnAmount = 100 * COIN;
        nIssueQualifierAssetBurnAmount = 1000 * COIN;
        nIssueSubQualifierAssetBurnAmount = 100 * COIN;
        nIssueRestrictedAssetBurnAmount = 1500 * COIN;
        nAddNullQualifierTagBurnAmount = .1 * COIN;

        // Burn Addresses - TuriCoin specific (generated deterministically)
        strIssueAssetBurnAddress = "TWKF47cLFGhv4vPh2Q74fiUNBoaAz2S4me";
        strReissueAssetBurnAddress = "TWCdUU7tsXSk93RTMHcRqx298Kp8Zu4Scf";
        strIssueSubAssetBurnAddress = "TQCxLsfs5i4ZTKXn5Pyq7Uwa9xVKcbQd1B";
        strIssueUniqueAssetBurnAddress = "TMTAAFtKBtznvpyGpQjaymgUYu4Mqa2mHi";
        strIssueMsgChannelAssetBurnAddress = "TExmNmtmzpHHTNvREeSB8kF298uz4g6AA8";
        strIssueQualifierAssetBurnAddress = "TQzsy8y8pECMbRTG1sDQ4MEMX9G9qqtTWb";
        strIssueSubQualifierAssetBurnAddress = "TM1s1wWUBQe6S7MzbY5YhSYEwcgNXw6hhN";
        strIssueRestrictedAssetBurnAddress = "TNcZxyeLTyzvLhhVHZwS3BZ1oj5vHvDj33";
        strAddNullQualifierTagBurnAddress = "TK6YsGLcF4eF7NoiQVZB4981tJzSd5SRpT";

            //Global Burn Address
        strGlobalBurnAddress = "TBVnkrrjdpNyVZueU1s7HA8mQsqmJge4af";

        // DGW Activation
        nDGWActivationBlock = 338778;

        nMaxReorganizationDepth = 144; // 144 at 25 second block timespan is +/- 60 minutes.
        nMinReorganizationPeers = 4;
        nMinReorganizationAge = 60 * 60 * 12; // 12 hours

        nAssetActivationHeight = 1000; // Asset activated block height
        nMessagingActivationBlock = 2000; // Messaging activated block height
        nRestrictedActivationBlock = 2000; // Restricted activated block height

        nKAAAWWWPOWActivationTime = 1730678400; // UTC: Nov 4, 2024
        nKAWPOWActivationTime = nKAAAWWWPOWActivationTime;
        /** RVN End **/
    }
};

/**
 * Testnet (v7)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 5040000;  //~ 4 yrs at 25 sec block time
        consensus.nBIP34Enabled = true;
        consensus.nBIP65Enabled = true; // 000000000000000004c2b624ed5d7756c508d90fd0da2c7c679febfa6c4735f0
        consensus.nBIP66Enabled = true;
        consensus.nSegwitEnabled = true;
        consensus.nCSVEnabled = true;

        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.kawpowLimit = uint256S("000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 50 * 25; // 50 blocks * 25 seconds = ~20.8 minutes
        consensus.nPowTargetSpacing = 25;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 32; // Approx 65% for testchains (65% of 50)
        consensus.nMinerConfirmationWindow = 50; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nOverrideRuleChangeActivationThreshold = 32;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nOverrideMinerConfirmationWindow = 50;
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].bit = 5;
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nStartTime = 1730678400; // UTC: Nov 4, 2024
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nTimeout = 1762214400; // UTC: Nov 4, 2025
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nOverrideRuleChangeActivationThreshold = 32;
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nOverrideMinerConfirmationWindow = 50;
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].bit = 6;  //Assets (RIP5)
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nStartTime = 1730678400; // UTC: Nov 4, 2024
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nTimeout = 1762214400; // UTC: Nov 4, 2025
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nOverrideRuleChangeActivationThreshold = 32;
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nOverrideMinerConfirmationWindow = 50;
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].bit = 8;
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nStartTime = 1730678400; // UTC: Nov 4, 2024
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nTimeout = 1762214400; // UTC: Nov 4, 2025
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nOverrideRuleChangeActivationThreshold = 32;
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nOverrideMinerConfirmationWindow = 50;
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].bit = 9;
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nStartTime = 1730678400; // UTC: Nov 4, 2024
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nTimeout = 1762214400; // UTC: Nov 4, 2025
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nOverrideRuleChangeActivationThreshold = 35; // Approx 70% of 50
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nOverrideMinerConfirmationWindow = 50;
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].bit = 10;
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nStartTime = 1730678400; // UTC: Nov 4, 2024
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nTimeout = 1762214400; // UTC: Nov 4, 2025
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nOverrideRuleChangeActivationThreshold = 35; // Approx 70% of 50
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nOverrideMinerConfirmationWindow = 50;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x000000000000000000000000000000000000000000000000000168050db560b4");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x000000006272208605c4df3b54d4d5515759105e7ffcb258e8cd8077924ffef1");


        pchMessageStart[0] = 0x54; // T
        pchMessageStart[1] = 0x55; // U
        pchMessageStart[2] = 0x52; // R
        pchMessageStart[3] = 0x54; // T (testnet)
        nDefaultPort = 18777;
        nPruneAfterHeight = 1000;

        // uint32_t nGenesisTime = 1730678400;  // Nov 4, 2024 - TuriCoin testnet genesis (unused)

        // This is used inorder to mine the genesis block. Once found, we can use the nonce and block hash found to create a valid genesis block
//        /////////////////////////////////////////////////////////////////


//        arith_uint256 test;
//        bool fNegative;
//        bool fOverflow;
//        test.SetCompact(0x1e00ffff, &fNegative, &fOverflow);
//        std::cout << "Test threshold: " << test.GetHex() << "\n\n";
//
//        int genesisNonce = 0;
//        uint256 TempHashHolding = uint256S("0x0000000000000000000000000000000000000000000000000000000000000000");
//        uint256 BestBlockHash = uint256S("0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
//        for (int i=0;i<40000000;i++) {
//            genesis = CreateGenesisBlock(nGenesisTime, i, 0x1e00ffff, 2, 5000 * COIN);
//            //genesis.hashPrevBlock = TempHashHolding;
//            // Depending on when the timestamp is on the genesis block. You will need to use GetX16RHash or GetX16RV2Hash. Replace GetHash() with these below
//            consensus.hashGenesisBlock = genesis.GetHash();
//
//            arith_uint256 BestBlockHashArith = UintToArith256(BestBlockHash);
//            if (UintToArith256(consensus.hashGenesisBlock) < BestBlockHashArith) {
//                BestBlockHash = consensus.hashGenesisBlock;
//                std::cout << BestBlockHash.GetHex() << " Nonce: " << i << "\n";
//                std::cout << "   PrevBlockHash: " << genesis.hashPrevBlock.GetHex() << "\n";
//            }
//
//            TempHashHolding = consensus.hashGenesisBlock;
//
//            if (BestBlockHashArith < test) {
//                genesisNonce = i - 1;
//                break;
//            }
//            //std::cout << consensus.hashGenesisBlock.GetHex() << "\n";
//        }
//        std::cout << "\n";
//        std::cout << "\n";
//        std::cout << "\n";
//
//        std::cout << "hashGenesisBlock to 0x" << BestBlockHash.GetHex() << std::endl;
//        std::cout << "Genesis Nonce to " << genesisNonce << std::endl;
//        std::cout << "Genesis Merkle " << genesis.hashMerkleRoot.GetHex() << std::endl;
//
//        std::cout << "\n";
//        std::cout << "\n";
//        int totalHits = 0;
//        double totalTime = 0.0;
//
//        for(int x = 0; x < 16; x++) {
//            totalHits += algoHashHits[x];
//            totalTime += algoHashTotal[x];
//            std::cout << "hash algo " << x << " hits " << algoHashHits[x] << " total " << algoHashTotal[x] << " avg " << algoHashTotal[x]/algoHashHits[x] << std::endl;
//        }
//
//        std::cout << "Totals: hash algo " <<  " hits " << totalHits << " total " << totalTime << " avg " << totalTime/totalHits << std::endl;
//
//        genesis.hashPrevBlock = TempHashHolding;
//
//        return;

//        /////////////////////////////////////////////////////////////////

        genesis = CreateGenesisBlock(1730678401, 1, 0x207fffff, 4, 5000 * COIN);
        consensus.hashGenesisBlock = genesis.GetX16RHash();

        // Genesis block mined for TuriCoin testnet
        assert(consensus.hashGenesisBlock == uint256S("0x388251282eea8eb09c9e5a3b82e455e60cf8b775a034908bc6e0ea2cf021a136"));
        assert(genesis.hashMerkleRoot == uint256S("0x28ff00a867739a352523808d301f504bc4547699398d70faf2266a8bae5f3516"));

        vFixedSeeds.clear();
        vSeeds.clear();

        // No testnet seeds initially
        // vSeeds.emplace_back("testnet-seed1.turicoin.org", false);
        // vSeeds.emplace_back("testnet-seed2.turicoin.org", false);

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);  // testnet 'tc' addresses
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);  // testnet script addresses
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);  // testnet private keys
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        // TuriCoin BIP44 cointype in testnet
        nExtCoinType = 1;

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fMiningRequiresPeers = true;

        checkpointData = (CCheckpointData) {
            {
                    { 225, uint256S("0x000003465e3e0167322eb8269ce91246bbc211e293bc5fbf6f0a0d12c1ccb363")},
                    {223408, uint256S("0x000000012a0c09dd6456ab19018cc458648dec762b04f4ddf8ef8108eae69db9")},
                    {232980, uint256S("0x000000007b16ae547fce76c3308dbeec2090cde75de74ab5dfcd6f60d13f089b")},
                    {257610, uint256S("0x000000006272208605c4df3b54d4d5515759105e7ffcb258e8cd8077924ffef1")}
            }
        };

        chainTxData = ChainTxData{
            // Update as we know more about the contents of the Raven chain
            // Stats as of 00000023b66f46d74890287a7b1157dd780c7c5fdda2b561eb96684d2b39d62e window size 43200
            1543633332, // * UNIX timestamp of last known number of transactions
            146666,     // * total number of transactions between genesis and that timestamp
                        //   (the tx=... number in the SetBestChain debug.log lines)
            0.02        // * estimated number of transactions per second after that timestamp
        };

        /** RVN Start **/
        // Burn Amounts
        nIssueAssetBurnAmount = 500 * COIN;
        nReissueAssetBurnAmount = 100 * COIN;
        nIssueSubAssetBurnAmount = 100 * COIN;
        nIssueUniqueAssetBurnAmount = 5 * COIN;
        nIssueMsgChannelAssetBurnAmount = 100 * COIN;
        nIssueQualifierAssetBurnAmount = 1000 * COIN;
        nIssueSubQualifierAssetBurnAmount = 100 * COIN;
        nIssueRestrictedAssetBurnAmount = 1500 * COIN;
        nAddNullQualifierTagBurnAmount = .1 * COIN;

        // Burn Addresses - TuriCoin testnet (generated deterministically)
        strIssueAssetBurnAddress = "n1rzM7Kaua2Dgrog9hRiyUzZ91RZiStiUT";
        strReissueAssetBurnAddress = "n1kNmTq9Xpm3kyqSUaw69iYL5XfXKQrv7q";
        strIssueSubAssetBurnAddress = "mukhdsP7k1Ns5FwmChJVRFTm7ALiLASpdo";
        strIssueUniqueAssetBurnAddress = "mrzuTFbZrCK6YmPFwi4FHYCfW6ukYZFEyz";
        strIssueMsgChannelAssetBurnAddress = "mkWWfmc2f7bb5KLQMwkqSWmD6LmNjHDB35";
        strIssueQualifierAssetBurnAddress = "mvYdG8gPUXWfDMsF9AY4N7kYUM7YXwSL7S";
        strIssueSubQualifierAssetBurnAddress = "mrZcJwDiqhxQ43myiqQD1D4RtpXmFX9FjS";
        strIssueRestrictedAssetBurnAddress = "mtAKFyMb8HKDxe7UQsG6Lx5CkvwJuaoCc3";
        strAddNullQualifierTagBurnAddress = "mpeJAG3ruMxYjKDhXnsqMueCqWqqHd1Sz5";

        // Global Burn Address
        strGlobalBurnAddress = "mh3Y3rZzJ7hH7WKdbKBmavexN5h9x71Z9d";

        // DGW Activation
        nDGWActivationBlock = 1;

        nMaxReorganizationDepth = 150; // 150 at 25 second block timespan is +/- 62.5 minutes.
        nMinReorganizationPeers = 4;
        nMinReorganizationAge = 60 * 60 * 12; // 12 hours

        nAssetActivationHeight = 6048; // Asset activated block height
        nMessagingActivationBlock = 10080; // Messaging activated block height
        nRestrictedActivationBlock = 10080; // Restricted activated block height

        nKAAAWWWPOWActivationTime = 1585159200; //Wed Mar 25 2020 18:00:00 UTC
        nKAWPOWActivationTime = nKAAAWWWPOWActivationTime;
        /** RVN End **/
    }
};

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nBIP34Enabled = true;
        consensus.nBIP65Enabled = true; // 000000000000000004c2b624ed5d7756c508d90fd0da2c7c679febfa6c4735f0
        consensus.nBIP66Enabled = true;
        consensus.nSegwitEnabled = true;
        consensus.nCSVEnabled = true;
        consensus.nSubsidyHalvingInterval = 360;  // Adjusted for 25 second blocks
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.kawpowLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 50 * 25; // 50 blocks * 25 seconds = ~20.8 minutes
        consensus.nPowTargetSpacing = 25;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 37; // 75% for testchains (75% of 50)
        consensus.nMinerConfirmationWindow = 50; // Faster than normal for regtest
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nOverrideRuleChangeActivationThreshold = 37;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nOverrideMinerConfirmationWindow = 50;
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].bit = 6;
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nOverrideRuleChangeActivationThreshold = 37;
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nOverrideMinerConfirmationWindow = 50;
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].bit = 7;  // Assets (RIP5)
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nStartTime = 0; // GMT: Sun Mar 3, 2019 5:00:00 PM
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nTimeout = 999999999999ULL; // UTC: Wed Dec 25 2019 07:00:00
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nOverrideRuleChangeActivationThreshold = 37;
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nOverrideMinerConfirmationWindow = 50;
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].bit = 8;
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nOverrideRuleChangeActivationThreshold = 37;
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nOverrideMinerConfirmationWindow = 50;
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].bit = 9;
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nOverrideRuleChangeActivationThreshold = 37;
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nOverrideMinerConfirmationWindow = 50;
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].bit = 10;
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nOverrideRuleChangeActivationThreshold = 37;
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nOverrideMinerConfirmationWindow = 50;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        pchMessageStart[0] = 0x54; // T
        pchMessageStart[1] = 0x55; // U
        pchMessageStart[2] = 0x52; // R
        pchMessageStart[3] = 0x52; // R (regtest)
        nDefaultPort = 18777;
        nPruneAfterHeight = 1000;

// This is used inorder to mine the genesis block. Once found, we can use the nonce and block hash found to create a valid genesis block
//        /////////////////////////////////////////////////////////////////
//
//
//        arith_uint256 test;
//        bool fNegative;
//        bool fOverflow;
//        test.SetCompact(0x207fffff, &fNegative, &fOverflow);
//        std::cout << "Test threshold: " << test.GetHex() << "\n\n";
//
//        int genesisNonce = 0;
//        uint256 TempHashHolding = uint256S("0x0000000000000000000000000000000000000000000000000000000000000000");
//        uint256 BestBlockHash = uint256S("0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
//        for (int i=0;i<40000000;i++) {
//            genesis = CreateGenesisBlock(1533751200, i, 0x207fffff, 2, 5000 * COIN);
//            //genesis.hashPrevBlock = TempHashHolding;
//            consensus.hashGenesisBlock = genesis.GetHash();
//
//            arith_uint256 BestBlockHashArith = UintToArith256(BestBlockHash);
//            if (UintToArith256(consensus.hashGenesisBlock) < BestBlockHashArith) {
//                BestBlockHash = consensus.hashGenesisBlock;
//                std::cout << BestBlockHash.GetHex() << " Nonce: " << i << "\n";
//                std::cout << "   PrevBlockHash: " << genesis.hashPrevBlock.GetHex() << "\n";
//            }
//
//            TempHashHolding = consensus.hashGenesisBlock;
//
//            if (BestBlockHashArith < test) {
//                genesisNonce = i - 1;
//                break;
//            }
//            //std::cout << consensus.hashGenesisBlock.GetHex() << "\n";
//        }
//        std::cout << "\n";
//        std::cout << "\n";
//        std::cout << "\n";
//
//        std::cout << "hashGenesisBlock to 0x" << BestBlockHash.GetHex() << std::endl;
//        std::cout << "Genesis Nonce to " << genesisNonce << std::endl;
//        std::cout << "Genesis Merkle " << genesis.hashMerkleRoot.GetHex() << std::endl;
//
//        std::cout << "\n";
//        std::cout << "\n";
//        int totalHits = 0;
//        double totalTime = 0.0;
//
//        for(int x = 0; x < 16; x++) {
//            totalHits += algoHashHits[x];
//            totalTime += algoHashTotal[x];
//            std::cout << "hash algo " << x << " hits " << algoHashHits[x] << " total " << algoHashTotal[x] << " avg " << algoHashTotal[x]/algoHashHits[x] << std::endl;
//        }
//
//        std::cout << "Totals: hash algo " <<  " hits " << totalHits << " total " << totalTime << " avg " << totalTime/totalHits << std::endl;
//
//        genesis.hashPrevBlock = TempHashHolding;
//
//        return;

//        /////////////////////////////////////////////////////////////////


        genesis = CreateGenesisBlock(1524179366, 1, 0x207fffff, 4, 5000 * COIN);
        consensus.hashGenesisBlock = genesis.GetX16RHash();

        assert(consensus.hashGenesisBlock == uint256S("0x0b2c703dc93bb63a36c4e33b85be4855ddbca2ac951a7a0a29b8de0408200a3c "));
        assert(genesis.hashMerkleRoot == uint256S("0x28ff00a867739a352523808d301f504bc4547699398d70faf2266a8bae5f3516"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;

        checkpointData = (CCheckpointData) {
            {
            }
        };

        chainTxData = ChainTxData{
            0,
            0,
            0
        };

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        // Raven BIP44 cointype in regtest
        nExtCoinType = 1;

        /** RVN Start **/
        // Burn Amounts
        nIssueAssetBurnAmount = 500 * COIN;
        nReissueAssetBurnAmount = 100 * COIN;
        nIssueSubAssetBurnAmount = 100 * COIN;
        nIssueUniqueAssetBurnAmount = 5 * COIN;
        nIssueMsgChannelAssetBurnAmount = 100 * COIN;
        nIssueQualifierAssetBurnAmount = 1000 * COIN;
        nIssueSubQualifierAssetBurnAmount = 100 * COIN;
        nIssueRestrictedAssetBurnAmount = 1500 * COIN;
        nAddNullQualifierTagBurnAmount = .1 * COIN;

        // Burn Addresses
        strIssueAssetBurnAddress = "n1issueAssetXXXXXXXXXXXXXXXXWdnemQ";
        strReissueAssetBurnAddress = "n1ReissueAssetXXXXXXXXXXXXXXWG9NLd";
        strIssueSubAssetBurnAddress = "n1issueSubAssetXXXXXXXXXXXXXbNiH6v";
        strIssueUniqueAssetBurnAddress = "n1issueUniqueAssetXXXXXXXXXXS4695i";
        strIssueMsgChannelAssetBurnAddress = "n1issueMsgChanneLAssetXXXXXXT2PBdD";
        strIssueQualifierAssetBurnAddress = "n1issueQuaLifierXXXXXXXXXXXXUysLTj";
        strIssueSubQualifierAssetBurnAddress = "n1issueSubQuaLifierXXXXXXXXXYffPLh";
        strIssueRestrictedAssetBurnAddress = "n1issueRestrictedXXXXXXXXXXXXZVT9V";
        strAddNullQualifierTagBurnAddress = "n1addTagBurnXXXXXXXXXXXXXXXXX5oLMH";

        // Global Burn Address
        strGlobalBurnAddress = "n1BurnXXXXXXXXXXXXXXXXXXXXXXU1qejP";

        // DGW Activation
        nDGWActivationBlock = 200;

        nMaxReorganizationDepth = 60; // 60 at 1 minute block timespan is +/- 60 minutes.
        nMinReorganizationPeers = 4;
        nMinReorganizationAge = 60 * 60 * 12; // 12 hours

        nAssetActivationHeight = 0; // Asset activated block height
        nMessagingActivationBlock = 0; // Messaging activated block height
        nRestrictedActivationBlock = 0; // Restricted activated block height

        // TODO, we need to figure out what to do with this for regtest. This effects the unit tests
        // For now we can use a timestamp very far away
        // If you are looking to test the kawpow hashing function in regtest. You will need to change this number
        nKAAAWWWPOWActivationTime = 3582830167;
        nKAWPOWActivationTime = nKAAAWWWPOWActivationTime;
        /** RVN End **/
    }
};

static std::unique_ptr<CChainParams> globalChainParams;

const CChainParams &GetParams() {
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

void SelectParams(const std::string& network, bool fForceBlockNetwork)
{
    SelectBaseParams(network);
    if (fForceBlockNetwork) {
        bNetwork.SetNetwork(network);
    }
    globalChainParams = CreateChainParams(network);
}

void UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    globalChainParams->UpdateVersionBitsParameters(d, nStartTime, nTimeout);
}

void TurnOffSegwit(){
	globalChainParams->TurnOffSegwit();
}

void TurnOffCSV() {
	globalChainParams->TurnOffCSV();
}

void TurnOffBIP34() {
	globalChainParams->TurnOffBIP34();
}

void TurnOffBIP65() {
	globalChainParams->TurnOffBIP65();
}

void TurnOffBIP66() {
	globalChainParams->TurnOffBIP66();
}
