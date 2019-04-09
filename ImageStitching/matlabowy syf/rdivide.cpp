//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: rdivide.cpp
//
// MATLAB Coder version            : 3.2
// C/C++ source code generated on  : 02-Jan-2018 16:36:12
//

// Include Files
#include "stdafx.h"
#include "rt_nonfinite.h"
#include "processImage.h"
#include "rdivide.h"
#include "processImage_emxutil.h"

// Function Definitions

//
// Arguments    : const emxArray_real_T *x
//                double y
//                emxArray_real_T *z
// Return Type  : void
//
void rdivide(const emxArray_real_T *x, double y, emxArray_real_T *z)
{
  int i4;
  int loop_ub;
  i4 = z->size[0] * z->size[1];
  z->size[0] = 1;
  z->size[1] = x->size[1];
  emxEnsureCapacity((emxArray__common *)z, i4, (int)sizeof(double));
  loop_ub = x->size[0] * x->size[1];
  for (i4 = 0; i4 < loop_ub; i4++) {
    z->data[i4] = x->data[i4] / y;
  }
}

//
// File trailer for rdivide.cpp
//
// [EOF]
//
