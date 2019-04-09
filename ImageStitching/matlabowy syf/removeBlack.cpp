//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: removeBlack.cpp
//
// MATLAB Coder version            : 3.2
// C/C++ source code generated on  : 02-Jan-2018 16:36:12
//

// Include Files
#include "stdafx.h"
#include "rt_nonfinite.h"
#include "processImage.h"
#include "removeBlack.h"
#include "processImage_emxutil.h"

// Function Definitions

//
// Arguments    : const emxArray_real_T *T
//                double *left
//                double *right
//                emxArray_real_T *V
//                double *times
// Return Type  : void
//
void removeBlack(const emxArray_real_T *T, double *left, double *right,
                 emxArray_real_T *V, double *times)
{
  unsigned int iter;
  int s;
  int i;
  int i5;
  iter = 1U;
  s = T->size[1] - 1;
  if ((*left == 1.0) && (*right == 0.0)) {
    i = 0;
    while ((i <= s) && (T->data[T->size[0] * i] <= 0.05)) {
      (*left)++;
      i++;
    }

    i5 = (int)((1.0 + (-1.0 - (double)T->size[1])) / -1.0);
    i = 0;
    while ((i <= i5 - 1) && (T->data[T->size[0] * (s - i)] <= 0.05)) {
      (*right)++;
      i++;
    }
  }

  i5 = V->size[0] * V->size[1];
  V->size[0] = 1;
  V->size[1] = (int)(((double)T->size[1] - *left) - *right);
  emxEnsureCapacity((emxArray__common *)V, i5, (int)sizeof(double));
  s = (int)(((double)T->size[1] - *left) - *right);
  for (i5 = 0; i5 < s; i5++) {
    V->data[i5] = 0.0;
  }

  i5 = (int)(((double)T->size[1] - *right) + (1.0 - *left));
  for (i = 0; i < i5; i++) {
    V->data[V->size[0] * ((int)iter - 1)] = T->data[T->size[0] * ((int)(*left +
      (double)i) - 1)];
    iter++;
  }

  *times = (double)T->size[1] / (((double)T->size[1] - *left) - *right);
}

//
// File trailer for removeBlack.cpp
//
// [EOF]
//
