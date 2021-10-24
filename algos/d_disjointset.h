/*
 *  d_disjointset.h
 *  algos
 *
 *  Created by Emre Akı on 2021-05-07.
 *
 *  SYNOPSIS:
 *      A module for Disjoint Set data structure.
 */

#ifndef d_disjointset_h

#define d_disjointset_h_DS_Node DS_Node
#define d_disjointset_h_DS_Init DS_Init
#define d_disjointset_h_DS_Destroy DS_Destroy
#define d_disjointset_h_DS_Find DS_Find
#define d_disjointset_h_DS_Union DS_Union
#define d_disjointset_h_DS_Dump DS_Dump

typedef struct {
    int data;
    int id, parent_id;
    int rank;
} DS_Node;

typedef struct {
    DS_Node* p_nodes;
    int size;
} DS_Set;

DS_Set* DS_Init (int size);
void DS_Destroy (DS_Set* p_set);
void DS_SetData (DS_Set* p_set, int id, int data);
int DS_Find (DS_Set* p_set, int id);
int DS_Union (DS_Set* p_set, int i, int j);
void DS_Dump (DS_Set* p_set);

#endif
