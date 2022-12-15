/*
 *  q_queue.h
 *  algos
 *
 *  Created by Emre Akı on 2022-12-14.
 *
 *  SYNOPSIS:
 *      A module that helps in keeping a list of elements in a queue structure.
 */

#ifndef queue_h

#define q_queue_h_qnode_t qnode_t
#define q_queue_h_Q_Init Q_Init
#define q_queue_h_Q_Push Q_Push
#define q_queue_h_Q_Pop Q_Pop
#define q_queue_h_Q_IsEmpty Q_IsEmpty
#define q_queue_h_Q_Destroy Q_Destroy
#define q_queue_h_Q_Print Q_Print

struct qnode {
    void* data;
    struct qnode* next;
};

typedef struct qnode qnode_t;

typedef struct {
    qnode_t *head, *tail;
} queue_t;

queue_t* Q_Init (void);
void Q_Push (queue_t* queue, void* data);
void* Q_Pop (queue_t* queue);
int Q_IsEmpty (queue_t* queue);
void Q_Destroy (queue_t* queue);
void Q_Print (queue_t* queue);

#endif
