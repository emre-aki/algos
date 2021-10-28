/*
 *  a_avl.c
 *  algos
 *
 *  Created by Emre Akı on 2021-05-10.
 *
 *  SYNOPSIS:
 *      A module for the self-balancing binary tree data structure known as AVL
 *      trees.
 *
 *      Balancing is performed at each insertion to the tree to keep the depth
 *      of the tree at a minimum with the intention of preserving the
 *      `O(log n)` search complexity.
 */

#include <stdio.h>

#include "t_typedef.h"
#include "e_malloc.h"
#include "a_avl.h"

static AVL_Node* AVL_InitNode (int data)
{
    AVL_Node* p_node = (AVL_Node*) E_Malloc(sizeof(AVL_Node), AVL_InitNode);
    p_node->data = data;
    p_node->p_left = NULL;
    p_node->p_right = NULL;
    return p_node;
}

static void AVL_PrintNode (AVL_Node* p_node)
{
    printf("data: %d", p_node->data);

    if (p_node->p_left) printf("\tleft: %d", p_node->p_left->data);
    else printf("\tleft: NULL");

    if (p_node->p_right) printf("\tright: %d", p_node->p_right->data);
    else printf("\tright: NULL");

    printf("\n");
}

static void AVL_PrintPreOrder (AVL_Node* p_root)
{
    AVL_PrintNode(p_root);
    if (p_root->p_left) AVL_PrintPreOrder(p_root->p_left);
    if (p_root->p_right) AVL_PrintPreOrder(p_root->p_right);
}

static void AVL_Push_ (AVL_Node* p_root, AVL_Node* p_node)
{
    if (p_root->data >= p_node->data)
    {
        if (p_root->p_left == NULL) p_root->p_left = p_node;
        else AVL_Push_(p_root->p_left, p_node);
    }
    else if (p_root->data < p_node->data)
    {
        if (p_root->p_right == NULL) p_root->p_right = p_node;
        else AVL_Push_(p_root->p_right, p_node);
    }
}

static int AVL_Balance_ (AVL_Node* p_node, AVL_Node* p_parent, AVL_Tree* p_tree)
{
    // base case: if a leaf node, the subtree is balanced by default, return
    // a depth value of 1
    if (p_node->p_left == NULL && p_node->p_right == NULL) return 1;
    /* recursively call the same routine for left and right children */
    int depthleft = 0, depthright = 0;
    if (p_node->p_left)
        depthleft = AVL_Balance_(p_node->p_left, p_node, p_tree);
    if (p_node->p_right)
        depthright = AVL_Balance_(p_node->p_right, p_node, p_tree);
    /* calculate the balancing factor for the current subtree, and perform
     * rotations based on the orientation of the subtree if needed
     */
    int balancefactor = depthleft - depthright;
    /* unbalanced due to the right subtree */
    if (balancefactor < -1)
    {
        /* balance subtree (double rotation) */
        if (p_node->p_right->p_left != NULL)
        {
            AVL_Node* p_newintroot = p_node->p_right->p_left;
            p_node->p_right->p_left = p_newintroot->p_right;
            p_newintroot->p_right = p_node->p_right;
            p_node->p_right = p_newintroot;
        }
        /* balance subtree (single rotation) */
        AVL_Node* p_newroot = p_node->p_right;
        p_node->p_right = p_newroot->p_left;
        p_newroot->p_left = p_node;
        /* update parent/root */
        // if parent is NULL, it means we are at the absolute root of the tree
        if (p_parent == NULL) p_tree->p_root = p_newroot;
        else if (p_parent->p_left == p_node) p_parent->p_left = p_newroot;
        else if (p_parent->p_right == p_node) p_parent->p_right = p_newroot;
        return depthright; // return the depth after balancing
    }
    /* unbalanced due to the left subtree */
    if (balancefactor > 1)
    {
        /* balance subtree (double rotation) */
        if (p_node->p_left->p_right != NULL)
        {
            AVL_Node* p_newintroot = p_node->p_left->p_right;
            p_node->p_left->p_right = p_newintroot->p_left;
            p_newintroot->p_left = p_node->p_left;
            p_node->p_left = p_newintroot;
        }
        /* balance subtree (single rotation) */
        AVL_Node* p_newroot = p_node->p_left;
        p_node->p_left = p_newroot->p_right;
        p_newroot->p_right = p_node;
        /* update parent/root */
        // if parent is NULL, it means we are at the absolute root of the tree
        if (p_parent == NULL) p_tree->p_root = p_newroot;
        else if (p_parent->p_left == p_node) p_parent->p_left = p_newroot;
        else if (p_parent->p_right == p_node) p_parent->p_right = p_newroot;
        return depthleft; // return the depth after balancing
    }
    return (depthleft >= depthright ? depthleft : depthright) + 1;
}

static void AVL_Balance (AVL_Tree* p_tree)
{
    AVL_Balance_(p_tree->p_root, NULL, p_tree);
}

static int AVL_Depth_ (AVL_Node* p_root)
{
    if (p_root == NULL) return 0;
    int depthleft = AVL_Depth_(p_root->p_left);
    int depthright = AVL_Depth_(p_root->p_right);
    int maxdepth = depthleft >= depthright ? depthleft : depthright;
    return maxdepth + 1;
}

static void AVL_Destroy_ (AVL_Node* p_node)
{
    E_Free(p_node);// destroy this node
    /* destroy left and right subtrees recursively */
    AVL_Node* p_left = p_node->p_left;
    AVL_Node* p_right = p_node->p_right;
    if (p_left) AVL_Destroy_(p_left);
    if (p_right) AVL_Destroy_(p_right);
}

AVL_Tree* AVL_InitTree (void)
{
    AVL_Tree* p_tree = (AVL_Tree*) E_Malloc(sizeof(AVL_Tree), AVL_InitTree);
    p_tree->p_root = NULL;
    return p_tree;
}

int AVL_IsEmpty (AVL_Tree tree)
{
    return tree.p_root == NULL;
}

void AVL_Push (AVL_Tree* p_tree, int data)
{
    AVL_Node* p_node = AVL_InitNode(data);
    if (AVL_IsEmpty(*p_tree)) p_tree->p_root = p_node;
    else AVL_Push_(p_tree->p_root, p_node);
    AVL_Balance(p_tree);
}

int AVL_Depth (AVL_Tree* p_tree)
{
    if (!p_tree) return 0;
    return AVL_Depth_(p_tree->p_root);
}

void AVL_Destroy (AVL_Tree* p_tree)
{
    if (!p_tree) return;
    AVL_Destroy_(p_tree->p_root);
    E_Free(p_tree);
}

void AVL_Dump (AVL_Tree tree)
{
    AVL_Node* p_root = tree.p_root;
    printf("AVL tree @%p:\n\n", (byte*) p_root);
    AVL_PrintPreOrder(p_root);
}
