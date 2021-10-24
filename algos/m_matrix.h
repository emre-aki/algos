/*
 *  m_matrix.h
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

#ifndef m_matrix_h

#define m_matrix_h_M_ToRREF M_ToRREF
#define m_matrix_h_M_Invert M_Invert
#define m_matrix_h_M_Dump M_Dump

double* M_ToRREF (double* matrix, int rows, int cols, int delimiter);
double* M_Invert (double* matrix, int rows);
void M_Dump (double* matrix, int rows, int cols);

#endif
