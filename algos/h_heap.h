/*
 *  h_heap.h
 *  algos
 *
 *  Created by Emre Akı on 2023-03-20.
 *
 *  SYNOPSIS:
 *      A simple priority-queue (max-heap) implementation that supports
 *      integer (i32) keys and values.
 */

#ifndef h_heap_h

#define h_heap_h
#define h_heap_h_HeapNode HeapNode
#define h_heap_h_H_Heapify H_Heapify
#define h_heap_h_H_HeapPop H_HeapPop
#define h_heap_h_H_PrintHeap H_PrintHeap

typedef struct {
    int data;
    int key;
} HeapNode;

void H_Heapify (HeapNode* raw, HeapNode* heap, int size);
HeapNode H_HeapPop (HeapNode* heap, int* size);
void H_PrintHeap (HeapNode* heap, int size);

#endif
