/*
 *  s_palind.h
 *  algos
 *
 *  Created by Emre Akı on 2023-12-13.
 *
 *  SYNOPSIS:
 *      A module that implements Manacher's algorithm, used for palindrome
 *      detection and for finding the largest palindromic substring in a given
 *      input string
 */

#include <stddef.h>

#ifndef s_palind_h

#define s_palind_h
#define s_palind_h_S_Manachers S_Manachers

size_t S_Manachers (char* str, size_t strlen, size_t* start);

#endif
