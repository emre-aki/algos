/*
 *  h_heap.c
 *  algos
 *
 *  Created by Emre Akı on 2023-03-20.
 *
 *  SYNOPSIS:
 *      A simple priority-queue (max-heap) implementation that supports
 *      integer (i32) keys and values.
 */

#include <stdio.h>
#include <math.h>

#include "h_heap.h"

static void H_Swap (HeapNode* heap, int i, int j)
{
    HeapNode aux = *(heap + i);
    *(heap + i) = *(heap + j);
    *(heap + j) = aux;
}

static int H_Parent (int index)
{
    return ceil((index * 0.5) - 1);
}

static int H_Child (int index, int right)
{
    return (index << 1) + right + 1;
}

void H_Heapify (HeapNode* raw, HeapNode* heap, int size)
{
    for (int i = 0; i < size; ++i)
    {
        *(heap + i) = *(raw + i);
        int key = (heap + i)->key;
        int self = i, parent = H_Parent(self);
        /* bubble-up procedure */
        while (parent >= 0 && (heap + parent)->key < key)
        {
            H_Swap(heap, parent, self);
            self = parent;
            parent = H_Parent(self);
        }
    }
}

HeapNode H_HeapPop (HeapNode* heap, int* size)
{
    HeapNode popped = *heap;
    int newsize = *size - 1;
    *heap = *(heap + newsize); // promote the last node to the root
    int key = heap->key;
    /* start the bubble-down procedure */
    int self = 0, left = H_Child(self, 0), right = H_Child(self, 1);
    /* continue bubbling-down as long as the current node still has a child */
    while (left < newsize)
    {
        int leftkey = (heap + left)->key;
        /* if the current node has 2 children */
        if (right < newsize)
        {
            int rightkey = (heap + right)->key;
            // stop bubbling-down if the current node has higher priority than
            // its children
            if (key >= leftkey && key >= rightkey) break;
            /* if the left child has higher priority than the right child, swap
             * the current node with the left child
             */
            if (leftkey > rightkey)
            {
                H_Swap(heap, self, left);
                self = left;
            }
            /* if the right child has higher priority than the left child, swap
             * the current node with the right child
             */
            else
            {
                H_Swap(heap, self, right);
                self = right;
            }
            left = H_Child(self, 0);
            right = H_Child(self, 1);
        }
        /* swap the current node with its only child if the child has higher
         * priority than its parent
         */
        else if (leftkey > key)
        {
            H_Swap(heap, self, left);
            break;
        }
        /* stop bubbling-down if the current node has higher priority than its
         * only child
         */
        else
            break;
    }
    *size = newsize;
    return popped;
}

void H_PrintHeap (HeapNode* heap, int size)
{
    printf("Priority queue @%p:\n\n", heap);
    for (int i = 0; i < size; ++i)
    {
        HeapNode* node = heap + i;
        printf("[%d] data: %d\tkey: %d\tloc: %p\n",
               i, node->data, node->key, node);
    }
}
