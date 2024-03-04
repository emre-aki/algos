/*
 *  sr_sort.c
 *  algos
 *
 *  Created by Emre Akı on 2024-02-24.
 *
 *  SYNOPSIS:
 *      A module that houses different sorting algorithms
 */

#include "sr_sort.h"

static void SR_Slice (int* in, int* out, size_t start, size_t size)
{
    const size_t stop = start + size;

    for (size_t r = start, w = 0; r < stop; ++r) *(out + w++) = *(in + r);
}

void SR_Mergesort (int* in, int* out, size_t size)
{
    if (size == 1)
    {
        *out = *in;

        return;
    }

    const size_t leftsize = size >> 1, rightsize = size - leftsize;
    int left[leftsize], right[rightsize];
    int leftsorted[leftsize], rightsorted[rightsize];

    SR_Slice(in, left, 0, leftsize);
    SR_Slice(in, right, leftsize, rightsize);
    SR_Mergesort(left, leftsorted, leftsize);
    SR_Mergesort(right, rightsorted, rightsize);

    size_t i = 0, j = 0, k = 0;

    while (i < leftsize && j < rightsize)
    {
        if (*(leftsorted + i) < *(rightsorted + j))
            *(out + k++) = *(leftsorted + i++);
        else
            *(out + k++) = *(rightsorted + j++);
    }

    while (i < leftsize) *(out + k++) = *(leftsorted + i++);
    while (j < rightsize) *(out + k++) = *(rightsorted + j++);
}
