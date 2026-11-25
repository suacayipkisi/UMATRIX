// SPDX-License-Identifier: MIT
// Copyright (c) 2026 suacayipkisi

#include "umatrix/elemental.h"

int main(void){
    
    // set, fill, assign, print and free the real number matrix
    U_Matrix a = um_alloc(2, 3);
    um_fill(a, 1.0);
    *um_at(a, 1, 2) = 42.0;
    UM_PRINT(a);
    um_free_null(a);
    
    // set, fill, assign, print and free the imaginary number matrix
    U_Im_Matrix i = um_im_alloc(2, 3);
    um_im_fill(i, 1.0, 3.0);
    *um_im_at(i, 1, 2) = (U_Imag){42, 34};
    UM_IM_PRINT(i);
    um_im_free_null(i);
    
    return 0;
}
