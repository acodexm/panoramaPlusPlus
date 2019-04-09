//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: processImage_rtwutil.cpp
//
// MATLAB Coder version            : 3.2
// C/C++ source code generated on  : 02-Jan-2018 16:36:12
//

// Include Files
#include "stdafx.h"
#include "rt_nonfinite.h"
#include "processImage.h"
#include "processImage_rtwutil.h"

// Function Definitions

//
// Arguments    : double u
// Return Type  : double
//
double rt_roundd_snf(double u)
{
  double y;
  if (std::abs(u) < 4.503599627370496E+15) {
    if (u >= 0.5) {
      y = std::floor(u + 0.5);
    } else if (u > -0.5) {
      y = u * 0.0;
    } else {
      y = std::ceil(u - 0.5);
    }
  } else {
    y = u;
  }

  return y;
}

//
// File trailer for processImage_rtwutil.cpp
//
// [EOF]
//
