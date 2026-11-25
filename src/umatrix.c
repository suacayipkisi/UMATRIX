#include "umatrix.h"

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

U_Matrix um_alloc(size_t rows, size_t cols){
    U_Matrix m;
    m.rows = rows;
    m.cols = cols;
    m.stride = cols;
    m.data = UM_MALLOC(sizeof(*m.data) * rows * cols);
    UM_ASSERT(m.data != NULL);
    return m;
}

U_Im_Matrix um_im_alloc(size_t rows, size_t cols){
    U_Im_Matrix m;
    m.rows = rows;
    m.cols = cols;
    m.stride = cols;
    m.data = UM_MALLOC(sizeof(*m.data) * rows * cols);
    UM_ASSERT(m.data != NULL);
    return m;
}

void um_free_null(U_Matrix m){
    UM_FREE(m.data);
    m.data = NULL;
}

void um_im_free_null(U_Im_Matrix m){
    UM_FREE(m.data);
    m.data = NULL;
}

/* definition of filling matrix with a value */
void um_fill(U_Matrix m, double value){
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            *um_at(m, i, j) = value; 
        }
    }
}

void um_im_fill(U_Im_Matrix m, double real_val, double imag_val){
    for (size_t i = 0; i < m.rows; ++i) {
        for (size_t j = 0; j < m.cols; ++j) {
            *um_im_at(m, i, j) = (U_Imag){real_val, imag_val};
        }
    }
}

/* definiton of printing the matrix into terminal */
void um_print(U_Matrix m, const char *name){
    printf("%s = [\n", name);
    for (size_t i = 0; i < m.rows; ++i) {
        printf("    ");
        for (size_t j = 0; j < m.cols; ++j) {
            printf("%10.4f ", *um_at(m, i, j));
        }
        printf("\n");
    }
    printf("]\n");
}

void um_im_print(U_Im_Matrix m, const char *name){
    printf("%s = [\n", name);
    for (size_t i = 0; i < m.rows; ++i) {
        printf("    ");
        for (size_t j = 0; j < m.cols; ++j) {
            U_Imag imag_num = *um_im_at(m, i, j);
            printf("%10.4f + %-10.4f ", imag_num.real, imag_num.imag);
        }
        printf("\n");
    }
    printf("]\n");
}
