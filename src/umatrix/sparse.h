// SPDX-License-Identifier: MIT
// Copyright (c) 2026 suacayipkisi

#ifndef SPARSE_H_
#define SPARSE_H_

#include "elemental.h"
#include <stddef.h>

typedef struct U_Sparse_Value{
    size_t row_index;
    size_t col_index;
    double value;
} U_Sparse_Value;

typedef struct U_Cmpx_Sparse_Value{
    size_t row_index;
    size_t col_index;
    double complex value;
} U_Cmpx_Sparse_Value;

/* use if most part of the real matrix is zero, only holds non-zero values */
typedef struct U_Matrix_Sparse{
    size_t rows;
    size_t cols;
    size_t stride;
    size_t data_num; // how many value in that matrix is not zero, also size of the data
    size_t container_size; // how many is the max size of data
    U_Sparse_Value *data;
} U_Matrix_Sparse;

/* use if most part of the complex matrix is zero, only holds non-zero values */
typedef struct U_Cmpx_Matrix_Sparse{
    size_t rows;
    size_t cols;
    size_t stride;
    size_t data_num; // how many value in that matrix is not zero, also size of the data
    size_t container_size; // how many is the max size of data
    U_Cmpx_Sparse_Value *data;
} U_Cmpx_Matrix_Sparse;

// check is it NULL. if NULL -> this (i, j) is still zero in sparse real matrix
static inline U_Sparse_Value *um_at_sparse(U_Matrix_Sparse *m, size_t i, size_t j){
    size_t row_num = m->rows;
    size_t col_num = m->cols;
    UM_ASSERT(i < row_num && j < col_num);
    for(ptrdiff_t index = 0; index < (ptrdiff_t)m->data_num; ++index){
        if(m->data[index].row_index == i && m->data[index].col_index == j){
            return &m->data[index];
        }
    }
    return NULL;
}

// check is it NULL. if NULL -> this (i, j) is still zero in sparse complex matrix
static inline U_Cmpx_Sparse_Value *um_cmpx_at_sparse(U_Cmpx_Matrix_Sparse *m, size_t i, size_t j){
    size_t row_num = m->rows;
    size_t col_num = m->cols;
    UM_ASSERT(i < row_num && j < col_num);
    for(ptrdiff_t index = 0; index < (ptrdiff_t)m->data_num; ++index){
        if(m->data[index].row_index == i && m->data[index].col_index == j){
            return &m->data[index];
        }
    }
    return NULL;
}

// setting a matrix
U_Matrix_Sparse um_init_sparse(size_t rows, size_t cols);
U_Cmpx_Matrix_Sparse um_cmpx_init_sparse(size_t rows, size_t cols);

// reserving the matrix 
bool um_sparse_reserve(U_Matrix_Sparse *m, size_t new_container_size);
bool um_cmpx_sparse_reserve(U_Cmpx_Matrix_Sparse *m, size_t new_container_size);

// value assignation into a matrix (only one value), check is it true
void um_set_val_sparse(U_Matrix_Sparse *m, size_t i, size_t j, double value);
void um_cmpx_set_val_sparse(U_Cmpx_Matrix_Sparse *m, size_t i, size_t j, double complex value);

// release and delete the matrix
void um_free_null_sparse(U_Matrix_Sparse *m);
void um_cmpx_free_null_sparse(U_Cmpx_Matrix_Sparse *m);

void um_print_sparse(U_Matrix_Sparse *m, const char *name);
void um_cmpx_print_sparse(U_Cmpx_Matrix_Sparse *m, const char *name);
#define UM_PRINT_SPARSE(m) um_print_sparse((m), #m)
#define UM_CMPX_PRINT_SPARSE(m) um_cmpx_print_sparse((m), #m);



#endif // SPARSE_H_
