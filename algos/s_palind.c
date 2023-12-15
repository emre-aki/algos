/*
 *  s_palind.c
 *  algos
 *
 *  Created by Emre Akı on 2023-12-13.
 *
 *  SYNOPSIS:
 *      A module that implements Manacher's algorithm, used for palindrome
 *      detection and for finding the largest palindromic substring in a given
 *      input string
 */

#include <stdio.h>

#include "s_palind.h"

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

static void S_Extend (char* og, size_t oglen, char* extended)
{
    size_t offset = 1;
    *extended = '%';

    for (size_t i = 0 ; i < oglen; ++i)
    {
        *(extended + offset++) = *(og + i);
        *(extended + offset++) = '%';
    }
    *(extended + offset) = '\0';
}

size_t S_Manachers (char* str, size_t strlen, size_t* start)
{
    const size_t extendedlen = (strlen + 1) << 1, end = extendedlen - 2;
    char extended[extendedlen];
    size_t p[end]; // the array that stores palindrome radii
    size_t center = 0, radius = 0, max_center = center, max_radius = radius;
    // extend the input string by inserting an arbitrary symbol after each
    // character to be able to conveniently handle strings of both even and odd
    // lengths
    S_Extend(str, strlen, extended);
    /* iterate over the extended string, till the last inserted symbol */
    for (size_t i = center; i < end; ++i)
    {
        // right edge of the most recently encountered sub-palindrome
        const size_t right = center + radius;
        // the mirror image of the current character we're inspecting with
        // respect to the center of the most recently encountered sub-palindrome
        int mirror = (center << 1) - i;
        // the radius of the sub-palindrome centered at the current character
        // we're inspecting, if it forms a palindrome
        size_t* curr_radius = p + i;
        // if the current character we're inspecting is in the second half of
        // the most recently encountered sub-palindrome, copy the radius from
        // its mirror in the first half. clamp the radius mirrored from the
        // first half if it happens to extend beyond the second half of the
        // palindrome
        if (i <= right) *curr_radius = MIN(*(p + mirror), right - i);
        // if the current character we're inspecting is outside the most
        // recently encountered palindrome, simply initialize its radius to 0
        else *curr_radius = 0;
        // try to extend the sub-palindrome centered at the current character
        // we're inspecting from the left
        int extend_left = i - *curr_radius - 1;
        // try to extend the sub-palindrome centered at the current character
        // we're inspecting from the right
        size_t extend_right = i + *curr_radius + 1;
        /* extend the current sub-palindrome either until the entire string has
         * been exhausted or until the characters from the left and the right no
         * longer match
         */
        while (extend_left >= 0 && extend_right <= end &&
               *(extended + extend_left--) == *(extended + extend_right++))
            (*curr_radius)++;
        /* update the center and radius if a new sub-palindrome has been found
         */
        if (i + *curr_radius > right)
        {
            center = i;
            radius = *curr_radius;
        }
        /* update the maxima if a larger palindrome has been found */
        if (radius > max_radius)
        {
            max_center = i;
            max_radius = radius;
        }
    }
    // calculate the start of the largest palindrome found
    *start = (max_center - max_radius) >> 1;

    return max_radius;
}
