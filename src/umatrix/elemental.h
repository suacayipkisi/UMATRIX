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

#ifndef UM_ASSERT
#include <assert.h>
#define UM_ASSERT assert
#endif

typedef struct U_Imag{
    double real;
    double imag;
} U_Imag;

typedef struct U_Matrix{
    size_t rows;
    size_t cols;
    size_t stride;
    double *data;
} U_Matrix;

typedef struct U_Im_Matrix {
    size_t rows;
    size_t cols;
    size_t stride;
    U_Imag *data;
} U_Im_Matrix;

// Returns a pointer to element (i, j). Bounds are checked via UM_ASSERT
// (disabled when NDEBUG is defined, so release builds pay no cost).
// Kept static inline in the header so every call site can be inlined.
static inline double *um_at(U_Matrix m, size_t i, size_t j){
    UM_ASSERT(i < m.rows && j < m.cols);
    return &m.data[i * m.stride + j];
}

static inline U_Imag *um_im_at(U_Im_Matrix m, size_t i, size_t j){
    UM_ASSERT(i < m.rows && j < m.cols);
    return &m.data[i * m.stride + j];
}

U_Matrix  um_alloc(size_t rows, size_t cols);
U_Im_Matrix um_im_alloc(size_t rows, size_t cols);
void um_free_null(U_Matrix m);
void um_im_free_null(U_Im_Matrix m);
void um_fill(U_Matrix m, double value);
void um_im_fill(U_Im_Matrix m, double real_val, double imag_val);
void um_print(U_Matrix m, const char *name);
void um_im_print(U_Im_Matrix m, const char *name);
#define UM_PRINT(m) um_print((m), #m)
#define UM_IM_PRINT(m) um_im_print((m), #m);

// element wise operations: adding, substracting and scaling with a value
void um_add(U_Matrix dst, U_Matrix a, U_Matrix b);
void um_sub(U_Matrix dst, U_Matrix a, U_Matrix b);
void um_scale(U_Matrix dst, U_Matrix a, double k);

// --- Step 3: multiplication (TODO) ------------------------------------------
// void um_mul(U_Matrix dst, U_Matrix a, U_Matrix b);

// --- Step 4: transpose / identity (TODO) ------------------------------------
// void um_transpose(U_Matrix dst, U_Matrix a);
// void um_identity(U_Matrix m);

// --- Step 5: Gaussian elimination (TODO) ------------------------------------
// double um_det(U_Matrix a);
// int    um_inverse(U_Matrix dst, U_Matrix a);   // returns 0 if singular
// int    um_solve(U_Matrix x, U_Matrix a, U_Matrix b); // solves a * x = b

#endif // UU_MatrixRIX_H_
