// SPDX-License-Identifier: MIT
// Copyright (c) 2026 suacayipkisi

#include "sparse.h"
#include "umatrix/elemental.h"
#include <stddef.h>
#include <stdint.h>

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
    
    U_Sparse_Value *temporary = realloc(m->data, new_container_size * sizeof(*m->data));
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
    
    U_Cmpx_Sparse_Value *temporary = realloc(m->data, new_container_size * sizeof(*m->data));
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
    U_Sparse_Value *val_adress = um_at_sparse(m, i, j);
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
        m->data[m->data_num++] = (U_Sparse_Value){i, j, value}; // index start with zero, so it fills an empty place
    }
    UM_ASSERT(can_return == 1);
}

/* if value at this (i, j) in complex matrix, reallocate the data array 1,5x if it is full, also check does it return true */
void um_cmpx_set_val_sparse(U_Cmpx_Matrix_Sparse *m, size_t i, size_t j, double complex value){
    int can_return = 1;
    UM_ASSERT(i < m->rows && j < m->cols);
    U_Cmpx_Sparse_Value *val_adress = um_cmpx_at_sparse(m, i, j);
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
        m->data[m->data_num++] = (U_Cmpx_Sparse_Value){i, j, value}; // index start with zero, so it fills an empty place
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

void um_fill_sparse(U_Matrix_Sparse *m, double value);

void um_cmpx_fill_sparse(U_Cmpx_Matrix_Sparse *m, double complex value);

void um_print_sparse(U_Matrix_Sparse *m, const char *name);

void um_cmpx_print_sparse(U_Cmpx_Matrix_Sparse *m, const char *name);

#define UM_PRINT_SPARSE(m) um_print_sparse((m), #m)

#define UM_CMPX_PRINT_SPARSE(m) um_cmpx_print_sparse((m), #m);
