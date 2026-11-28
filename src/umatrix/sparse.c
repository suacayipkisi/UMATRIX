// SPDX-License-Identifier: MIT
// Copyright (c) 2026 suacayipkisi

#include "sparse.h"
#include "umatrix/elemental.h"
#include <stddef.h>
#include <stdint.h>
#include <string.h>

// Allocator hooks. Override at library build time, e.g.
//   make CPPFLAGS='-DUM_MALLOC=my_malloc -DUM_FREE=my_free'
#if !defined(UM_MALLOC) || !defined(UM_FREE)
#include <stdlib.h>
#endif
#ifndef UM_MALLOC
#define UM_MALLOC malloc
#endif
#ifndef UM_CALLOC
#define UM_CALLOC calloc
#endif
#ifndef UM_MEMSET
#define UM_MEMSET memset
#endif
#ifndef UM_FREE
#define UM_FREE free
#endif 

/* allocation of a real number sparse matrix (3x3 and empty) */
U_Matrix_Sparse um_init_sparse(size_t rows, size_t cols){
    U_Matrix_Sparse m;
    m.rows = rows;
    m.cols = cols;
    m.stride = cols;
    m.data_num = 0;
    m.container_size = 9;
    m.data = UM_MALLOC(3 * 3 * sizeof(*m.data));
    UM_ASSERT(m.data != NULL);
    return m;
}

/* allocation of a complex number sparse matrix (3x3 and empty) */
U_Cmpx_Matrix_Sparse um_cmpx_init_sparse(size_t rows, size_t cols){
    U_Cmpx_Matrix_Sparse m;
    m.rows = rows;
    m.cols = cols;
    m.stride = cols;
    m.data_num = 0;
    m.container_size = 9;
    m.data = UM_MALLOC(3 * 3 * sizeof(*m.data));
    UM_ASSERT(m.data != NULL);
    return m;
}

// reserve a defined real matrix with a size (if given size is smaller, nothing changes)
bool um_sparse_reserve(U_Matrix_Sparse *m, size_t new_container_size){
    if(new_container_size <= m->container_size){
        return true;
    }
    if(new_container_size > SIZE_MAX / sizeof(*m->data)){
        return false;
    }
    
    U_Sparse_Triplet *temporary = realloc(m->data, new_container_size * sizeof(*m->data));
    if(!temporary){
        return false;
    }
    
    m->data = temporary;
    m->container_size = new_container_size;
    return true;
}

// reserve a defined complex matrix with a size (if given size is smaller, nothing changes)
bool um_cmpx_sparse_reserve(U_Cmpx_Matrix_Sparse *m, size_t new_container_size){
    if(new_container_size <= m->container_size){
        return true;
    }
    if(new_container_size > SIZE_MAX / sizeof(*m->data)){
        return false;
    }
    
    U_Cmpx_Sparse_Triplet *temporary = realloc(m->data, new_container_size * sizeof(*m->data));
    if(!temporary){
        return false;
    }
    
    m->data = temporary;
    m->container_size = new_container_size;
    return true;
}

/* if value at this (i, j) in real matrix, reallocate the data array if it is full, also check does it return true */
void um_set_val_sparse(U_Matrix_Sparse *m, size_t i, size_t j, double value){
    int can_return = 1;
    UM_ASSERT(i < m->rows && j < m->cols);
    U_Sparse_Triplet *val_adress = um_at_sparse(m, i, j);
    if(!val_adress){
        m->data[m->data_num + 1].value = value;
    } else {
        if(m->data_num == m->container_size){
            size_t new_container_size = (m->container_size < 2) ? 4 : m->container_size + (m->container_size / 2);
            if(new_container_size < m->container_size){
                can_return = 0;
            }
            if(!um_sparse_reserve(m, new_container_size)){
                can_return = 0;
            }
        }
        m->data[m->data_num++] = (U_Sparse_Triplet){i, j, value}; // index start with zero, so it fills an empty place
    }
    UM_ASSERT(can_return == 1);
}

/* if value at this (i, j) in complex matrix, reallocate the data array 1,5x if it is full, also check does it return true */
void um_cmpx_set_val_sparse(U_Cmpx_Matrix_Sparse *m, size_t i, size_t j, double complex value){
    int can_return = 1;
    UM_ASSERT(i < m->rows && j < m->cols);
    U_Cmpx_Sparse_Triplet *val_adress = um_cmpx_at_sparse(m, i, j);
    if(!val_adress){
        m->data[m->data_num + 1].value = value;
    } else {
        if(m->data_num == m->container_size){
            size_t new_container_size = (m->container_size < 2) ? 4 : m->container_size + (m->container_size / 2);
            if(new_container_size < m->container_size){
                can_return = 0;
            }
            if(!um_cmpx_sparse_reserve(m, new_container_size)){
                can_return = 0;
            }
        }
        m->data[m->data_num++] = (U_Cmpx_Sparse_Triplet){i, j, value}; // index start with zero, so it fills an empty place
    }
    UM_ASSERT(can_return == 1);
}

// free null a real number sparse matrix
void um_free_null_sparse(U_Matrix_Sparse *m){
    UM_FREE(m->data);
    m->data = NULL;
    m->container_size = 0;
    m->data_num = 0;
}

// free null a complex number sparse matrix
void um_cmpx_free_null_sparse(U_Cmpx_Matrix_Sparse *m){
    UM_FREE(m->data);
    m->data = NULL;
    m->container_size = 0;
    m->data_num = 0;
}

// row major, getting real matrix ready to faster calculations
/* two pass counting sort */
void um_sparse_give_order(U_Matrix_Sparse *m){
    size_t rows = m->rows;
    size_t cols = m->cols;
    size_t nnz = m->data_num; // nubebr of not zero
    UM_ASSERT(!(nnz < 2 || rows == 0 || cols == 0));
    
    size_t max_dim = (rows > cols) ? rows : cols;
    size_t *count = UM_CALLOC(max_dim + 1, sizeof(size_t));
    U_Sparse_Triplet *tmp = UM_MALLOC(nnz * sizeof(U_Sparse_Triplet));
    if(!count || !tmp){
        free(count);
        free(tmp);
        UM_ASSERT(false);
    }
    
    /* Pass 1: Stable counting sort by column (arr -> tmp) */
    for (size_t i = 0; i < nnz; i++) {
        count[m->data[i].col_index + 1]++;
    }
    for (size_t c = 0; c < cols; c++) {
        count[c + 1] += count[c];
    }
    for (size_t i = 0; i < nnz; i++) {
        tmp[count[m->data[i].col_index]++] = m->data[i];
    }

    /* Reset count buffer for Pass 2 */
    UM_MEMSET(count, 0, (rows + 1) * sizeof(size_t));

    /* Pass 2: Stable counting sort by row (tmp -> arr) */
    for (size_t i = 0; i < nnz; i++) {
        count[tmp[i].row_index + 1]++;
    }
    for (size_t r = 0; r < rows; r++) {
        count[r + 1] += count[r];
    }
    for (size_t i = 0; i < nnz; i++) {
        m->data[count[tmp[i].row_index]++] = tmp[i];
    }

    free(tmp);
    free(count);
}

// row major, getting complex matrix ready to faster calculations
/* two pass counting sort */
void um_cmpx_sparse_give_order(U_Cmpx_Matrix_Sparse *m){
    size_t rows = m->rows;
    size_t cols = m->cols;
    size_t nnz = m->data_num; // nubebr of not zero
    UM_ASSERT(!(nnz < 2 || rows == 0 || cols == 0));
    
    size_t max_dim = (rows > cols) ? rows : cols;
    size_t *count = UM_CALLOC(max_dim + 1, sizeof(size_t));
    U_Cmpx_Sparse_Triplet *tmp = UM_MALLOC(nnz * sizeof(U_Cmpx_Sparse_Triplet));
    if(!count || !tmp){
        free(count);
        free(tmp);
        UM_ASSERT(false);
    }
    
    /* Pass 1: Stable counting sort by column (arr -> tmp) */
    for (size_t i = 0; i < nnz; i++) {
        count[m->data[i].col_index + 1]++;
    }
    for (size_t c = 0; c < cols; c++) {
        count[c + 1] += count[c];
    }
    for (size_t i = 0; i < nnz; i++) {
        tmp[count[m->data[i].col_index]++] = m->data[i];
    }

    /* Reset count buffer for Pass 2 */
    UM_MEMSET(count, 0, (rows + 1) * sizeof(size_t));

    /* Pass 2: Stable counting sort by row (tmp -> arr) */
    for (size_t i = 0; i < nnz; i++) {
        count[tmp[i].row_index + 1]++;
    }
    for (size_t r = 0; r < rows; r++) {
        count[r + 1] += count[r];
    }
    for (size_t i = 0; i < nnz; i++) {
        m->data[count[tmp[i].row_index]++] = tmp[i];
    }

    free(tmp);
    free(count);
}
