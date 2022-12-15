/*
 *  q_queue.c
 *  algos
 *
 *  Created by Emre Akı on 2022-12-14.
 *
 *  SYNOPSIS:
 *      A module that helps in keeping a list of elements in a queue structure.
 */

#include <stdio.h>

#include "e_malloc.h"
#include "q_queue.h"

static qnode_t* Q_InitNode (void* data)
{
    qnode_t* qnode = (qnode_t*) E_Malloc(sizeof(qnode_t), Q_InitNode);
    qnode->data = data;
    qnode->next = NULL;
    return qnode;
}

queue_t* Q_Init (void)
{
    queue_t* queue = (queue_t*) E_Malloc(sizeof(queue_t), Q_Init);
    queue->head = NULL;
    queue->tail = NULL;
    return queue;
}

void Q_Push (queue_t* queue, void* data)
{
    qnode_t* qnode = Q_InitNode(data);
    if (Q_IsEmpty(queue))
    {
        queue->head = qnode;
        queue->tail = qnode;
        return;
    }
    queue->tail->next = qnode;
    queue->tail = qnode;
}

void* Q_Pop (queue_t* queue)
{
    if (Q_IsEmpty(queue)) return NULL;
    qnode_t* popped = queue->head;
    void* data = popped->data;
    if (queue->head == queue->tail)
    {
        queue->head = NULL;
        queue->tail = NULL;
    }
    else
        queue->head = queue->head->next;
    E_Free(popped);
    return data;
}

int Q_IsEmpty (queue_t* queue)
{
    return !queue->head;
}

void Q_Destroy (queue_t* queue)
{
    while(!Q_IsEmpty(queue)) Q_Pop(queue);
    E_Free(queue);
}
