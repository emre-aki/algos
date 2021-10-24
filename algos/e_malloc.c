/*
 *  m_emalloc.c
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
 *
 *  TODO:
 *      - Add descriptions for "public" functions
 */

#include <stdlib.h>
#include <stdio.h>

#include "e_malloc.h"

static const int E_TAG = 1;
static const int SIZE_HEADER = sizeof(E_Memblock);

E_Memblock* p_mainmemory = NULL;
E_Memblock* p_rover = NULL;

static void E_InitBlock (E_Memblock* p_block, int size, void* owner, int tag,
                         E_Memblock* p_prev, E_Memblock* p_next)
{
    p_block->size = size;
    p_block->owner = owner;
    p_block->tag = tag;
    p_block->p_prev = p_prev;
    p_block->p_next = p_next;
}

void E_Init (int sizemib)
{
    int size = sizemib * 1024 * 1024 + SIZE_HEADER;
    // allocate an entire contiguous block of memory for our custom allocator
    // to manage
    E_Memblock* p_memhead = (E_Memblock*) ((byte*) malloc(size));
    // initialize the list of memory blocks
    E_InitBlock(p_memhead, size - SIZE_HEADER, NULL, E_TAG, NULL, NULL);
    p_mainmemory = p_memhead;
    // initialize the rover to point to the head of the memory blocks
    p_rover = p_mainmemory;
}

void E_Destroy (void)
{
    for (E_Memblock* p_current = p_mainmemory; p_current;
         p_current = p_current->p_next)
        if (p_current->owner)
            E_Free((byte*) p_current + SIZE_HEADER);
    free((byte*) p_mainmemory);
    p_mainmemory = NULL; p_rover = NULL;
}

void* E_Malloc (int size, void* requester)
{
    // early return if the memory had not been initialized
    if (!p_mainmemory)
    {
        printf("E_Malloc: Uninitialized memory.\n");
        return NULL;
    }
    // fix input size to a factor of 4
    size = (size + 3) >> 2 << 2; // = (size + 3) & ~3 = 4 * ((size + 3) / 4)
    // account for the header of the next block
    int sizereq = size + SIZE_HEADER;
    int lap = 0;
    E_Memblock* p_start = p_rover, *p_current = p_rover;
    /* try to find a big enough block to allocate for the requester */
    while (!lap && (p_current->size < sizereq || p_current->owner != NULL))
    {
        p_current = p_current->p_next;
        // wrap around and point back to the head of the main memory
        if (p_current == NULL) p_current = p_mainmemory;
        // if a full revolution is complete and an appropriate memory block
        // had not been found, raise the `lap` flag
        if (p_current == p_start) lap = 1;
    }
    if (lap)
    {
        printf("E_Malloc: Insufficient memory for %dBs.\n", size);
        return NULL;
    }
    /* found a block that is big enough for the requester */
    int sizefound = p_current->size;
    int nextsize = sizefound - sizereq;
    byte* p_freeroom = (byte*) p_current + SIZE_HEADER;
    E_Memblock* p_newprev = p_current->p_prev;
    E_Memblock* p_newnext = (E_Memblock*) (p_freeroom + size);
    E_Memblock* p_newnextnext = p_current->p_next;
    E_InitBlock(p_current, size, requester, E_TAG, p_newprev, p_newnext);
    E_InitBlock(p_newnext, nextsize, NULL, E_TAG, p_current, p_newnextnext);
    // fix the `prev` pointer of the next of the `newnext`
    if (p_newnextnext) p_newnextnext->p_prev = p_newnext;
    p_rover = p_newnext; // future searches for allocation will start here
    // return the address for the newly allocated block
    return (void*) p_freeroom;
}

void* E_Free (void* ptr)
{
    E_Memblock* p_blockhead = (E_Memblock*) ((byte*) ptr - SIZE_HEADER);
    if (p_blockhead->owner == NULL)
    {
        printf("E_Free: The block does not have an owner.\n");
        return NULL;
    }
    if (p_blockhead->tag != E_TAG)
    {
        printf("E_Free: The block had not been initialized by E_Malloc.\n");
        return NULL;
    }
    E_Memblock* p_prev = p_blockhead->p_prev;
    E_Memblock* p_next = p_blockhead->p_next;
    int sizetotal = p_blockhead->size;
    /* merge with next block if it is free */
    if (p_next != NULL && p_next->owner == NULL)
    {
        sizetotal += p_next->size + SIZE_HEADER;
        E_InitBlock(p_blockhead, sizetotal, NULL, E_TAG,
                    p_prev, p_next->p_next);
        // back-up rover if it is pointing currently to `p_next`
        if (p_rover == p_next) p_rover = p_blockhead;
        p_next = p_blockhead->p_next;
    }
    // if not, just free the block itself
    else E_InitBlock(p_blockhead, sizetotal, NULL, E_TAG, p_prev, p_next);
    /* merge with previous block if it is free */
    if (p_prev != NULL && p_prev->owner == NULL)
    {
        sizetotal += p_prev->size + SIZE_HEADER;
        E_InitBlock(p_prev, sizetotal, NULL, E_TAG, p_prev->p_prev, p_next);
        // back-up rover if it is pointing currently to `p_blockhead`
        if (p_rover == p_blockhead) p_rover = p_prev;
        p_blockhead = p_prev;
    }
    // fix the `prev` pointer of the `next`
    if (p_next) p_next->p_prev = p_blockhead;
    return p_blockhead;
}

void E_Memcpy (void* dest, void* src, int size)
{
    byte *cpydest = (byte*) dest, *cpysrc = (byte*) src;
    for (int i = 0; i < size; i += 1) *(cpydest + i) = *(cpysrc + i);
}

void* E_Realloc (void* ptr, int size)
{
    E_Memblock* p_blockhead = (E_Memblock*) ((byte*) ptr - SIZE_HEADER);
    // try to allocate a new memory block with the requested size
    byte* p_dest = (byte*) E_Malloc(size, p_blockhead->owner);
    // early return if a memory block with sufficient size could not be found
    if (p_dest == NULL)
    {
        printf("E_Realloc: Insufficient memory for %dBs.\n", size);
        return NULL;
    }
    /* copy the old memory block to the newly malloced */
    byte* p_src = (byte*) p_blockhead + SIZE_HEADER;
    E_Memcpy(p_dest, p_src, p_blockhead->size);
    E_Free(p_src); // free the old block
    return (void*) p_dest;
}

void E_Dump (void)
{
    if (!E_Verify())
    {
        printf("Heap dump @%p:\n\n", p_mainmemory);
        for (E_Memblock* p_current = p_mainmemory; p_current;
             p_current = p_current->p_next)
        {
            byte* ptr = (byte*) p_current;
            if (p_current->owner) ptr += SIZE_HEADER;

            printf("loc: %p\tsize: %d", ptr, p_current->size);

            if (p_current->owner) printf("\towner: %p", p_current->owner);
            else printf("\towner: NULL");

            if (p_current == p_rover) printf("\t(*)");

            printf("\n");
        }
    }
}

int E_Verify (void)
{
    E_Memblock* p_current = p_mainmemory;
    int lap = 0, error = 0;
    if (!p_current)
    {
        printf("E_Verify: Uninitialized memory.\n");
        error = 1;
    }
    while (!lap && !error)
    {
        E_Memblock* p_next = p_current->p_next;
        if (p_next == NULL) p_next = p_mainmemory;

        E_Memblock* p_selftestprev = NULL;
        if (p_current == p_mainmemory) p_selftestprev = p_current;
        else p_selftestprev = p_current->p_prev->p_next;

        E_Memblock* p_selftestnext = NULL;
        if (p_next == p_mainmemory) p_selftestnext = p_current;
        else p_selftestnext = p_current->p_next->p_prev;

        if (
            // bypass this test if the last block in the memory
            p_next != p_mainmemory &&
            // add size of the block header to its allotted size and check if
            // this address actually points to its `next`
            (byte*) p_current + p_current->size + SIZE_HEADER != (byte*) p_next)
        {
            printf("E_Verify: [%p] Block does not touch to its next.\n",
                   p_current);
            error = 1;
        }

        if (p_current != p_selftestprev)
        {
            printf("E_Verify: [%p] Block has an improper previous link.\n",
                   p_current);
            error = 1;
        }

        if (p_current != p_selftestnext)
        {
            printf("E_Verify: [%p] Block has an improper next link.\n",
                   p_current);
            error = 1;
        }

        // bypass this test if the last block in the memory
        if (p_next != p_mainmemory && !p_current->owner && !p_next->owner)
        {
            printf("E_Verify: [%p] Two consecutive vacant blocks in memory.\n",
                   p_current);
            error = 1;
        }

        // memory block had not been initialized via `E_Malloc`
        if (p_current->tag != E_TAG)
        {
            printf("E_Verify: [%p] The block had not been initialized by " \
                   "E_Malloc.\n", p_current);
            error = 1;
        }

        p_current = p_next;
        // reached to the end of the main memory
        if (p_current == p_mainmemory) lap = 1;
    }

    return error;
}
