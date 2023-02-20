/*
 *  s_subsets.c
 *  algos
 *
 *  Created by Emre Akı on 2023-02-19.
 *
 *  SYNOPSIS:
 *      A module that helps build a powerset (a set of all subsets) from a given
 *      set of numbers.
 */

#include <stdio.h>

#include "e_malloc.h"
#include "s_subsets.h"

static void S_PrintSubset (int* subset, int nSubset)
{
    if (!nSubset)
    {
        printf("[]\n");
        return;
    }
    printf("[");
    int stop = nSubset - 1;
    for (int i = 0; i < stop; ++i) printf("%d, ", *(subset + i));
    printf("%d]\n", *(subset + stop));
}

static int* S_GetSubset (int* nums, int* nSubset, short n)
{
    int nSubset_ = 0;
    for (char i = 0; i < 10; ++i) if (n & (1 << i)) ++nSubset_;
    int* subset = (int*) E_Malloc(sizeof(int) * nSubset_, S_GetSubset);
    char next = 0;
    for (char i = 0; i < 10; ++i)
    {
        if (n & (1 << i))
        {
            *(subset + next) = *(nums + i);
            ++next;
        }
    }
    *nSubset = nSubset_;
    return subset;
}

int** S_Subsets (int* nums, int nNums, int* nPowerset, int** nSubsets)
{
    short nPowerset_ = 1 << nNums;
    int* subsetSizes = (int*) E_Malloc(sizeof(int) * nPowerset_, S_Subsets);
    int** subsets = (int**) E_Malloc(sizeof(int*) * nPowerset_, S_Subsets);
    for (short i = 0; i < nPowerset_; ++i)
    {
        int nSubset;
        int* subset = S_GetSubset(nums, &nSubset, i);
        *(subsetSizes + i) = nSubset;
        *(subsets + i) = subset;
    }
    *nPowerset = nPowerset_;
    *nSubsets = subsetSizes;
    return subsets;
}

void S_TestSubsets (int* nums, int nNums)
{
    int nPowerset;
    int* nSubsets;
    int** subsets = S_Subsets(nums, nNums, &nPowerset, &nSubsets);
    for (short i = 0; i < nPowerset; ++i)
        S_PrintSubset(*(subsets + i), *(nSubsets + i));
    for (short i = 0; i < nPowerset; ++i) E_Free(*(subsets + i));
    E_Free(subsets);
    E_Free(nSubsets);
    E_Dump();
}
