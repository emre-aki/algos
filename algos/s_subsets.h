/*
 *  s_subsets.h
 *  algos
 *
 *  Created by Emre Akı on 2023-02-19.
 *
 *  SYNOPSIS:
 *      A module that helps build a powerset (a set of all subsets) from a given
 *      set of numbers.
 */

#ifndef s_subsets_h

#define s_subsets_h
#define s_subsets_h_S_Subsets S_Subsets
#define s_subsets_h_S_TestSubsets S_TestSubsets

int** S_Subsets (int* nums, int nNums, int* nPowerset, int** nSubsets);
void S_TestSubsets (int* nums, int nNums);

#endif
