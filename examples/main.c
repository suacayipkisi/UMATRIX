// SPDX-License-Identifier: MIT
// Copyright (c) 2026 suacayipkisi

#include "umatrix/elemental.h"

int main(void){
    
    // set, fill, assign, print and free the real number matrix
    U_Matrix a = um_calloc(2, 3);
    um_fill(&a, 1.0);
    um_set_val(&a, 1, 2, 42);
    UM_PRINT(&a);
    um_free_null(&a);
    
    // set, fill, assign, print and free the imaginary number matrix
    U_Cmpx_Matrix i = um_cmpx_calloc(2, 3);
    um_cmpx_fill(&i, 1.0 + 3.0 * I);
    um_cmpx_set_val(&i, 1, 2, 42 + (-34) * I);
    UM_CMPX_PRINT(&i);
    um_cmpx_free_null(&i);
    
    return 0;
}
