/*
 *  main.c
 *  algos
 *
 *  Created by Emre Akı on 2021-05-09.
 *
 *  SYNOPSIS:
 *      The main program
 */

#include <stdio.h>

#include "e_malloc.h"
#include "d_disjointset.h"
#include "a_avl.h"
#include "m_matrix.h"

void TestDisjointSet (void)
{
    // TODO: write tests for Disjoint Sets
}

void TestAVL (void)
{
    AVL_Tree* p_tree = AVL_InitTree();
    AVL_Push(p_tree, 4);
    AVL_Push(p_tree, 2);
    AVL_Push(p_tree, 3);
    AVL_Dump(*p_tree);
    AVL_Destroy(p_tree);
    E_Dump();
}

void TestMatrix (void)
{
    /* test reducing matrix to row reduced echelon form */
    double matrix[] = { 0, 1, 0,
                        0, 1, 0,
                        1, 1, 1 };

    double* rref = M_ToRREF(matrix, 3, 3, 3);
    M_Dump(rref, 3, 3);

    /* test inverting the matrix */
    double invertthis[] = { 3, -1, 0,
                            1, 3, 0,
                            0, 0, 1 };

    double* inverted = M_Invert(invertthis, 3);
    M_Dump(inverted, 3, 3);
}

int main (int argc, const char * argv[])
{
    E_Init(1);
    TestAVL();
    TestMatrix();
    E_Dump();
    E_Destroy();
    return 0;
}
