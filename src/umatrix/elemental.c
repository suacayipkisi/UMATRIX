// SPDX-License-Identifier: MIT
// Copyright (c) 2026 suacayipkisi

#include "elemental.h"

#include <complex.h>
#include <stddef.h>
#include <stdio.h>

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

/* allocation of a real number matrix */
U_Matrix um_alloc(size_t rows, size_t cols){
    U_Matrix m;
    m.rows = rows;
    m.cols = cols;
    m.stride = cols;
    m.data = UM_MALLOC(sizeof(*m.data) * rows * cols);
    UM_ASSERT(m.data != NULL);
    return m;
}

/* allocation of a complex number matrix */
U_Cmpx_Matrix um_cmpx_alloc(size_t rows, size_t cols){
    U_Cmpx_Matrix m;
    m.rows = rows;
    m.cols = cols;
    m.stride = cols;
    m.data = UM_MALLOC(sizeof(*m.data) * rows * cols);
    UM_ASSERT(m.data != NULL);
    return m;
}

/* free the matrix and set null its pointer */
/* using is strictly required after you have done with matrix */
void um_free_null(U_Matrix *m){
    UM_FREE(m->data);
    if(m->data != NULL){
        m->data = NULL;
    }
}

/* free the matrix and set null its pointer */
/* using is strictly required after you have done with matrix */
void um_cmpx_free_null(U_Cmpx_Matrix *m){
    UM_FREE(m->data);
    if(m->data != NULL){
        m->data = NULL;
    }
}

/* fill the matrix with a real value */
void um_fill(U_Matrix *m, double value){
    for (size_t i = 0; i < m->rows; ++i) {
        for (size_t j = 0; j < m->cols; ++j) {
            *um_at(m, i, j) = value; 
        }
    }
}

/* fill the matrix with a complex value */
void um_cmpx_fill(U_Cmpx_Matrix *m, double complex value){
    for (size_t i = 0; i < m->rows; ++i) {
        for (size_t j = 0; j < m->cols; ++j) {
            *um_cmpx_at(m, i, j) = value;
        }
    }
}

/* print the real number matrix into terminal */
void um_print(U_Matrix *m, const char *name){
    printf("%s = [\n", name);
    for (size_t i = 0; i < m->rows; ++i) {
        printf("    ");
        for (size_t j = 0; j < m->cols; ++j) {
            printf("%10.4f ", *um_at(m, i, j));
        }
        printf("\n");
    }
    printf("]\n");
}

/* print the complex number matrix into terminal */
void um_cmpx_print(U_Cmpx_Matrix *m, const char *name){
    printf("%s = [\n", name);
    for (size_t i = 0; i < m->rows; ++i) {
        printf("    ");
        for (size_t j = 0; j < m->cols; ++j) {
            double complex imag_num = *um_cmpx_at(m, i, j);
            printf("%10.4f + %-10.4f ", creal(imag_num), cimag(imag_num));
        }
        printf("\n");
    }
    printf("]\n");
}

/* assign a value at (i, j) in real number matrix */
void um_set_val(U_Matrix *m, size_t i, size_t j, double value){
    *um_at(m, i, j) = value;
}

/* assign a complex value at (i, j) in complex number matrix */
void um_cmpx_set_val(U_Cmpx_Matrix *m, size_t i, size_t j, double complex value){
    *um_cmpx_at(m, i, j) = value;
}

/* convert a real number matrix to a complex number matrix */
/* for basic operations in matrices (addition, subtraction, multiplication and scaling) 
    you don't need to use that because builtin functions for these operations are exist */
void um_convert_to_cmpx(U_Cmpx_Matrix *comp_mat, U_Matrix *real_mat){
    size_t col = comp_mat->cols;
    size_t row = comp_mat->rows;
    UM_ASSERT((comp_mat->cols == real_mat->cols) && (comp_mat->rows == real_mat->rows));
    for(size_t i = 0; i < row; ++i){
        for(size_t j = 0; j < col; ++j){
            size_t index = i * comp_mat->stride + j;
            comp_mat->data[index] = real_mat->data[index];
        }
    }
}

/* adding two real number matrices */
void um_add(U_Matrix *dst, U_Matrix *a, U_Matrix *b){
    size_t col = dst->cols;
    size_t row = dst->rows;
    UM_ASSERT((a->cols == b->cols == col) && (a->rows == b->rows == row));
    for(size_t i = 0; i < row; ++i){
        for(size_t j = 0; j < col; ++j){
            size_t index = i * dst->stride + j;
            dst->data[index] = (a->data[index] + b->data[index]);
        }
    }
}
/* adding a complex number matrix and a real number matrix */
void um_cmpx_add_real(U_Cmpx_Matrix *dst, U_Cmpx_Matrix *a, U_Matrix *b){
    size_t col = dst->cols;
    size_t row = dst->rows;
    UM_ASSERT((a->cols == b->cols == col) && (a->rows == b->rows == row));
    for(size_t i = 0; i < row; ++i){
        for(size_t j = 0; j < col; ++j){
            size_t index = i * dst->stride + j;
            dst->data[index] = (a->data[index] + b->data[index]);
        }
    }
}

/* adding two complex number matrices */
void um_cmpx_add(U_Cmpx_Matrix *dst, U_Cmpx_Matrix *a, U_Cmpx_Matrix *b){
    size_t col = dst->cols;
    size_t row = dst->rows;
    UM_ASSERT((a->cols == b->cols == col) && (a->rows == b->rows == row));
    for(size_t i = 0; i < row; ++i){
        for(size_t j = 0; j < col; ++j){
            size_t index = i * dst->stride + j;
            dst->data[index] = (a->data[index] + b->data[index]);
        }
    }
}

/* substracting two real number matrices */
void um_sub(U_Matrix *dst, U_Matrix *a, U_Matrix *b){
    size_t col = dst->cols;
    size_t row = dst->rows;
    UM_ASSERT((a->cols == b->cols == col) && (a->rows == b->rows == row));
    for(size_t i = 0; i < row; ++i){
        for(size_t j = 0; j < col; ++j){
            size_t index = i * dst->stride + j;
            dst->data[index] = (a->data[index] - b->data[index]);
        }
    }
}

void um_cmpx_sub_real(U_Cmpx_Matrix *dst, U_Cmpx_Matrix *a, U_Matrix *b){
    size_t col = dst->cols;
    size_t row = dst->rows;
    UM_ASSERT((a->cols == b->cols == col) && (a->rows == b->rows == row));
    for(size_t i = 0; i < row; ++i){
        for(size_t j = 0; j < col; ++j){
            size_t index = i * dst->stride + j;
            dst->data[index] = (a->data[index] - b->data[index]);
        }
    }
}

void um_real_sub_cmpx(U_Cmpx_Matrix *dst, U_Matrix *a, U_Cmpx_Matrix *b){
    size_t col = dst->cols;
    size_t row = dst->rows;
    UM_ASSERT((a->cols == b->cols == col) && (a->rows == b->rows == row));
    for(size_t i = 0; i < row; ++i){
        for(size_t j = 0; j < col; ++j){
            size_t index = i * dst->stride + j;
            dst->data[index] = (a->data[index] - b->data[index]);
        }
    }
}

/* substracting two real number matrices */
void um_cmpx_sub(U_Cmpx_Matrix *dst, U_Cmpx_Matrix *a, U_Cmpx_Matrix *b){
    size_t col = dst->cols;
    size_t row = dst->rows;
    UM_ASSERT((a->cols == b->cols == col) && (a->rows == b->rows == row));
    for(size_t i = 0; i < row; ++i){
        for(size_t j = 0; j < col; ++j){
            size_t index = i * dst->stride + j;
            dst->data[index] = (a->data[index] - b->data[index]);
        }
    }
}

void um_scale(U_Matrix *dst, U_Matrix *a, double k){
    size_t col = dst->cols;
    size_t row = dst->rows;
    UM_ASSERT((a->cols == col) && (a->rows == row));
    for(size_t i = 0; i < row; ++i){
        for(size_t j = 0; j < col; ++j){
            dst->data[i * dst->stride + j] *= k;
        }
    }
}

void um_real_scale_cmpx(U_Cmpx_Matrix *dst, U_Matrix *a, double complex k){
    size_t col = dst->cols;
    size_t row = dst->rows;
    UM_ASSERT((a->cols == col) && (a->rows == row));
    for(size_t i = 0; i < row; ++i){
        for(size_t j = 0; j < col; ++j){
            dst->data[i * dst->stride + j] *= k;
        }
    }
}

void um_cmpx_scale_real(U_Cmpx_Matrix *dst, U_Cmpx_Matrix *a, double k){
    size_t col = dst->cols;
    size_t row = dst->rows;
    UM_ASSERT((a->cols == col) && (a->rows == row));
    for(size_t i = 0; i < row; ++i){
        for(size_t j = 0; j < col; ++j){
            dst->data[i * dst->stride + j] *= k;
        }
    }
}

void um_cmpx_scale_cmpx(U_Cmpx_Matrix *dst, U_Cmpx_Matrix *a, double complex k){
    size_t col = dst->cols;
    size_t row = dst->rows;
    UM_ASSERT((a->cols == col) && (a->rows == row));
    for(size_t i = 0; i < row; ++i){
        for(size_t j = 0; j < col; ++j){
            dst->data[i * dst->stride + j] *= k;
        }
    }
}

/* multiplication of two real number matrices. (i,j) x (k,l) => should satisfy j == k and result mush be (i,l) */
void um_mul(U_Matrix *dst, U_Matrix *a, U_Matrix *b){
    size_t col = dst->cols;
    size_t row = dst->rows;
    UM_ASSERT((a->cols == b->rows) && (dst->rows == a->rows) && (dst->cols == b->cols));
    for(size_t i = 0; i < row; ++i){
        for(size_t j = 0; j < col; ++j){
            double temp_sum = 0;
            for(size_t k = 0; k < a->cols; ++k){
                temp_sum += a->data[i * a->stride + k] * b->data[k * b->stride + j]; 
            }
            dst->data[i * dst->stride + j] = temp_sum;
        }
    }
}

void um_cmpx_mul_real(U_Cmpx_Matrix *dst, U_Cmpx_Matrix *a, U_Matrix *b){
    size_t col = dst->cols;
    size_t row = dst->rows;
    UM_ASSERT((a->cols == b->rows) && (dst->rows == a->rows) && (dst->cols == b->cols));
    for(size_t i = 0; i < row; ++i){
        for(size_t j = 0; j < col; ++j){
            double complex temp_sum = 0;
            for(size_t k = 0; k < a->cols; ++k){
                temp_sum += a->data[i * a->stride + k] * b->data[k * b->stride + j]; 
            }
            dst->data[i * dst->stride + j] = temp_sum;
        }
    }
}

void um_real_mul_cmpx(U_Cmpx_Matrix *dst, U_Matrix *a, U_Cmpx_Matrix *b){
    size_t col = dst->cols;
    size_t row = dst->rows;
    UM_ASSERT((a->cols == b->rows) && (dst->rows == a->rows) && (dst->cols == b->cols));
    for(size_t i = 0; i < row; ++i){
        for(size_t j = 0; j < col; ++j){
            double complex temp_sum = 0;
            for(size_t k = 0; k < a->cols; ++k){
                temp_sum += a->data[i * a->stride + k] * b->data[k * b->stride + j]; 
            }
            dst->data[i * dst->stride + j] = temp_sum;
        }
    }
}

void um_cmpx_mul_cmpx(U_Cmpx_Matrix *dst, U_Cmpx_Matrix *a, U_Cmpx_Matrix *b){
    size_t col = dst->cols;
    size_t row = dst->rows;
    UM_ASSERT((a->cols == b->rows) && (dst->rows == a->rows) && (dst->cols == b->cols));
    for(size_t i = 0; i < row; ++i){
        for(size_t j = 0; j < col; ++j){
            double complex temp_sum = 0;
            for(size_t k = 0; k < a->cols; ++k){
                temp_sum += a->data[i * a->stride + k] * b->data[k * b->stride + j]; 
            }
            dst->data[i * dst->stride + j] = temp_sum;
        }
    }
}

void um_transpose(U_Matrix *dst, U_Matrix *a){
    size_t col = dst->cols;
    size_t row = dst->rows;
    UM_ASSERT((col == a->rows) && (row == a->cols));
    for(size_t i = 0; i < row; ++i){
        for(size_t j = 0; j < col; ++j){
            dst->data[i * col + j] = a->data[j * row +i];
        }
    }
}

void um_cmpx_transpose(U_Cmpx_Matrix *dst, U_Cmpx_Matrix *a){
    size_t col = dst->cols;
    size_t row = dst->rows;
    UM_ASSERT((col == a->rows) && (row == a->cols));
    for(size_t i = 0; i < row; ++i){
        for(size_t j = 0; j < col; ++j){
            dst->data[i * col + j] = a->data[j * row +i];
        }
    }
}

void um_identity(U_Matrix *m){
    for(size_t i = 0; i < m->stride; ++i){
        m->data[i * m->stride + i] = 1.0;
    }
}
