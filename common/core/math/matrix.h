/**
 * Matrix math library.
 */

#pragma once

#include "math_util.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STR_LENGTH 500

/**
 * A Matrix instance, using externally allocated
 * memory to store indices.
 */
typedef struct
{
    /** Pointer to the backing buffer. */
    double* data;

    /** Number of rows. */
    const size_t rows;

    /** Number of columns. */
    const size_t cols;
} Matrix;

/**
 * Basic Matrix Constructor.
 * 
 * @param name variable name for the matrix.
 * @param rows number of matrix rows.
 * @param cols number of matrix columns.
 * @param ... varargs for bracket initialization of backing buffer.
 */
#define MATRIX(name, rows, cols, ...)              \
    double name##_buf[rows][cols] = {__VA_ARGS__}; \
    Matrix name = {(double*)name##_buf, rows, cols}

/**
 * Typed Matrix Constructor.
 * 
 * @param type the additional typing to give the backing buffer
 *             and matrix struct.
 */
#define MATRIX_T(type, name, rows, cols, ...)           \
    type double name##_buf[rows][cols] = {__VA_ARGS__}; \
    type Matrix name = {(double*)name##_buf, rows, cols}

/**
 * Allows construction of a matrix with an uninitialized
 * buffer using const variable row/col parameters.
 */
#define VAR_MATRIX(name, rows, cols) \
    double name##_buf[rows][cols];   \
    Matrix name = {(double*)name##_buf, rows, cols}

/**
 * Gets (dereferences) the given index of the matrix.
 * 
 * @param mat a pointer to the matrix.
 * @param row the row number.
 * @param col the column number.
 */
#define MAT_GET(mat, row, col) (mat)->data[((row) * (mat)->cols) + (col)]

/**
 * Copies a matrix from the source to the destination.
 * The two matrices must have identical dimensions.
 * 
 * @param[in] src the source matrix.
 * @param[out] dest the destination matrix.
 * @return a pointer to the destination matrix for usage convenience.
 */
Matrix* matrix_copy(Matrix* src, Matrix* dest);

/**
 * Creates an augmented matrix that is 2 times wider than a given
 * matrix, and copies the original contents into it.
 */
#define MAT_AUGMENT(mat, name)                      \
    VAR_MATRIX(name, (mat)->rows, (mat)->cols * 2); \
    matrix_copy(mat, &name)

/**
 * Multiples two matrices. Multiplication is NOT commutative.
 * 
 * @param[in] A the first matrix.
 * @param[in] B the second matrix.
 * @param[out] C the output matrix.
 * @return a pointer to matrix C (the result) for usage convenience.
 */
Matrix* matrix_multiply(Matrix* A, Matrix* B, Matrix* C);

/**
 * Transposes a matrix. 
 * 
 * @param[in] A the first matrix.
 * @param[out] AT the output matrix.
 * @return a pointer to matrix AT (the result) for usage convenience.
 */
Matrix* matrix_transpose(Matrix* A, Matrix* AT);

/**
 * Adds two matrices.
 * 
 * @param[in] A the first matrix.
 * @param[in] B the second matrix.
 * @param[out] C the output matrix.
 * @return a pointer to matrix C (the result) for usage convenience.
 */
Matrix* matrix_add(Matrix* A, Matrix* B, Matrix* C);

/**
 * Subtracts two matrices.
 * 
 * @param[in] A the first matrix.
 * @param[in] B the second matrix.
 * @param[out] C the output matrix.
 * @return a pointer to matrix C (the result) for usage convenience.
 */
Matrix* matrix_subtract(Matrix* A, Matrix* B, Matrix* C);

/**
 * Inverts a matrix.
 * 
 * @param[in] A the first matrix.
 * @param[out] AInv the output matrix.
 * @return a pointer to matrix AInv (the result) for usage convenience.
 */
Matrix* matrix_inverse(Matrix* A, Matrix* AInv);

/**
 * Compares two matrices for near equivalency.
 * 
 * @param[in] A the first matrix.
 * @param[in] B the second matrix.
 * @return True if the matrices are equivalent within a float threshold,
 *         otherwise false.
 */
bool matrix_compare(Matrix* A, Matrix* B);

/**
 * Creates a string representation of a matrix.
 * Written for immediate consumption or copying, not for storing
 * and accessing at a later time.
 * 
 * @param[in] mat the matrix.
 * @return a char pointer to the matrix representation.
 */
char* matrix_to_string(Matrix* mat);
