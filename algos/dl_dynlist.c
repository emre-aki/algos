/*
 *  dl_dynlist.c
 *  algos
 *
 *  Created by Emre Akı on 2023-06-24.
 *
 *  SYNOPSIS:
 *      List of objects of equal size that grows dynamically in heap memory as
 *      new objects are added. When there's no room for further addition to the
 *      list, memory reserved is doubled by a realloc.
 */

#include <stdio.h>

#include "e_malloc.h"
#include "dl_dynlist.h"

#define DYNLIST(p_list) ((DL_Dynlist*) ((byte*) p_list - SIZE_HEADER))

static const size_t SIZE_HEADER = sizeof(DL_Dynlist);

/* TODO: add "push front" and "pop front" functionality */

void* DL_Alloc (size_t size, size_t unit)
{
    DL_Dynlist* dynlist = (DL_Dynlist*) E_Malloc(SIZE_HEADER + size * unit,
                                                 DL_Alloc);
    dynlist->size = size;
    dynlist->length = 0;
    dynlist->unit = unit;
    return (void*) ((byte*) dynlist + SIZE_HEADER);
}

size_t DL_Length (void* p_list)
{
    return DYNLIST(p_list)->length;
}

/* returns a void pointer to the next available slot in the list for the caller
 * to assign, having the width specified when initializing the list
 */
void* DL_Push (void** p_p_list) // peepee list lol
{
    byte* p_list = (byte*) *p_p_list;
    DL_Dynlist* dynlist = DYNLIST(p_list);
    size_t size = dynlist->size, length = dynlist->length, unit = dynlist->unit;
    /* there's no room left in the memory reserved for the list, double its size
     */
    if (size == length)
    {
        size_t newsize = size << 1;
        dynlist->size = newsize;
        dynlist = (DL_Dynlist*) E_Realloc(dynlist,
                                          SIZE_HEADER + newsize * unit);
        *p_p_list = (void*) ((byte*) dynlist + SIZE_HEADER);
        p_list = (byte*) *p_p_list;
    }
    byte* newslot = p_list + (dynlist->length)++ * unit;
    return (void*) newslot;
}

void* DL_Pop (void* p_list)
{
    DL_Dynlist* dynlist = DYNLIST(p_list);
    size_t length = --(dynlist->length), unit = dynlist->unit;
    return (void*) ((byte*) p_list + length * unit);
}

void DL_Free (void* p_list)
{
    E_Free(DYNLIST(p_list));
}

void DL_Print (void* p_list)
{
    byte* rawlist = (byte*) p_list;
    DL_Dynlist* dynlist = DYNLIST(rawlist);
    size_t length = dynlist->length;
    if (!length)
    {
        printf("[]\n");
        return;
    }
    size_t stopat = length - 1, unit = dynlist->unit;
    size_t i;
    printf("[");
    for (i = 0; i < stopat; ++i) printf("0x%x, ", *(rawlist + i * unit));
    printf("0x%x]\n", *(rawlist + i * unit));
}
