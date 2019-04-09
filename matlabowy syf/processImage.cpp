//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: processImage.cpp
//
// MATLAB Coder version            : 3.2
// C/C++ source code generated on  : 02-Jan-2018 16:36:12
//

// Include Files
#include "stdafx.h"
#include "rt_nonfinite.h"
#include "processImage.h"
#include "processImage_emxutil.h"
#include "strnImage.h"
#include "processImage_rtwutil.h"

// Function Definitions

//
// Arguments    : const emxArray_real_T *I
//                emxArray_real_T *J
// Return Type  : void
//
void processImage(const emxArray_real_T *I, emxArray_real_T *J)
{
  emxArray_real_T *varargin_1;
  unsigned int rect_idx_2;
  unsigned int rect_idx_3;
  unsigned int xdata_idx_1;
  unsigned int ydata_idx_1;
  int xmax;
  int xmin;
  int ymax;
  int ymin;
  double pixelsPerVerticalUnit;
  double pixelsPerHorizontalUnit;
  double r1;
  double x;
  double b_x;
  int rowIdx2;
  double c1;
  int colIdx2;
  int i0;
  int i1;
  int loop_ub;
  int i2;
  emxInit_real_T(&varargin_1, 3);
  strnImage(I, varargin_1);
  rect_idx_2 = (unsigned int)I->size[1];
  rect_idx_3 = (unsigned int)I->size[0];
  xdata_idx_1 = (unsigned int)varargin_1->size[1];
  ydata_idx_1 = (unsigned int)varargin_1->size[0];
  xmax = 1;
  xmin = 1;
  if ((int)xdata_idx_1 > 1) {
    xmax = (int)xdata_idx_1;
  }

  if ((int)xdata_idx_1 < 1) {
    xmin = 0;
  }

  ymax = 1;
  ymin = 1;
  if ((int)ydata_idx_1 > 1) {
    ymax = (int)ydata_idx_1;
  }

  if ((int)ydata_idx_1 < 1) {
    ymin = 0;
  }

  if (varargin_1->size[0] == 1) {
    pixelsPerVerticalUnit = 1.0;
  } else {
    pixelsPerVerticalUnit = ((double)varargin_1->size[0] - 1.0) / (double)(ymax
      - ymin);
  }

  if (varargin_1->size[1] == 1) {
    pixelsPerHorizontalUnit = 1.0;
  } else {
    pixelsPerHorizontalUnit = ((double)varargin_1->size[1] - 1.0) / (double)
      (xmax - xmin);
  }

  r1 = (0.0 - (double)ymin) * pixelsPerVerticalUnit + 1.0;
  x = r1 + (double)rect_idx_3 * pixelsPerVerticalUnit;
  b_x = rt_roundd_snf(x);
  rowIdx2 = (int)rt_roundd_snf(x);
  x = rt_roundd_snf(r1);
  xmax = (int)rt_roundd_snf(r1);
  c1 = (0.0 - (double)xmin) * pixelsPerHorizontalUnit + 1.0;
  pixelsPerVerticalUnit = c1 + (double)rect_idx_2 * pixelsPerHorizontalUnit;
  r1 = rt_roundd_snf(pixelsPerVerticalUnit);
  colIdx2 = (int)rt_roundd_snf(pixelsPerVerticalUnit);
  pixelsPerVerticalUnit = rt_roundd_snf(c1);
  ymax = (int)rt_roundd_snf(c1);
  if (((int)x > varargin_1->size[0]) || ((int)b_x < 1) || ((int)
       pixelsPerVerticalUnit > varargin_1->size[1]) || ((int)r1 < 1)) {
    i0 = J->size[0] * J->size[1] * J->size[2];
    J->size[0] = 0;
    J->size[1] = 0;
    J->size[2] = 1;
    emxEnsureCapacity((emxArray__common *)J, i0, (int)sizeof(double));
  } else {
    if ((int)x < 1) {
      xmax = 1;
    }

    if ((int)b_x > varargin_1->size[0]) {
      rowIdx2 = varargin_1->size[0];
    }

    if ((int)pixelsPerVerticalUnit < 1) {
      ymax = 1;
    }

    if ((int)r1 > varargin_1->size[1]) {
      colIdx2 = varargin_1->size[1];
    }

    if (xmax > rowIdx2) {
      i0 = 0;
      rowIdx2 = 0;
    } else {
      i0 = xmax - 1;
    }

    if (ymax > colIdx2) {
      xmax = 0;
      colIdx2 = 0;
    } else {
      xmax = ymax - 1;
    }

    ymax = varargin_1->size[2];
    ymin = J->size[0] * J->size[1] * J->size[2];
    J->size[0] = rowIdx2 - i0;
    J->size[1] = colIdx2 - xmax;
    J->size[2] = ymax;
    emxEnsureCapacity((emxArray__common *)J, ymin, (int)sizeof(double));
    for (ymin = 0; ymin < ymax; ymin++) {
      xmin = colIdx2 - xmax;
      for (i1 = 0; i1 < xmin; i1++) {
        loop_ub = rowIdx2 - i0;
        for (i2 = 0; i2 < loop_ub; i2++) {
          J->data[(i2 + J->size[0] * i1) + J->size[0] * J->size[1] * ymin] =
            varargin_1->data[((i0 + i2) + varargin_1->size[0] * (xmax + i1)) +
            varargin_1->size[0] * varargin_1->size[1] * ymin];
        }
      }
    }
  }

  emxFree_real_T(&varargin_1);
}

//
// File trailer for processImage.cpp
//
// [EOF]
//
