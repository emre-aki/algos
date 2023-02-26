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
#include "m_fixed.h"
#include "m_lookat.h"
#include "q_queue.h"
#include "z_zigzagtree.h"
#include "s_subsets.h"
#include "s_substring.h"

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
    double matrix[] = { 1, 0, 0, 1, 1, 0, 0, 0,
                        0, 1, 0, 2, 0, 1, 0, 0,
                        0, 0, 1, 3, 0, 0, 1, 0,
                        0, 0, 0, 1, 0, 0, 0, 1 };

    double* rref = M_ToRREF(matrix, 4, 8, 4);
    M_Dump(rref, 4, 8);
    if (rref) E_Free(rref);
    E_Dump();
}

void TestFixedPoint (void)
{
    double da = 3, db = -0.8; fixed_t fa = M_ToFixed(da), fb = M_ToFixed(db);
    double ftoda = M_ToDouble(fa), ftodb = M_ToDouble(fb);
    /* test fixed-point multiplication */
    double doublemul = da * db, fixedmul = M_ToDouble(M_Mul(fa, fb));
    printf("(double) %.1f * (double) %.1f = %.20f\n", da, db, doublemul);
    printf("(fixed_t) %.1f * (fixed_t) %.1f = %.20f\n", ftoda, ftodb, fixedmul);
    double errorpercent = (fixedmul - doublemul) / doublemul;
    errorpercent *= errorpercent > 0 ? 100 : -100;
    printf("Rounding error = %.3f%%\n", errorpercent);
    /* test fixed-point division */
    double doublediv = da / db, fixeddiv = M_ToDouble(M_Div(fa, fb));
    printf("(double) %.1f / (double) %.1f = %.20f\n", da, db, doublediv);
    printf("(fixed_t) %.1f / (fixed_t) %.1f = %.20f\n", ftoda, ftodb, fixeddiv);
    errorpercent = (fixeddiv - doublediv) / doublediv;
    errorpercent *= errorpercent > 0 ? 100 : -100;
    printf("Rounding error = %.3f%%\n", errorpercent);
    /* test various fixed point operations */
    printf("floor(4.3) = %d\n", (int) M_ToDouble(M_Floor(M_ToFixed(4.3))));
    printf("floor(-4.3) = %d\n", (int) M_ToDouble(M_Floor(M_ToFixed(-4.3))));
    printf("ceil(4.3) = %d\n", (int) M_ToDouble(M_Ceil(M_ToFixed(4.3))));
    printf("ceil(-4.3) = %d\n", (int) M_ToDouble(M_Ceil(M_ToFixed(-4.3))));
    printf("round(4.3) = %d\n", (int) M_ToDouble(M_Round(M_ToFixed(4.3))));
    printf("round(-4.3) = %d\n", (int) M_ToDouble(M_Round(M_ToFixed(-4.3))));
    printf("round(4.5) = %d\n", (int) M_ToDouble(M_Round(M_ToFixed(4.5))));
    printf("round(-4.5) = %d\n", (int) M_ToDouble(M_Round(M_ToFixed(-4.5))));
    printf("abs(-4.314) = %f\n", M_ToDouble(M_Abs(M_ToFixed(-4.314))));
    printf("abs(4.314) = %f\n", M_ToDouble(M_Abs(M_ToFixed(4.314))));
}

void TestLookAt (void)
{
    vec3_t eye = { 0, 0, 0 };
    vec3_t center = { 0, 0.7071067811865476, 0.7071067811865476 };
    vec3_t up = { 0, 1, 0 };
    mat4_t* lookat = M_LookAt(&eye, &center, &up);
    M_Debug(lookat);
    E_Dump();
    E_Free(lookat);
    E_Dump();
}

void TestQueue (void)
{
    queue_t* queue = Q_Init();
    node_t* node0 = (node_t*) E_Malloc(sizeof(node_t), TestQueue);
    node0->data = 42;
    node_t* node1 = (node_t*) E_Malloc(sizeof(node_t), TestQueue);
    node1->data = 43;
    node_t* node2 = (node_t*) E_Malloc(sizeof(node_t), TestQueue);
    node2->data = 44;
    E_Dump();
    Q_Push(queue, node0);
    Q_Push(queue, node1);
    Q_Push(queue, node2);
    E_Dump();
    Q_Print(queue);
    Q_Pop(queue);
    Q_Print(queue);
    Q_Pop(queue);
    Q_Print(queue);
    Q_Pop(queue);
    Q_Print(queue);
    E_Dump();
    node_t* node3 = (node_t*) E_Malloc(sizeof(node_t), TestQueue);
    node3->data = 45;
    Q_Push(queue, node3);
    Q_Print(queue);
    E_Dump();
    Q_Destroy(queue);
    E_Dump();
    E_Free(node0);
    E_Free(node1);
    E_Free(node2);
    E_Free(node3);
}

void TestTree (void)
{
    E_Dump();
    tree_t* tree = Z_Init();
    Z_Insert(tree, 1);
    Z_Insert(tree, 2);
    Z_Insert(tree, 3);
    Z_Print(tree);
    E_Dump();
    Z_Insert(tree, 7);
    Z_Insert(tree, 6);
    Z_Insert(tree, 5);
    Z_Insert(tree, 4);
    printf("size = %d\n", Z_Size(tree));
    Z_Print(tree);
    E_Dump();
    Z_Insert(tree, 8);
    Z_Insert(tree, 9);
    Z_Insert(tree, 10);
    Z_Insert(tree, 11);
    Z_Insert(tree, 12);
    Z_Insert(tree, 13);
    Z_Print(tree);
    E_Dump();
    Z_PrintZigZag(tree);
    Z_Destroy(tree);
    E_Dump();
    Z_Print(tree);
    Z_PrintZigZag(tree);
}

void TestSubsets ()
{
    int nNums = 5;
    int nums[5] = { 1, 2, 3, 4, 5 };
    S_TestSubsets(nums, nNums);
}

void TestSubstrings ()
{
    S_LongestSubstringWithEvenVowels("eleetminicoworoep");
    S_LongestSubstringWithEvenVowels("Hello, world!");
    S_LongestSubstringWithNonRepeatingChars("eleetminicoworoep");
    S_LongestSubstringWithNonRepeatingChars("Hello, world!");
}

int main (int argc, const char** argv)
{
    E_Init(1);
    TestAVL();
    TestMatrixInversion();
    TestMatrixRREF();
    TestLookAt();
    TestFixedPoint();
    TestQueue();
    TestTree();
    E_Dump();
    TestSubsets();
    E_Destroy();
    TestSubstrings();
    return 0;
}
