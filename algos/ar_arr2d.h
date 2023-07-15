/*
 *  ar_arr2d.h
 *  algos
 *
 *  Created by Emre Akı on 2023-07-15.
 *
 *  SYNOPSIS:
 *      2-D array of unsigned integers
 */

#ifndef ar_arr2d_h

#include <stdlib.h>

#define ar_arr2d_h
#define ar_arr2d_h_arr2d_t arr2d_t
#define ar_arr2d_h_AR_Init AR_Init
#define ar_arr2d_h_AR_Get AR_Get
#define ar_arr2d_h_AR_Set AR_Set
#define ar_arr2d_h_AR_Print AR_Print

typedef struct {
    size_t* data;
    size_t rows;
    size_t cols;
} arr2d_t;

arr2d_t AR_Init (size_t* data, size_t rows, size_t cols, size_t val);
size_t AR_Get (arr2d_t* arr, size_t row, size_t col);
void AR_Set (arr2d_t* arr, size_t row, size_t col, size_t val);
void AR_Print (arr2d_t* arr);

#endif
