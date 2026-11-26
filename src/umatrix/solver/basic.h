// SPDX-License-Identifier: MIT
// Copyright (c) 2026 suacayipkisi

#include "../elemental.h"

#include <complex.h>

// Gaussian elimination
double um_det(U_Matrix a);
int um_inverse(U_Matrix dst, U_Matrix a);   // returns 0 if singular
int um_solve(U_Matrix x, U_Matrix a, U_Matrix b); // solves a * x = b