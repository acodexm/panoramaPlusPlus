////
//// Academic License - for use in teaching, academic research, and meeting
//// course requirements at degree granting institutions only.  Not for
//// government, commercial, or other organizational use.
//// File: main.cpp
////
//// MATLAB Coder version            : 3.2
//// C/C++ source code generated on  : 02-Jan-2018 16:36:12
////
//
////***********************************************************************
//// This automatically generated example C main file shows how to call
//// entry-point functions that MATLAB Coder generated. You must customize
//// this file for your application. Do not modify this file directly.
//// Instead, make a copy of this file, modify it, and integrate it into
//// your development environment.
////
//// This file initializes entry-point function arguments to a default
//// size and value before calling the entry-point functions. It does
//// not store or use any values returned from the entry-point functions.
//// If necessary, it does pre-allocate memory for returned values.
//// You can use this file as a starting point for a main function that
//// you can deploy in your application.
////
//// After you copy the file, and before you deploy it, you must make the
//// following changes:
//// * For variable-size function arguments, change the example sizes to
//// the sizes that your application requires.
//// * Change the example values of function arguments to the values that
//// your application requires.
//// * If the entry-point functions return values, store these values or
//// otherwise use them as required by your application.
////
////***********************************************************************
//// Include Files
#include "stdafx.h"
//#include "rt_nonfinite.h"
//#include "processImage.h"
//#include "main.h"
//#include "processImage_terminate.h"
//#include "processImage_emxAPI.h"
//#include "processImage_initialize.h"
//
//// Function Declarations
//static double argInit_real_T();
//static emxArray_real_T *c_argInit_UnboundedxUnboundedxd();
//static void main_processImage();
//
//// Function Definitions
//
////
//// Arguments    : void
//// Return Type  : double
////
//static double argInit_real_T()
//{
//  return 0.0;
//}
//
////
//// Arguments    : void
//// Return Type  : emxArray_real_T *
////
//static emxArray_real_T *c_argInit_UnboundedxUnboundedxd()
//{
//  emxArray_real_T *result;
//  static int iv0[3] = { 2, 2, 2 };
//
//  int idx0;
//  int idx1;
//  int idx2;
//
//  // Set the size of the array.
//  // Change this size to the value that the application requires.
//  result = emxCreateND_real_T(3, *(int (*)[3])&iv0[0]);
//
//  // Loop over the array to initialize each element.
//  for (idx0 = 0; idx0 < result->size[0U]; idx0++) {
//    for (idx1 = 0; idx1 < result->size[1U]; idx1++) {
//      for (idx2 = 0; idx2 < result->size[2U]; idx2++) {
//        // Set the value of the array element.
//        // Change this value to the value that the application requires.
//        result->data[(idx0 + result->size[0] * idx1) + result->size[0] *
//          result->size[1] * idx2] = argInit_real_T();
//      }
//    }
//  }
//
//  return result;
//}
//
////
//// Arguments    : void
//// Return Type  : void
////
//static void main_processImage()
//{
//  emxArray_real_T *J;
//  emxArray_real_T *I;
//  emxInitArray_real_T(&J, 3);
//
//  // Initialize function 'processImage' input arguments.
//  // Initialize function input argument 'I'.
//  I = c_argInit_UnboundedxUnboundedxd();
//
//  // Call the entry-point 'processImage'.
//  processImage(I, J);
//  emxDestroyArray_real_T(J);
//  emxDestroyArray_real_T(I);
//}
//
////
//// Arguments    : int argc
////                const char * const argv[]
//// Return Type  : int
////
//int main(int, const char * const [])
//{
//  // Initialize the application.
//  // You do not need to do this more than one time.
//  processImage_initialize();
//
//  // Invoke the entry-point functions.
//  // You can call entry-point functions multiple times.
//  main_processImage();
//
//  // Terminate the application.
//  // You do not need to do this more than one time.
//  processImage_terminate();
//  return 0;
//}
//
////
//// File trailer for main.cpp
////
//// [EOF]
////
