//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: round.cpp
//
// MATLAB Coder version            : 3.2
// C/C++ source code generated on  : 02-Jan-2018 16:36:12
//

// Include Files
#include "stdafx.h"
#include "rt_nonfinite.h"
#include "processImage.h"
#include "round.h"
#include "processImage_rtwutil.h"

// Function Definitions

//
// Arguments    : emxArray_real_T *x
// Return Type  : void
//
void b_round(emxArray_real_T *x)
{
  int nx;
  int k;
  nx = x->size[1];
  for (k = 0; k + 1 <= nx; k++) {
    x->data[k] = rt_roundd_snf(x->data[k]);
  }
}

//
// File trailer for round.cpp
//
// [EOF]
//
