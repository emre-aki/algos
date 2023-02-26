/*
 *  s_substring.c
 *  algos
 *
 *  Created by Emre Akı on 2023-02-26.
 *
 *  SYNOPSIS:
 *      A module that houses solutions for LeetCode problems #1371 and #3.
 *
 *         #1371: https://leetcode.com/problems/longest-substring-without-repeating-characters
 *         #3: https://leetcode.com/problems/find-the-longest-substring-containing-vowels-in-even-counts
 */

#include <stdio.h>

#include "t_typedef.h"

static int S_StrLen (char* str)
{
    int len = 0;
    while (*(str + len) != '\0') ++len;
    return len;
}

static void S_PrintStr (char* str, int start, int end)
{
    for (int i = start; i < end; ++i) printf("%c", *(str + i));
    printf("\n");
}

static void S_FlipBit (byte *b, byte i)
{
    byte front = *b >> i, rear = *b & ((1 << i) - 1);
    front ^= 1;
    front <<= i;
    *b = front | rear;
}

byte S_VowelIndex (char vowel)
{
    switch (vowel)
    {
        case 'a': return 0;
        case 'e': return 1;
        case 'i': return 2;
        case 'o': return 3;
        case 'u': return 4;
        default: return -1;
    }
}

void S_LongestSubstringWithEvenVowels (char* str)
{
    int len = S_StrLen(str);
    byte evenMask = 0;
    char memo[32] = { [0 ... 31] = -1 };
    int currlen = 0, maxlen = 0, start = 0;
    for (int i = 0; i < len; ++i)
    {
        char c = *(str + i);
        char vowelIndex = S_VowelIndex(c);
        if (vowelIndex >= 0) S_FlipBit(&evenMask, vowelIndex);
        if (evenMask && *(memo + evenMask) < 0) *(memo + evenMask) = i;
        currlen = i - *(memo + evenMask);
        if (currlen >= maxlen)
        {
            start = *(memo + evenMask) + 1;
            maxlen = currlen;
        }
    }
    S_PrintStr(str, start, start + maxlen);
}

void S_LongestSubstringWithNonRepeatingChars (char* str)
{
    int len = S_StrLen(str);
    int memo[128] = { [0 ... 127] = -1 };
    int currlen = 0, maxlen = 0, start = 0, lastrecurring = -1;
    for (int i = 0; i < len; ++i)
    {
        char c = *(str + i);
        if (*(memo + c) > lastrecurring) lastrecurring = *(memo + c);
        currlen = i - lastrecurring;
        *(memo + c) = i;
        if (currlen >= maxlen)
        {
            start = lastrecurring + 1;
            maxlen = currlen;
        }
    }
    S_PrintStr(str, start, start + maxlen);
}
