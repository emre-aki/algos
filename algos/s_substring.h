/*
 *  s_substring.h
 *  algos
 *
 *  Created by Emre Akı on 2023-02-26.
 *
 *  SYNOPSIS:
 *      A module that houses solutions for some problems around substrings over
 *      at LeetCode.
 *
 *         #1371: https://leetcode.com/problems/longest-substring-without-repeating-characters
 *         #3: https://leetcode.com/problems/find-the-longest-substring-containing-vowels-in-even-counts
 *         #1143: https://leetcode.com/problems/longest-common-subsequence
 *         #5: https://leetcode.com/problems/longest-palindromic-substring
 */

#ifndef s_substring_h

#define s_substring_h
#define s_substring_h_S_LongestSubstringWithEvenVowels S_LongestSubstringWithEvenVowels
#define s_substring_h_S_LongestSubstringWithNonRepeatingChars S_LongestSubstringWithNonRepeatingChars
#define s_substring_h_S_LongestCommonSubsequence S_LongestCommonSubsequence
#define s_substring_h_S_LongestPalindromicSubstring S_LongestPalindromicSubstring

void S_LongestSubstringWithEvenVowels (char* str);
void S_LongestSubstringWithNonRepeatingChars (char* str);
void S_LongestCommonSubsequence (char* str0, char* str1);
void S_LongestPalindromicSubstring (char* str);

#endif
