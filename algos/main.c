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

void TestMatrixInversion (void)
{
    double matrix[] = { 3, -1, 0,
                        1, 3, 0,
                        0, 0, 1 };

    double* inverted = M_Invert(matrix, 3);
    M_Dump(inverted, 3, 3);
    double* testforidentity = M_Mult(inverted, 3, 3, matrix, 3, 3);
    M_Dump(testforidentity, 3, 3);

    double identity[] = { 1, 0, 0,
                          0, 1, 0,
                          0, 0, 1 };

    int equals = M_Equals(testforidentity, identity, 3, 3);
    printf("TestMatrixInversion exited with %d.\n", !equals);
    E_Free(inverted);
    E_Free(testforidentity);
    E_Dump();
}

void TestMatrixRREF (void)
{
    /* test reducing matrix to row reduced echelon form */
    double matrix[] = { 0, 1, 0,
                        0, 1, 0,
                        1, 1, 1 };

    double* rref = M_ToRREF(matrix, 3, 3, 3);
    M_Dump(rref, 3, 3);
    if (rref) E_Free(rref);
    E_Dump();
}

int main (int argc, const char * argv[])
{
    E_Init(1);
    TestAVL();
    TestMatrixInversion();
    TestMatrixRREF();
    E_Destroy();
    return 0;
}
