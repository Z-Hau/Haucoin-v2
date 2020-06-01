// Copyright (c) 2016-2017 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_CONSENSUS_VERSIONBITS
#define BITCOIN_CONSENSUS_VERSIONBITS

#include <chain.h>
#include <map>

/** What block version to use for new blocks (pre versionbits) */
static  int32_t VERSIONBITS_LAST_OLD_BLOCK_VERSION = 4;
/** What bits to set in version for versionbits blocks */
static  int32_t VERSIONBITS_TOP_BITS = 0x20000000UL;
/** What bitmask determines whether versionbits is in use */
static  int32_t VERSIONBITS_TOP_MASK = 0xE0000000UL;
/** Total bits available for versionbits */
static  int32_t VERSIONBITS_NUM_BITS = 29;

enum ThresholdState {
    THRESHOLD_DEFINED,
    THRESHOLD_STARTED,
    THRESHOLD_LOCKED_IN,
    THRESHOLD_ACTIVE,
    THRESHOLD_FAILED,
};

// A map that gives the state for blocks whose height is a multiple of Period().
// The map is indexed by the block's parent, however, so all keys in the map
// will either be nullptr or a block with (height + 1) % Period() == 0.
typedef std::map< CBlockIndex*, ThresholdState> ThresholdConditionCache;

struct VBDeploymentInfo {
    /** Deployment name */
     char *name;
    /** Whether GBT clients can safely ignore this rule in simplified usage */
    bool gbt_force;
};

struct BIP9Stats {
    int period;
    int threshold;
    int elapsed;
    int count;
    bool possible;
};

extern  struct VBDeploymentInfo VersionBitsDeploymentInfo[];

/**
 * Abstract class that implements BIP9-style threshold logic, and caches results.
 */
class AbstractThresholdConditionChecker {
protected:
    virtual bool Condition( CBlockIndex* pindex, Consensus::Params& params)  =0;
    virtual int64_t BeginTime(Consensus::Params& params)  =0;
    virtual int64_t EndTime(Consensus::Params& params)  =0;
    virtual int Period(Consensus::Params& params)  =0;
    virtual int Threshold(Consensus::Params& params)  =0;

public:
    BIP9Stats GetStateStatisticsFor( CBlockIndex* pindex, Consensus::Params& params) ;
    // Note that the functions below take a pindexPrev as input: they compute information for block B based on its parent.
    ThresholdState GetStateFor( CBlockIndex* pindexPrev, Consensus::Params& params, ThresholdConditionCache& cache) ;
    int GetStateSinceHeightFor( CBlockIndex* pindexPrev, Consensus::Params& params, ThresholdConditionCache& cache) ;
};

struct VersionBitsCache
{
    ThresholdConditionCache caches[Consensus::MAX_VERSION_BITS_DEPLOYMENTS];

    void Clear();
};

ThresholdState VersionBitsState( CBlockIndex* pindexPrev, Consensus::Params& params, Consensus::DeploymentPos pos, VersionBitsCache& cache);
BIP9Stats VersionBitsStatistics( CBlockIndex* pindexPrev, Consensus::Params& params, Consensus::DeploymentPos pos);
int VersionBitsStateSinceHeight( CBlockIndex* pindexPrev, Consensus::Params& params, Consensus::DeploymentPos pos, VersionBitsCache& cache);
uint32_t VersionBitsMask(Consensus::Params& params, Consensus::DeploymentPos pos);

#endif
