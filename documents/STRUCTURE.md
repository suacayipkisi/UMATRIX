# Main Structure

- `U_`          : type of data
- `U_Cmpx_`     : type of complex number data
- `um_`         : function names
- `um_cmpx_`    : functions for complex numbers  

- `...Sparse`   : sparse matrix data types
- `...sparse`   : sparse matrix functions

## First define a matrix according to its type
types: 
- U_Matrix
- U_Cmpx_Matrix
- U_Matrix_Sparse
- U_Cmpx_Matrix_Sparse

## If matrix is `Sparse`
- `um_...init_...` : (must) init matrix (default 3x3)
- `um_...reserve`  : if you know number of non-empty values, reserve

