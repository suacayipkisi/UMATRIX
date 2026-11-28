// SPDX-License-Identifier: MIT
// Copyright (c) 2026 suacayipkisi

#ifndef DETERMINANT_METHODS_H_
#define DETERMINANT_METHODS_H_

double det_cofactor_expansion();

/* n <= 10^4, O((2/3)n^3) */
double det_lu_decomposition();

/* n <= 10^4, O((1/3)n^3), SPD */
double det_cholesky_decomposition();

/* n ~= 10^4 to 10^6, sparse */
double det_sparse_cholesky();

/* not able to factorize, n > 10^7 */
double det_stochastic_lanczos_quadrature();

#endif