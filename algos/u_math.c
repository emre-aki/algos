/*
 *  u_math.c
 *  algos
 *
 *  Created by Emre Akı on 2021-10-25.
 *
 *  SYNOPSIS:
 *      Various math utils.
 */

#include <math.h>

#include "u_math.h"

double U_ToFixed (double number, int fractiondigits)
{
    double orderofmag = pow(10, fractiondigits);
    return round((number * orderofmag)) / orderofmag;
}
