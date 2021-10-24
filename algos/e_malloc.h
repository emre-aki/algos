/*
 *  m_emalloc.h
 *  algos
 *
 *  Created by Emre Akı on 2021-03-22.
 *
 *  SYNOPSIS:
 *      My very own custom memory allocator. (The prefix 'E' stands for 'Emre'–
 *      I know, I know...)
 *
 *      Exposes a single memory zone available for allocation. There is never
 *      any space between memory blocks, and there will never be two contiguous
 *      free memory blocks.
 */

#ifndef e_malloc_h

#include "t_typedef.h"

#define e_malloc_h_E_Memblock E_Memblock
#define e_malloc_h_E_Init E_Init
#define e_malloc_h_E_Destroy E_Destroy
#define e_malloc_h_E_Malloc E_Malloc
#define e_malloc_h_E_Free E_Free
#define e_malloc_h_E_Memcpy E_Memcpy
#define e_malloc_h_E_Relloc E_Realloc
#define e_malloc_h_E_Verify E_Verify
#define e_malloc_h_E_Dump E_Dump

typedef struct memblock {
    int size;
    void* owner;
    int tag;
    struct memblock *p_prev, *p_next;
} E_Memblock;

void E_Init (int sizemib);
void E_Destroy (void);
void* E_Malloc (int size, void* requester);
void* E_Free (void* ptr);
void E_Memcpy (void* dest, void* src, int size);
void* E_Realloc (void* ptr, int size);
int E_Verify (void);
void E_Dump (void);

#endif
