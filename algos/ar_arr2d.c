/*
 *  ar_arr2d.c
 *  algos
 *
 *  Created by Emre Akı on 2023-07-15.
 *
 *  SYNOPSIS:
 *      2-D array of unsigned integers
 */

#include <stdio.h>

#include "ar_arr2d.h"

arr2d_t AR_Init (size_t* data, size_t rows, size_t cols, size_t val)
{
    arr2d_t arr = { data, rows, cols };
    size_t size = rows * cols;
    for (size_t i = 0; i < size; ++i) *(data + i) = val;

    return arr;
}

size_t AR_Get (arr2d_t* arr, size_t row, size_t col)
{
    return *(arr->data + arr->cols * row + col);
}

void AR_Set (arr2d_t* arr, size_t row, size_t col, size_t val)
{
    *(arr->data + arr->cols * row + col) = val;
}

void AR_Print (arr2d_t* arr)
{
    size_t* data = arr->data;
    size_t rows = arr->rows;
    size_t cols = arr->cols;
    size_t lastcol = cols - 1;
    for (size_t r = 0; r < rows; ++r)
    {
        size_t c = 0;
        printf("[%d]\t", r);
        for (; c < lastcol; ++c) printf(" %d\t", AR_Get(arr, r, c));
        printf("%d\n", AR_Get(arr, r, c));
    }
}
