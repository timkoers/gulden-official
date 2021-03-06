// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
//
// File contains modifications by: The Gulden developers
// All modifications:
// Copyright (c) 2016-2019 The Gulden developers
// Authored by: Malcolm MacLeod (mmacleod@gmx.com)
// Distributed under the GULDEN software license, see the accompanying
// file COPYING

#include "pow.h"

#include "arith_uint256.h"
#include "chain.h"
#include "primitives/block.h"
#include "uint256.h"
#include "crypto/hash/sigma/sigma.h"
#include "random.h"
#include <thread>

/*GULDEN - We  use our own calculation from elsewhere in the source
unsigned int GetNextWorkRequired(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params)
{
    assert(pindexLast != NULL);
    unsigned int nProofOfWorkLimit = UintToArith256(params.powLimit).GetCompact();

    // Only change once per difficulty adjustment interval
    if ((pindexLast->nHeight+1) % params.DifficultyAdjustmentInterval() != 0)
    {
        if (params.fPowAllowMinDifficultyBlocks)
        {
            // Special difficulty rule for testnet:
            // If the new block's timestamp is more than 2* 10 minutes
            // then allow mining of a min-difficulty block.
            if (pblock->GetBlockTime() > pindexLast->GetBlockTime() + params.nPowTargetSpacing*2)
                return nProofOfWorkLimit;
            else
            {
                // Return the last non-special-min-difficulty-rules-block
                const CBlockIndex* pindex = pindexLast;
                while (pindex->pprev && pindex->nHeight % params.DifficultyAdjustmentInterval() != 0 && pindex->nBits == nProofOfWorkLimit)
                    pindex = pindex->pprev;
                return pindex->nBits;
            }
        }
        return pindexLast->nBits;
    }

    // Go back by what we want to be 14 days worth of blocks
    int nHeightFirst = pindexLast->nHeight - (params.DifficultyAdjustmentInterval()-1);
    assert(nHeightFirst >= 0);
    const CBlockIndex* pindexFirst = pindexLast->GetAncestor(nHeightFirst);
    assert(pindexFirst);

    return CalculateNextWorkRequired(pindexLast, pindexFirst->GetBlockTime(), params);
}

unsigned int CalculateNextWorkRequired(const CBlockIndex* pindexLast, int64_t nFirstBlockTime, const Consensus::Params& params)
{
    if (params.fPowNoRetargeting)
        return pindexLast->nBits;

    // Limit adjustment step
    int64_t nActualTimespan = pindexLast->GetBlockTime() - nFirstBlockTime;
    if (nActualTimespan < params.nPowTargetTimespan/4)
        nActualTimespan = params.nPowTargetTimespan/4;
    if (nActualTimespan > params.nPowTargetTimespan*4)
        nActualTimespan = params.nPowTargetTimespan*4;

    // Retarget
    const arith_uint256 bnPowLimit = UintToArith256(params.powLimit);
    arith_uint256 bnNew;
    bnNew.SetCompact(pindexLast->nBits);
    bnNew *= nActualTimespan;
    bnNew /= params.nPowTargetTimespan;

    if (bnNew > bnPowLimit)
        bnNew = bnPowLimit;

    return bnNew.GetCompact();
}
*/

bool CheckProofOfWork(const CBlock* block, const Consensus::Params& params)
{    
    bool fNegative;
    bool fOverflow;
    arith_uint256 bnTarget;

    bnTarget.SetCompact(block->nBits, &fNegative, &fOverflow);

    if (block->nTime > 1571320800)
    {
        uint256 newProofOfWorkLimit = uint256S("0x003fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        // Check range
        if (fNegative || bnTarget == 0 || fOverflow || bnTarget > UintToArith256(newProofOfWorkLimit))
            return false;
    }
    else
    {
        // Check range
        if (fNegative || bnTarget == 0 || fOverflow || bnTarget > UintToArith256(params.powLimit))
            return false;
    }

    //fixme: (SIGMA) - Post activation we can simplify this.
    // Check proof of work matches claimed amount
    if (block->nTime > defaultSigmaSettings.activationDate)
    {
        static sigma_verify_context verify(defaultSigmaSettings,std::min(defaultSigmaSettings.numVerifyThreads, (uint64_t)std::thread::hardware_concurrency()));
        static CCriticalSection csPOW;
        LOCK(csPOW);
        
        //fixme: (SIGMA) - Detect faster machines and disable this optimisation for them, this will further increase network security.
        // We speed up verification by doing a half verify 40% of the time instead of a full verify
        // As a half verify has a 50% chance of detecting a 'half valid' hash an attacker has only a 20% chance of a node accepting his header without banning him
        // This should provide a ~20% speed up for slow machines
        int verifyLevel = GetRand(100);
        if (verifyLevel < 20)
        {
            return verify.verifyHeader<1>(*block);
        }
        else if (verifyLevel < 40)
        {
            return verify.verifyHeader<2>(*block);
        }
        return verify.verifyHeader<0>(*block);
    }
    else
    {
        if (UintToArith256(block->GetPoWHash()) > bnTarget)
            return false;
    }

    return true;
}
