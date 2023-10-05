/*
 *  s_substring.c
 *  algos
 *
 *  Created by Emre Akı on 2023-02-26.
 *
 *  SYNOPSIS:
 *      A module that houses solutions for LeetCode problems #1371, #3,
 *      and #1143.
 *
 *         #1371: https://leetcode.com/problems/longest-substring-without-repeating-characters
 *         #3: https://leetcode.com/problems/find-the-longest-substring-containing-vowels-in-even-counts
 *         #1143: https://leetcode.com/problems/longest-common-subsequence
 */

#include <stdio.h>

#include "s_substring.h"
#include "t_typedef.h"
#include "ar_arr2d.h"

#define MAX(a, b) (((a) >= (b)) ? (a) : (b))

static char S_CharAt (char* str, size_t i)
{
    return *(str + i);
}

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

byte S_VowelIndex (char vowel)
{
    switch (vowel)
    {
        case 'a': return 0x2;
        case 'e': return 0x4;
        case 'i': return 0x8;
        case 'o': return 0x10;
        case 'u': return 0x20;
        default: return 0;
    }
}

void S_LongestSubstringWithEvenVowels (char* str)
{
    int len = S_StrLen(str);
    byte evenMask = 0;
    char memo[64] = { [0 ... 63] = -1 };
    int currlen = 0, maxlen = 0, start = 0;
    for (int i = 0; i < len; ++i)
    {
        char c = *(str + i);
        evenMask ^= S_VowelIndex(c);
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

void S_LongestCommonSubsequence (char* str0, char* str1)
{
    size_t len0 = 0, len1 = 0;
    while (*(str0 + len0) != '\0') ++len0;
    while (*(str1 + len1) != '\0') ++len1;
    /* swap the strings if the second happens to be longer than first, as the
     * longer should always be spanned along the columns while the shorter along
     * the rows
     */
    if (len0 < len1)
    {
        char* caux = str0; str0 = str1; str1 = caux;
        size_t laux = len0; len0 = len1; len1 = laux;
    }
    size_t rows = len1 + 1, cols = len0 + 1, size = rows * cols;
    size_t memo[size];
    arr2d_t arr2d = AR_Init(memo, rows, cols, 0);
    size_t r = 1, c;
    for (; r < rows; ++r)
    {
        size_t j = r - 1;
        char c1 = S_CharAt(str1, j);
        for (c = 1; c < cols; ++c)
        {
            size_t i = c - 1;
            char c0 = S_CharAt(str0, i);
            if (c0 == c1) AR_Set(&arr2d, r, c, AR_Get(&arr2d, j, i) + 1);
            else AR_Set(&arr2d, r, c, MAX(AR_Get(&arr2d, r, i),
                                          AR_Get(&arr2d, j, c)));
        }
    }
    size_t reslen = *(memo + size - 1);
    size_t nextchar = reslen;
    char res[reslen + 1];
    *(res + (nextchar--)) = '\0';
    r = rows - 1; c = cols - 1;
    while (r && c)
    {
        size_t j = r - 1, i = c - 1;
        if (AR_Get(&arr2d, j, i) + 1 == AR_Get(&arr2d, r, c))
        {
            *(res + (nextchar--)) = S_CharAt(str1, j);
            --r; --c;
        }
        else if (AR_Get(&arr2d, r, i) <= AR_Get(&arr2d, j, c)) --r;
        else --c;
    }
    printf("\"%s\"\n", res);
}
