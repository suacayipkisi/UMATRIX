// SPDX-License-Identifier: MIT
// Copyright (c) 2026 suacayipkisi

// umatrix.h - experimental matrix library in pure C
//
// Public interface. Function bodies live in src/umatrix.c and are shipped
// as libumatrix.a; link with -lumatrix -lm.
//
// Conventions:
//   - Row-major storage in a single contiguous block.
//   - `stride` is the distance (in elements) between two consecutive rows.
//     For an owned matrix stride == cols; views may have stride > cols.
//   - Operations write into a caller-provided `dst` (the caller owns memory).

#ifndef UMATRIX_H_
#define UMATRIX_H_

#include <stddef.h>
#include <complex.h>

#ifndef UM_ASSERT
#include <assert.h>
#define UM_ASSERT assert
#endif

typedef struct U_Matrix{
    size_t rows;
    size_t cols;
    size_t stride; // for row major, equal to column
    double *data;
} U_Matrix;

typedef struct U_Cmpx_Matrix {
    size_t rows;
    size_t cols;
    size_t stride; // for row major, equal to column
    double complex *data;
} U_Cmpx_Matrix;

// Returns a pointer to element (i, j). Bounds are checked via UM_ASSERT
// (disabled when NDEBUG is defined, so release builds pay no cost).
// Kept static inline in the header so every call site can be inlined.
static inline double *um_at(U_Matrix m, size_t i, size_t j){
    UM_ASSERT(i < m.rows && j < m.cols);
    return &m.data[i * m.stride + j];
}

static inline double complex *um_cmpx_at(U_Cmpx_Matrix m, size_t i, size_t j){
    UM_ASSERT(i < m.rows && j < m.cols);
    return &m.data[i * m.stride + j];
}

U_Matrix  um_alloc(size_t rows, size_t cols);
U_Cmpx_Matrix um_cmpx_alloc(size_t rows, size_t cols);
void um_free_null(U_Matrix m);
void um_cmpx_free_null(U_Cmpx_Matrix m);
void um_fill(U_Matrix m, double value);
void um_cmpx_fill(U_Cmpx_Matrix m, double complex value);
void um_print(U_Matrix m, const char *name);
void um_cmpx_print(U_Cmpx_Matrix m, const char *name);
#define UM_PRINT(m) um_print((m), #m)
#define UM_CMPX_PRINT(m) um_cmpx_print((m), #m);

/* set value in a matrix */
void um_set_val(U_Matrix m, size_t i, size_t j, double value);
void um_cmpx_set_val(U_Cmpx_Matrix m, size_t i, size_t j, double complex value);

/* convert a real matrix to complex matrix */
void um_convert_to_cmpx(U_Cmpx_Matrix comp_mat, U_Matrix real_mat);

/* element wise operations: adding, subtracting and scaling with a value */
void um_add(U_Matrix dst, U_Matrix a, U_Matrix b);
void um_cmpx_add_real(U_Cmpx_Matrix dst, U_Cmpx_Matrix a, U_Matrix b);
void um_cmpx_add(U_Cmpx_Matrix dst, U_Cmpx_Matrix a, U_Cmpx_Matrix b);

void um_sub(U_Matrix dst, U_Matrix a, U_Matrix b);
void um_cmpx_sub_real(U_Cmpx_Matrix dst, U_Cmpx_Matrix a, U_Matrix b);
void um_real_sub_cmpx(U_Cmpx_Matrix dst, U_Matrix a, U_Cmpx_Matrix b);
void um_cmpx_sub(U_Cmpx_Matrix dst, U_Cmpx_Matrix a, U_Cmpx_Matrix b);

void um_scale(U_Matrix dst, U_Matrix a, double k);
void um_real_scale_cmpx(U_Cmpx_Matrix dst, U_Matrix a, double complex k);
void um_cmpx_scale_real(U_Cmpx_Matrix dst, U_Cmpx_Matrix a, double k);
void um_cmpx_scale_cmpx(U_Cmpx_Matrix dst, U_Cmpx_Matrix a, double complex k);

/* basic matrix multiplication */
void um_mul(U_Matrix dst, U_Matrix a, U_Matrix b);
void um_cmpx_mul_real(U_Matrix dst, U_Matrix a, U_Matrix b);
void um_real_mul_cmpx(U_Matrix dst, U_Matrix a, U_Matrix b);
void um_cmpx_mul_cmpx(U_Matrix dst, U_Matrix a, U_Matrix b);

// transpose / identity
// void um_transpose(U_Matrix dst, U_Matrix a);
// void um_identity(U_Matrix m);

// Gaussian elimination
// double um_det(U_Matrix a);
// int    um_inverse(U_Matrix dst, U_Matrix a);   // returns 0 if singular
// int    um_solve(U_Matrix x, U_Matrix a, U_Matrix b); // solves a * x = b

#endif // UU_MatrixRIX_H_
