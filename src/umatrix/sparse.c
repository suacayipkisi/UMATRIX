// SPDX-License-Identifier: MIT
// Copyright (c) 2026 suacayipkisi

#include "sparse.h"
#include "umatrix/elemental.h"

// Allocator hooks. Override at library build time, e.g.
//   make CPPFLAGS='-DUM_MALLOC=my_malloc -DUM_FREE=my_free'
#if !defined(UM_MALLOC) || !defined(UM_FREE)
#include <stdlib.h>
#endif
#ifndef UM_MALLOC
#define UM_MALLOC malloc
#endif
#ifndef UM_FREE
#define UM_FREE free
#endif

/* allocation of a real number sparse matrix */
U_Matrix_Sparse um_alloc_sparse(size_t rows, size_t cols){
    U_Matrix_Sparse m;
    m.rows = rows;
    m.cols = cols;
    m.stride = cols;
    m.data = UM_MALLOC(sizeof(*m.data) * rows * cols);
    UM_ASSERT(m.data != NULL);
    return m;
}

U_Cmpx_Matrix_Sparse um_cmpx_alloc_sparse(size_t rows, size_t cols){
    U_Cmpx_Matrix_Sparse m;
    m.rows = rows;
    m.cols = cols;
    m.stride = cols;
    m.data = UM_MALLOC(sizeof(*m.data) * rows * cols);
    UM_ASSERT(m.data != NULL);
    return m;
}

void um_free_null_sparse(U_Matrix_Sparse m){
    UM_FREE(m.data);
    if(m.data != NULL){
        m.data = NULL;
    }
}

void um_cmpx_free_null_sparse(U_Cmpx_Matrix_Sparse m){
    UM_FREE(m.data);
    if(m.data != NULL){
        m.data = NULL;
    }
}

void um_fill_sparse(U_Matrix_Sparse m, double value);

void um_cmpx_fill_sparse(U_Cmpx_Matrix_Sparse m, double complex value);

void um_print_sparse(U_Matrix_Sparse m, const char *name);

void um_cmpx_print_sparse(U_Cmpx_Matrix_Sparse m, const char *name);

#define UM_PRINT_SPARSE(m) um_print_sparse((m), #m)

#define UM_CMPX_PRINT_SPARSE(m) um_cmpx_print_sparse((m), #m);



/* if value at this (i, j) in matrix, expand the data array if  */
void um_set_val_sparse(U_Matrix_Sparse m, size_t i, size_t j, double value){
    UM_ASSERT(i < m.rows && j < m.cols);
    U_Sparse_Value *val_adress = um_at_sparse(m, i, j);
    if(!val_adress){
        m.data[m.data_num + 1].value = value;
    } else {
        *val_adress = (U_Sparse_Value){i, j, value};
    }
}

/* if value at this (i, j) in matrix, expand the data array if  */
void um_cmpx_set_val_sparse(U_Cmpx_Matrix_Sparse m, size_t i, size_t j, double complex value){
    UM_ASSERT(i < m.rows && j < m.cols);
    U_Cmpx_Sparse_Value *val_adress = um_at_sparse(m, i, j);
    if(!val_adress){
        m.data[m.data_num + 1].value = value;
    } else {
        *val_adress = (U_Cmpx_Sparse_Value){i, j, value};
    }
}

