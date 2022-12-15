/*
 *  z_zigzagtree.c
 *  algos
 *
 *  Created by Emre Akı on 2022-12-14.
 *
 *  SYNOPSIS:
 *      A module that helps print a binary tree in a "zig-zag" pattern.
 */

#include <stdio.h>

#include "e_malloc.h"
#include "z_zigzagtree.h"
#include "q_queue.h"

static node_t* Z_InitNode (int data)
{
    node_t* node = (node_t*) E_Malloc(sizeof(node_t), Z_InitNode);
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    return node;
}

tree_t* Z_Init (void)
{
    tree_t* tree = (tree_t*) E_Malloc(sizeof(tree_t), Z_Init);
    tree->root = NULL;
    return tree;
}

void Z_Insert (tree_t* tree, int data)
{
    node_t* node = Z_InitNode(data);
    if (Z_IsEmpty(tree))
    {
        tree->root = node;
        return;
    }
    queue_t* queue = Q_Init();
    Q_Push(queue, tree->root);
    while (!Q_IsEmpty(queue))
    {
        node_t* current = (node_t*) Q_Pop(queue);
        if (!current->left)
        {
            current->left = node;
            break;
        }
        if (!current->right)
        {
            current->right = node;
            break;
        }
        Q_Push(queue, current->left);
        Q_Push(queue, current->right);
    }
    Q_Destroy(queue);
}

int Z_Size (tree_t* tree)
{
    int size = 0;
    if (Z_IsEmpty(tree)) return size;
    queue_t* queue = Q_Init();
    Q_Push(queue, tree->root);
    while (!Q_IsEmpty(queue))
    {
        node_t* current = (node_t*) Q_Pop(queue);
        if (current->left) Q_Push(queue, current->left);
        if (current->right) Q_Push(queue, current->right);
        ++size;
    }
    Q_Destroy(queue);
    return size;
}

int Z_IsEmpty (tree_t* tree)
{
    return !tree->root;
}

void Z_Destroy (tree_t* tree)
{
    if (Z_IsEmpty(tree))
    {
        E_Free(tree);
        return;
    }
    queue_t* queue = Q_Init();
    Q_Push(queue, tree->root);
    while(!Q_IsEmpty(queue))
    {
        node_t* current = (node_t*) Q_Pop(queue);
        if (current->left) Q_Push(queue, current->left);
        if (current->right) Q_Push(queue, current->right);
        current->left = NULL;
        current->right = NULL;
        E_Free(current);
    }
    tree->root = NULL;
    E_Free(tree);
    Q_Destroy(queue);
}

static void Z_PrintNode (node_t* node, int index)
{
    printf("[%d] %p:\n", index, (void*) node);
    if (node->left) printf("left: %p\t", (void*) node->left);
    else printf("left: NULL\t");
    if (node->right) printf("right: %p\t", (void*) node->right);
    else printf("right: NULL\t");
    printf("data: %d\n", node->data);
}

void Z_Print (tree_t* tree)
{
    if (Z_IsEmpty(tree))
    {
        printf("NULL\n");
        return;
    }
    int index = 0;
    queue_t* queue = Q_Init();
    Q_Push(queue, tree->root);
    while(!Q_IsEmpty(queue))
    {
        node_t* current = (node_t*) Q_Pop(queue);
        Z_PrintNode(current, index);
        if (current->left) Q_Push(queue, current->left);
        if (current->right) Q_Push(queue, current->right);
        ++index;
    }
    Q_Destroy(queue);
}

void Z_PrintZigZag (tree_t* tree)
{
    if (Z_IsEmpty(tree))
    {
        printf("NULL\n");
        return;
    }
    int index = 0;
    int size = Z_Size(tree), halfsize = size >> 1;
    node_t** rtl = (node_t**) E_Malloc(sizeof(node_t*) * halfsize,
                                       Z_PrintZigZag);
    node_t** ltr = (node_t**) E_Malloc(sizeof(node_t*) * (size - halfsize),
                                       Z_PrintZigZag);
    int nextrtl = 0, nextltr = 0;
    *ltr = tree->root;
    ++nextltr;
    while (nextrtl || nextltr)
    {
        while (nextrtl)
        {
            node_t* current = *(rtl + nextrtl - 1);
            Z_PrintNode(current, index);
            --nextrtl;
            if (current->right)
            {
                *(ltr + nextltr) = current->right;
                ++nextltr;
            }
            if (current->left)
            {
                *(ltr + nextltr) = current->left;
                ++nextltr;
            }
            ++index;
        }
        while (nextltr)
        {
            node_t* current = *(ltr + nextltr - 1);
            Z_PrintNode(current, index);
            --nextltr;
            if (current->left)
            {
                *(rtl + nextrtl) = current->left;
                ++nextrtl;
            }
            if (current->right)
            {
                *(rtl + nextrtl) = current->right;
                ++nextrtl;
            }
            ++index;
        }
    }
    E_Free(rtl);
    E_Free(ltr);
}
