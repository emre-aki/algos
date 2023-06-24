/*
 *  z_zigzagtree.h
 *  algos
 *
 *  Created by Emre Akı on 2022-12-14.
 *
 *  SYNOPSIS:
 *      A module that helps print a binary tree in a "zig-zag" pattern.
 */

#ifndef zigzag_tree_h

#define zigzag_tree_h
#define z_zigzagtree_h_node_t node_t
#define z_zigzagtree_h_tree_t tree_t
#define z_zigzagtree_h_Z_Init Z_Init
#define z_zigzagtree_h_Z_Insert Z_Insert
#define z_zigzagtree_h_Z_Size Z_Size
#define z_zigzagtree_h_Z_IsEmpty Z_IsEmpty
#define z_zigzagtree_h_Z_Destroy Z_Destroy
#define z_zigzagtree_h_Z_Print Z_Print
#define z_zigzagtree_h_Z_PrintZigZag Z_PrintZigZag

struct node {
    int data;
    struct node *left, *right;
};

typedef struct node node_t;

typedef struct {
    node_t* root;
} tree_t;

tree_t* Z_Init (void);
void Z_Insert (tree_t* tree, int data);
int Z_Size (tree_t* tree);
int Z_IsEmpty (tree_t* tree);
void Z_Destroy (tree_t* tree);
void Z_Print (tree_t* tree);
void Z_PrintZigZag (tree_t* tree);

#endif
