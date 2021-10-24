/*
 *  m_matrix.c
 *  algos
 *
 *  Created by Emre Akı on 2021-10-22.
 *
 *  SYNOPSIS:
 *      A module for various matrix operations.
 *
 *      Uses the Gauss-Jordan Elimination method to calculate the row reduced
 *      echelon for a given matrix.
 */

#include <stdio.h>

#include "e_malloc.h"
#include "m_matrix.h"

static double M_Get (double* matrix, int cols, int r, int c)
{
    return *(matrix + (cols * r + c));
}

static void M_Set (double* matrix, double value, int cols, int r, int c)
{
    *(matrix + (cols * r + c)) = value;
}

double* M_ToRREF (double* matrix, int rows, int cols, int delimiter)
{
    int sizebytes = sizeof(double) * rows * cols;
    // allocate new memory for the row reduced matrix
    double* rref = E_Malloc(sizebytes, M_ToRREF);
    // clone the original matrix to the new one for processing
    E_Memcpy(rref, matrix, sizebytes);
    /* row reduction */
    int cpivot = 0;
    for (int r = 0; r < rows; ++r)
    {
        if (cpivot == delimiter) return NULL;
        int rpivot = r;
        /* find pivot */
        while(!M_Get(rref, cols, rpivot, cpivot))
        {
            // reached the end of the matrix, and there is a zero-column.
            // this means the matrix is non-invertible, so return immediately
            if (rpivot == rows - 1 && cpivot == delimiter - 1) return NULL;
            // the column has no non-zero entries, advance to the next one
            else if (rpivot == rows - 1) { ++cpivot; rpivot = r; }
            // look for a pivot in the row below in the current column
            else ++rpivot;
        }
        /* swap current row with the row that has the pivot,
         * if it is in another row
         */
        if (r != rpivot)
        {
            for (int c = 0; c < cols; ++c)
            {
                double temp = M_Get(rref, cols, r, c);
                M_Set(rref, M_Get(rref, cols, rpivot, c), cols, r, c);
                M_Set(rref, temp, cols, rpivot, c);
            }
            rpivot = r;
        }
        /* normalize the current row */
        double pivot = M_Get(rref, cols, rpivot, cpivot);
        if (pivot != 1)
        {
            for (int c = 0; c < cols; ++c)
            {
                double target = M_Get(rref, cols, rpivot, c);
                M_Set(rref, target / pivot, cols, rpivot, c);
            }
        }
        /* reduce rows in the current column */
        for (int reducerow = 0; reducerow < rows; ++reducerow)
        {
            if (reducerow == rpivot) continue; // skip if the pivot row
            double scalepivot = 0 - M_Get(rref, cols, reducerow, cpivot);
            if (!scalepivot) continue; // skip if the scaling is zero
            /* reduce the entire row */
            for (int c = cpivot; c < cols; ++c)
            {
                double source = M_Get(rref, cols, rpivot, c);
                double target = M_Get(rref, cols, reducerow, c);
                M_Set(rref, target + source * scalepivot, cols, reducerow, c);
            }
        }
        ++cpivot; // done reducing the current column, advance to the next
    }
    return rref;
}

double* M_Invert (double* matrix, int rows)
{
    int cols = rows + rows;
    // allocate new memory for the augmented matrix for it to be row reduced
    double* augmented = E_Malloc(sizeof(double) * rows * cols , M_Invert);
    /* create an augmented matrix for inversion */
    for (int r = 0; r < rows; ++r)
    {
        for (int c = 0; c < cols; ++c)
        {
            // if inside augmented matrix
            if (c >= rows) M_Set(augmented, c - rows == r ? 1 : 0, cols, r, c);
            else M_Set(augmented, M_Get(matrix, rows, r, c), cols, r, c);
        }
    }
    double* rref = M_ToRREF(augmented, rows, cols, rows);
    // allocate new memory for the inverted matrix
    double* inverted = E_Malloc(sizeof(double) * rows * rows , M_Invert);
    /* extract the inverted matrix from the rref'ed augmented matrix */
    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < rows; ++c)
            M_Set(inverted, M_Get(rref, cols, r, rows + c), rows, r, c);
    E_Free(augmented);
    E_Free(rref);
    return inverted;
}


void M_Dump (double* matrix, int rows, int cols)
{
    printf("Matrix @%p:\n\n", matrix);

    if (!matrix) return;

    for (int r = 0; r < rows; ++r)
    {
        for (int c = 0; c < cols; ++c)
        {
            printf("%.3f", M_Get(matrix, cols, r, c));
            if (c < cols - 1) printf(" ");
        }
        printf("\n");
    }
}