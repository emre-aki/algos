/*
 *  s_buffer.c
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

#include <stdio.h>

#include "e_malloc.h"
#include "s_buffer.h"
#include "t_typedef.h"

#define MAX(a, b) ((((a) > (b)) * (a)) + (((b) >= (a)) * (b)))

static sb_node_t* SB_Node (int start, int size, byte id)
{
    sb_node_t* node = (sb_node_t*) E_Malloc(sizeof(sb_node_t), SB_Node);

    node->start = start;
    node->size = size;
    node->prev = 0;
    node->next = 0;
    node->id = id;

    return node;
}

sbuffer_t* SB_Init (int size)
{
    sbuffer_t* sbuffer = (sbuffer_t*) E_Malloc(sizeof(sbuffer_t), SB_Init);

    sbuffer->root = 0;
    sbuffer->size = size;

    return sbuffer;
}

static
int
_SB_Push
( sb_node_t* node,
  int start, int size,
  int left, int right,
  byte id,
  byte* pushed )
{
    // clipping boundaries for the previous (left) subtree
    const int prevleft = left, prevright = node->start;
    // clipping boundaries for the next (right) subtree
    const int nextleft = node->start + node->size, nextright = right;
    // total size remaining after all the clipping
    int remaining = size;

    /* left-insertion: node to insert is to the left of the current node */
    if (start < node->start)
    {
        /* previous subtree is occupied, continue searching for an empty slot */
        if (node->prev)
        {
            remaining = _SB_Push(node->prev,
                                 start, size,
                                 prevleft, prevright,
                                 id,
                                 pushed);
        }
        /* found an empty slot */
        else
        {
            // clip the current segment from left
            const int clipleft = MAX(prevleft - start, 0);
            // ...and right
            const int clipright = MAX(start + size - prevright, 0);
            remaining -= clipleft + clipright;

            /* only insert if there's something left to insert */
            if (remaining > 0)
            {
                node->prev = SB_Node(start + clipleft, remaining, id);
                // update the pointer to `pushed` because we've just pushed
                // something into the buffer
                *pushed = 0xff;
            }

            // update the size remaining after the clipping
            remaining = size - clipleft - remaining;
        }

        // is there anything left from the segment after the left-insertion
        // that we should still proceed with a right-insertion?
        int stillremaining = remaining - node->size;

        if (stillremaining > 0)
        {
            /* next subtree is occupied, continue searching for an empty slot */
            if (node->next)
            {
                remaining = _SB_Push(node->next,
                                     nextleft, stillremaining,
                                     nextleft, nextright,
                                     id,
                                     pushed);
            }
            /* found an empty slot */
            else
            {
                // clipping from the right only is sufficient in this case, as
                // the start of the insertion always touches the end of parent
                const int clipright = MAX(nextleft + stillremaining - nextright,
                                          0);
                stillremaining -= clipright;

                if (stillremaining > 0)
                {
                    node->next = SB_Node(nextleft, stillremaining, id);
                    // update the pointer to `pushed` because we've just pushed
                    // something into the buffer
                    *pushed = 0xff;
                }

                // update the size remaining after the clipping
                remaining -= node->size + stillremaining;
            }
        }
    }
    /* right-insertion: node to insert is to the right of the current node */
    else
    {
        /* next subtree is occupied, continue searching for an empty slot */
        if (node->next)
        {
            remaining = _SB_Push(node->next,
                                 start, size,
                                 nextleft, nextright,
                                 id,
                                 pushed);
        }
        /* found an empty slot */
        else
        {
            // clip the current segment from left
            const int clipleft = MAX(nextleft - start, 0);
            // ...and right
            const int clipright = MAX(start + size - nextright, 0);
            remaining -= clipleft + clipright;

            if (remaining > 0)
            {
                node->next = SB_Node(start + clipleft, remaining, id);
                // update the pointer to `pushed` because we've just pushed
                // something into the buffer
                *pushed = 0xff;
            }

            // update the size remaining after the clipping
            remaining = size - clipleft - remaining;
        }
    }

    return remaining;
}

int SB_Push (sbuffer_t* sbuffer, int start, int size, byte id)
{
    /* the buffer is empty */
    if (!sbuffer->root)
    {
        // clip the current segment from left
        const int clipleft = MAX(-start, 0);
        // ...and right
        const int clipright = MAX(start + size - sbuffer->size, 0);
        const int remaining = size - clipleft - clipright;

        /* only insert if there's something left to insert */
        if (remaining > 0)
        {
            sbuffer->root = SB_Node(start + clipleft, remaining, id);

            return 0;
        }

        return 1;
    }

    // a flag to indicate whether we've been able to insert at least a portion
    // of the segment into the buffer, or if the space the segment should be
    // inserted into has already completely occluded
    byte pushed = 0;
    _SB_Push(sbuffer->root, start, size, 0, sbuffer->size, id, &pushed);

    if (!pushed)
    {
        printf("[SB_Push] Cannot add more segments, spot fully occluded!\n");

        return 1;
    }

    return 0;
}

static void _SB_Dump (sb_node_t* node, size_t depth)
{
    printf("[%zu] (id=%c) %d + %d\n", depth, node->id, node->start, node->size);
    if (node->prev) _SB_Dump(node->prev, depth + 1);
    if (node->next) _SB_Dump(node->next, depth + 1);
}

void SB_Dump (sbuffer_t* sbuffer)
{
    sb_node_t* root = sbuffer->root;
    if (!root)
    {
        printf("[SB_Dump] Empty S-Buffer!\n");

        return;
    }

    _SB_Dump(root, 0);
}

static void _SB_Print (sb_node_t* node, byte* span)
{
    int offset = node->start;

    for (int i = 0; i < node->size; ++i) *(span + offset++) = node->id;
    if (node->prev) _SB_Print(node->prev, span);
    if (node->next) _SB_Print(node->next, span);
}

void SB_Print (sbuffer_t* sbuffer)
{
    int spansize = sbuffer->size;
    byte span[spansize + 1];

    *(span + spansize) = 0;
    for (int i = 0; i < spansize; ++i) *(span + i) = '_';
    if (sbuffer->root) _SB_Print(sbuffer->root, span);
    printf("%s\n", span);
}

void _SB_Destroy (sb_node_t* node)
{
    if (node->prev) _SB_Destroy(node->prev);
    if (node->next) _SB_Destroy(node->next);
    E_Free(node);
}

void SB_Destroy (sbuffer_t* sbuffer)
{
    if (sbuffer->root) _SB_Destroy(sbuffer->root);
    E_Free(sbuffer);
}
