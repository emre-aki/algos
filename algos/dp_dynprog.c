/*
 *  dp_dynprog.c
 *  algos
 *
 *  Created by Emre Akı on 2023-10-06.
 *
 *  SYNOPSIS:
 *      A module that houses solutions for some dynamic programming problems at
 *      LeetCode.
 *
 *      #1335: https://leetcode.com/problems/minimum-difficulty-of-a-job-schedule
 */

#include "ar_arr2d.h"
#include "dp_dynprog.h"

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

int DP_MinDifficulty (int* jobs, int nJobs, int days)
{
    size_t memo[nJobs * days];
    arr2d_t dp = AR_Init(memo, days, nJobs, -1);

    size_t max = 0;
    for (size_t i = 0; i < nJobs; ++i)
    {
        max = MAX(max, *(jobs + i));
        AR_Set(&dp, 0, i, max);
    }

    for (size_t cut = 1; cut < days; ++cut)
    {
        for (size_t j = cut; j < nJobs; ++j)
        {
            size_t currentDayMax = *(jobs + j);

            for (size_t k = j; k >= cut; --k)
            {
                size_t minSoFar = AR_Get(&dp, cut, j);
                size_t minFromPreviousCut = AR_Get(&dp, cut - 1, k - 1);
                size_t previousJob = *(jobs + k - 1);

                AR_Set(&dp, cut, j,
                       MIN(minSoFar, currentDayMax + minFromPreviousCut));

                currentDayMax = MAX(currentDayMax, previousJob);
            }
        }
    }

    return AR_Get(&dp, days - 1, nJobs - 1);
}
