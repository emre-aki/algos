/*
 *  m_fixed.c
 *  algos
 *
 *  Created by Emre Akı on 2021-11-27.
 *
 *  SYNOPSIS:
 *      A module to carry out fixed-point arithmetic, as somewhat a faster
 *      alternative to floating-point.
 *
 *      The numbers are 32-bit integers in the format:
 *      16.16
 */

#include "m_fixed.h"

static const unsigned short PRECISION = 16;
static const unsigned int SCALE = 1 << PRECISION;
static const fixed_t SIGN_MASK = 0x80000000;
static const fixed_t INT_MASK = 0xffff0000;
static const fixed_t HALF = (fixed_t) SCALE >> 1;

fixed_t M_ToFixed (double num)
{
    return num * (fixed_t) SCALE;
}

double M_ToDouble (fixed_t num)
{
    return (double) num / SCALE;
}

fixed_t M_Mul (fixed_t a, fixed_t b)
{
    // casting `a` here to extend the product to a 64-bit integer, which,
    // otherwise won't fit in a 32-bit `int` format
    return (fixed_t) (((int64_t) a * b) >> PRECISION);
}

fixed_t M_Div (fixed_t numer, fixed_t denom)
{
    // casting the numerator to 64-bits before scaling it by `SCALE` to keep it
    // in range
    return (fixed_t) (((int64_t) numer << PRECISION) / denom);
}

int M_Sign (fixed_t num)
{
    return (num >> 31) + (num > 0);
}

fixed_t M_Floor (fixed_t num)
{
    return num & INT_MASK;
}

fixed_t M_Ceil (fixed_t num)
{
    return (num & INT_MASK) + SCALE;
}

fixed_t M_Round (fixed_t num)
{
    int sign = M_Sign(num);
    if (!sign) return 0;
    int pasthalfway = num - M_Floor(num) - HALF;
    if (sign > 0) return pasthalfway >= 0 ? M_Ceil(num) : M_Floor(num);
    return pasthalfway > 0 ? M_Ceil(num) : M_Floor(num);
}

fixed_t M_Abs (fixed_t num)
{
    int signMask = num >> 31;
    return (num ^ signMask) + (1 & signMask);
}
