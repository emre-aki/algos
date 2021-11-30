/*
 *  m_fixed.h
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

#ifndef m_fixed_h

#define m_fixed_h_fixed_t fixed_t
#define m_fixed_h_int64_t int64_t
#define m_fixed_h_M_ToFixed M_ToFixed
#define m_fixed_h_M_ToDouble M_ToDouble
#define m_fixed_h_M_Mul M_Mul
#define m_fixed_h_M_Div M_Div
#define m_fixed_h_M_Sign M_Sign
#define m_fixed_h_M_Floor M_Floor
#define m_fixed_h_M_Ceil M_Ceil
#define m_fixed_h_M_Round M_Round

typedef int fixed_t;
typedef long long int64_t;

fixed_t M_ToFixed (double num);
double M_ToDouble (fixed_t num);
fixed_t M_Mul (fixed_t a, fixed_t b);
fixed_t M_Div (fixed_t numer, fixed_t denom);
int M_Sign (fixed_t num);
fixed_t M_Floor (fixed_t num);
fixed_t M_Ceil (fixed_t num);
fixed_t M_Round (fixed_t num);

#endif
