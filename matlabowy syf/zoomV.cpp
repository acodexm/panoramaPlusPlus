//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: zoomV.cpp
//
// MATLAB Coder version            : 3.2
// C/C++ source code generated on  : 02-Jan-2018 16:36:12
//

// Include Files
#include "stdafx.h"
#include "rt_nonfinite.h"
#include "processImage.h"
#include "zoomV.h"
#include "processImage_emxutil.h"
#include "round.h"
#include "rdivide.h"

// Function Definitions

//
// Arguments    : const emxArray_real_T *J
//                double times
//                emxArray_real_T *JJ
// Return Type  : void
//
void zoomV(const emxArray_real_T *J, double times, emxArray_real_T *JJ)
{
  int size_J;
  double size_JJ;
  emxArray_real_T *y;
  int b_y;
  int loop_ub;
  emxArray_real_T *c_y;
  emxArray_real_T *i;
  int unnamed_idx_1;
  size_J = J->size[1];
  size_JJ = times * (double)J->size[1];
  emxInit_real_T1(&y, 2);
  if (rtIsNaN(size_JJ)) {
    b_y = y->size[0] * y->size[1];
    y->size[0] = 1;
    y->size[1] = 1;
    emxEnsureCapacity((emxArray__common *)y, b_y, (int)sizeof(double));
    y->data[0] = rtNaN;
  } else if (size_JJ < 1.0) {
    b_y = y->size[0] * y->size[1];
    y->size[0] = 1;
    y->size[1] = 0;
    emxEnsureCapacity((emxArray__common *)y, b_y, (int)sizeof(double));
  } else if (rtIsInf(size_JJ) && (1.0 == size_JJ)) {
    b_y = y->size[0] * y->size[1];
    y->size[0] = 1;
    y->size[1] = 1;
    emxEnsureCapacity((emxArray__common *)y, b_y, (int)sizeof(double));
    y->data[0] = rtNaN;
  } else {
    b_y = y->size[0] * y->size[1];
    y->size[0] = 1;
    y->size[1] = (int)std::floor(size_JJ - 1.0) + 1;
    emxEnsureCapacity((emxArray__common *)y, b_y, (int)sizeof(double));
    loop_ub = (int)std::floor(size_JJ - 1.0);
    for (b_y = 0; b_y <= loop_ub; b_y++) {
      y->data[y->size[0] * b_y] = 1.0 + (double)b_y;
    }
  }

  emxInit_real_T1(&c_y, 2);
  b_y = c_y->size[0] * c_y->size[1];
  c_y->size[0] = 1;
  c_y->size[1] = y->size[1];
  emxEnsureCapacity((emxArray__common *)c_y, b_y, (int)sizeof(double));
  loop_ub = y->size[0] * y->size[1];
  for (b_y = 0; b_y < loop_ub; b_y++) {
    c_y->data[b_y] = y->data[b_y] - 0.5;
  }

  rdivide(c_y, times, y);
  b_y = y->size[0] * y->size[1];
  y->size[0] = 1;
  emxEnsureCapacity((emxArray__common *)y, b_y, (int)sizeof(double));
  loop_ub = y->size[0];
  b_y = y->size[1];
  loop_ub *= b_y;
  emxFree_real_T(&c_y);
  for (b_y = 0; b_y < loop_ub; b_y++) {
    y->data[b_y] += 0.5;
  }

  emxInit_real_T1(&i, 2);
  b_round(y);
  unnamed_idx_1 = y->size[1];
  b_y = i->size[0] * i->size[1];
  i->size[0] = 1;
  i->size[1] = y->size[1];
  emxEnsureCapacity((emxArray__common *)i, b_y, (int)sizeof(double));
  for (loop_ub = 0; loop_ub + 1 <= unnamed_idx_1; loop_ub++) {
    if (y->data[loop_ub] <= size_J) {
      size_JJ = y->data[loop_ub];
    } else {
      size_JJ = size_J;
    }

    i->data[loop_ub] = size_JJ;
  }

  emxFree_real_T(&y);
  b_y = JJ->size[0] * JJ->size[1];
  JJ->size[0] = 1;
  JJ->size[1] = i->size[1];
  emxEnsureCapacity((emxArray__common *)JJ, b_y, (int)sizeof(double));
  loop_ub = i->size[1];
  for (b_y = 0; b_y < loop_ub; b_y++) {
    JJ->data[JJ->size[0] * b_y] = J->data[(int)i->data[i->size[0] * b_y] - 1];
  }

  emxFree_real_T(&i);
}

//
// File trailer for zoomV.cpp
//
// [EOF]
//
