/*
 *  d_disjointset.c
 *  algos
 *
 *  Created by Emre Akı on 2021-05-07.
 *
 *  SYNOPSIS:
 *      A module for Disjoint Set data structure.
 */

#include <stdio.h>

#include "e_malloc.h"
#include "t_typedef.h"
#include "d_disjointset.h"

static void DS_InitNode (DS_Node* p_node, int id)
{
    p_node->data = 0;
    p_node->id = id;
    p_node->parent_id = id;
    p_node->rank = 1;
}

DS_Set* DS_Init (int size)
{
    DS_Set* p_set = (DS_Set*) E_Malloc(sizeof(DS_Set), DS_Init);
    DS_Node* p_nodes = (DS_Node*) E_Malloc(size * sizeof(DS_Node), DS_Init);
    if (p_set == NULL || p_nodes == NULL)
    {
        printf("DS_Init: Error while allocating memory for the set\n");
        return NULL;
    }
    for (int i = 0; i < size; i += 1) DS_InitNode(p_nodes + i, i);
    p_set->p_nodes = p_nodes;
    p_set->size = size;
    return p_set;
}

void DS_Destroy (DS_Set* p_set)
{
    E_Free(p_set->p_nodes);
    E_Free(p_set);
}

void DS_SetData (DS_Set* p_set, int id, int data)
{
    (p_set->p_nodes + id)->data = data;
}

static int DS_Rank (DS_Set* p_set, int id)
{
    return (p_set->p_nodes + id)->rank;
}

int DS_Find (DS_Set* p_set, int id)
{
    DS_Node* p_nodes = p_set->p_nodes;
    DS_Node* p_rover = p_nodes + id;
    // look for a self-referencing node, i.e., a root node
    for (; p_rover->id != p_rover->parent_id;
           p_rover = p_nodes + p_rover->parent_id);
    // found the root at this point
    DS_Node* p_root = p_rover;
    // now, "flatten" this disjoint portion of the set
    p_rover = p_nodes + id;
    while (p_rover->id != p_root->id)
    {
        int parent_id = p_rover->parent_id;
        p_rover->parent_id = p_root->id;
        p_rover = p_nodes + parent_id;
    }
    return p_root->id;
}

int DS_Union (DS_Set* p_set, int i, int j)
{
    DS_Node* p_nodes = p_set->p_nodes;
    int root_i = DS_Find(p_set, i), root_j = DS_Find(p_set, j);
    int rank_i = DS_Rank(p_set, root_i), rank_j = DS_Rank(p_set, j);
    // early return if both nodes are in the same set
    if (root_i == root_j) return root_i;
    // unite lower-rank node with that of higher-rank
    if (rank_i >= rank_j)
    {
        (p_nodes + root_j)->parent_id = root_i;
        (p_nodes + root_i)->rank += (p_nodes + root_j)->rank;
        return root_i;
    }
    (p_nodes + root_i)->parent_id = root_j;
    (p_nodes + root_j)->rank += (p_nodes + root_i)->rank;
    return root_j;
}

void DS_Dump (DS_Set* p_set)
{
    int size = p_set->size;
    DS_Node* p_nodes = p_set->p_nodes;
    printf("Disjoint set @%p:\n\n", (byte*) p_set);

    for (int i = 0; i < size; i += 1)
    {
        DS_Node* p_node = p_nodes + i;
        printf("loc: %p\tdata: %d\tid: %d\tparent_id: %d\trank: %d\n",
               (byte*) p_node, p_node->data, p_node->id, p_node->parent_id,
               p_node->rank);
    }
}
