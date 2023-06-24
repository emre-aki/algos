/*
 *  a_avl.h
 *  algos
 *
 *  Created by Emre Akı on 2021-05-10.
 *
 *  SYNOPSIS:
 *      A module for a self-balancing binary tree data structure known as, AVL
 *      trees.
 *
 *      Balancing is performed at each insertion to the tree to keep the depth
 *      of the tree at a minimum with the intention of preserving the
 *      `O(log n)` search complexity.
 */

#ifndef a_avl_h

#define a_avl_h
#define a_avl_h_AVL_Tree AVL_Tree
#define a_avl_h_AVL_Node AVL_Node
#define a_avl_h_AVL_InitTree AVL_InitTree
#define a_avl_h_AVL_IsEmpty AVL_IsEmpty
#define a_avl_h_AVL_Push AVL_Push
#define a_avl_h_AVL_Depth AVL_Depth
#define a_avl_h_AVL_Destroy AVL_Destroy
#define a_avl_h_AVL_Dump AVL_Dump


typedef struct avl_node {
    int data;
    struct avl_node* p_left;
    struct avl_node* p_right;
} AVL_Node;

typedef struct {
    AVL_Node* p_root;
} AVL_Tree;

AVL_Tree* AVL_InitTree (void);
int AVL_IsEmpty (AVL_Tree tree);
void AVL_Push (AVL_Tree* p_tree, int data);
int AVL_Depth (AVL_Tree* p_tree);
void AVL_Destroy (AVL_Tree* p_tree);
void AVL_Dump (AVL_Tree tree);

#endif
