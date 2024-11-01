/*
 *  s_buffer.h
 *  algos
 *
 *  Created by Emre Akı on 2024-09-17.
 *
 *  SYNOPSIS:
 *      A rather unique implementation of the ubiquitous S-Buffer, once a very
 *      popular alternative to Z-Buffering for solving the hidden surface
 *      removal problem in software rendering.
 *
 *      The implementation uses a binary tree instead of a linked list to cut
 *      down on the search time.
 *
 *      Original FAQ by Paul Nettle:
 *      https://www.gamedev.net/articles/programming/graphics/s-buffer-faq-r668/
 */

#ifndef s_buffer_h

#include "t_typedef.h"

#define s_buffer_h
#define s_buffer_h_span_t span_t
#define s_buffer_h_sbuffer_t sbuffer_t
#define s_buffer_h_SB_Init SB_Init
#define s_buffer_h_SB_Push SB_Push
#define s_buffer_h_SB_Dump SB_Dump
#define s_buffer_h_SB_Print SB_Print
#define s_buffer_h_SB_Destroy SB_Destroy

typedef struct span {
    struct span *prev, *next;
    int          start, size;
    byte         id;
} span_t;

typedef struct {
    span_t* root;
    int     size;
    size_t  max_depth;
} sbuffer_t;

sbuffer_t* SB_Init (int size, size_t max_depth);
int SB_Push (sbuffer_t* sbuffer, int start, int size, byte id);
void SB_Dump (sbuffer_t* sbuffer);
void SB_Print (sbuffer_t* sbuffer);
void SB_Destroy (sbuffer_t* sbuffer);

#endif
