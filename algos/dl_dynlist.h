/*
 *  dl_dynlist.h
 *  algos
 *
 *  Created by Emre Akı on 2023-06-24.
 *
 *  SYNOPSIS:
 *      List of objects of equal size that grows dynamically in heap memory as
 *      new objects are added. When there's no room for further addition to the
 *      list, memory reserved is doubled by a realloc.
 */

#ifndef dl_dynlist_h

#include "t_typedef.h"

#define dl_dynlist_h
#define dl_dynlist_h_DL_Alloc DL_Alloc
#define dl_dynlist_h_DL_Length DL_Length
#define dl_dynlist_h_DL_Push DL_Push
#define dl_dynlist_h_DL_PopFront DL_Pop
#define dl_dynlist_h_DL_Free DL_Free
#define dl_dynlist_h_DL_Print DL_Print

typedef struct {
    size_t size;
    size_t length;
    size_t unit;
} DL_Dynlist;

void* DL_Alloc (size_t size, size_t unit);
size_t DL_Length (void* p_list);
void* DL_Push (void** p_p_list);
void* DL_Pop (void* p_list);
void DL_Free (void* p_list);
void DL_Print (void* p_list);

#endif
