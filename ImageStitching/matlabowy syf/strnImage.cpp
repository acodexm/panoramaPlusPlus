//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: strnImage.cpp
//
// MATLAB Coder version            : 3.2
// C/C++ source code generated on  : 02-Jan-2018 16:36:12
//

// Include Files
#include "stdafx.h"
#include "rt_nonfinite.h"
#include "processImage.h"
#include "strnImage.h"
#include "zoomV.h"
#include "removeBlack.h"
#include "processImage_emxutil.h"
#include "round.h"
#include "rdivide.h"

// Function Definitions

//
// Arguments    : const emxArray_real_T *I
//                emxArray_real_T *J
// Return Type  : void
//
void strnImage(const emxArray_real_T *I, emxArray_real_T *J)
{
  int rgb;
  int i3;
  int loop_ub;
  int i;
  emxArray_real_T *z;
  emxArray_real_T *j;
  emxArray_real_T *b_I;
  double left;
  double right;
  emxArray_real_T *b_z;
  int b_j;
  emxArray_real_T *V;
  emxArray_real_T *T;
  emxArray_real_T *c_j;
  emxArray_real_T *d_j;
  emxArray_real_T *e_j;
  emxArray_real_T *f_j;
  double iter;
  boolean_T x[2];
  boolean_T y;
  boolean_T exitg2;
  int s;
  boolean_T exitg1;
  int size_J;
  double size_JJ;
  rgb = I->size[2] - 1;
  i3 = J->size[0] * J->size[1] * J->size[2];
  J->size[0] = I->size[0];
  J->size[1] = I->size[1];
  J->size[2] = I->size[2];
  emxEnsureCapacity((emxArray__common *)J, i3, (int)sizeof(double));
  loop_ub = I->size[0] * I->size[1] * I->size[2];
  for (i3 = 0; i3 < loop_ub; i3++) {
    J->data[i3] = 0.0;
  }

  i = 0;
  emxInit_real_T1(&z, 2);
  emxInit_real_T1(&j, 2);
  emxInit_real_T1(&b_I, 2);
  while (i <= I->size[0] - 1) {
    left = 1.0;
    right = 0.0;
    for (b_j = 0; b_j <= rgb; b_j++) {
      loop_ub = I->size[1];
      i3 = b_I->size[0] * b_I->size[1];
      b_I->size[0] = 1;
      b_I->size[1] = loop_ub;
      emxEnsureCapacity((emxArray__common *)b_I, i3, (int)sizeof(double));
      for (i3 = 0; i3 < loop_ub; i3++) {
        b_I->data[b_I->size[0] * i3] = I->data[(i + I->size[0] * i3) + I->size[0]
          * I->size[1] * b_j];
      }

      removeBlack(b_I, &left, &right, j, &iter);
      for (i3 = 0; i3 < 2; i3++) {
        x[i3] = (j->size[i3] == 0);
      }

      y = true;
      loop_ub = 1;
      exitg2 = false;
      while ((!exitg2) && (loop_ub < 3)) {
        if (!x[loop_ub - 1]) {
          y = false;
          exitg2 = true;
        } else {
          loop_ub++;
        }
      }

      if (!y) {
        zoomV(j, iter, z);
        if (1 > z->size[1]) {
          loop_ub = -1;
        } else {
          loop_ub = z->size[1] - 1;
        }

        for (i3 = 0; i3 <= loop_ub; i3++) {
          J->data[(i + J->size[0] * i3) + J->size[0] * J->size[1] * b_j] =
            z->data[z->size[0] * i3];
        }
      }
    }

    i++;
  }

  emxFree_real_T(&b_I);
  i = 0;
  emxInit_real_T1(&b_z, 2);
  emxInit_real_T1(&V, 2);
  emxInit_real_T2(&T, 1);
  emxInit_real_T1(&c_j, 2);
  emxInit_real_T1(&d_j, 2);
  emxInit_real_T2(&e_j, 1);
  emxInit_real_T2(&f_j, 1);
  while (i <= I->size[1] - 1) {
    left = 1.0;
    right = 0.0;
    for (b_j = 0; b_j <= rgb; b_j++) {
      loop_ub = I->size[0];
      i3 = T->size[0];
      T->size[0] = loop_ub;
      emxEnsureCapacity((emxArray__common *)T, i3, (int)sizeof(double));
      for (i3 = 0; i3 < loop_ub; i3++) {
        T->data[i3] = I->data[(i3 + I->size[0] * i) + I->size[0] * I->size[1] *
          b_j];
      }

      iter = 1.0;
      i3 = I->size[0];
      if (i3 == 1) {
        s = 1;
        if ((left == 1.0) && (right == 0.0)) {
          if (T->data[0] <= 0.05) {
            left = 2.0;
          }

          if (T->data[0] <= 0.05) {
            right = 1.0;
          }
        }

        i3 = V->size[0] * V->size[1];
        V->size[0] = 1;
        V->size[1] = 0;
        emxEnsureCapacity((emxArray__common *)V, i3, (int)sizeof(double));
        i3 = (int)((1.0 - right) + (1.0 - left));
        loop_ub = 0;
        while (loop_ub <= i3 - 1) {
          V->data[V->size[0] * ((int)iter - 1)] = T->data[0];
          iter++;
          loop_ub = 1;
        }
      } else {
        s = I->size[0];
        if ((left == 1.0) && (right == 0.0)) {
          loop_ub = 0;
          while ((loop_ub <= s - 1) && (T->data[loop_ub] <= 0.05)) {
            left++;
            loop_ub++;
          }

          i3 = I->size[0];
          i3 = (int)((1.0 + (-1.0 - (double)i3)) / -1.0);
          loop_ub = 0;
          while ((loop_ub <= i3 - 1) && (T->data[(s - loop_ub) - 1] <= 0.05)) {
            right++;
            loop_ub++;
          }
        }

        i3 = I->size[0];
        loop_ub = V->size[0] * V->size[1];
        V->size[0] = (int)(((double)i3 - left) - right);
        V->size[1] = 1;
        emxEnsureCapacity((emxArray__common *)V, loop_ub, (int)sizeof(double));
        loop_ub = (int)(((double)i3 - left) - right);
        for (i3 = 0; i3 < loop_ub; i3++) {
          V->data[i3] = 0.0;
        }

        i3 = (int)(((double)s - right) + (1.0 - left));
        for (loop_ub = 0; loop_ub < i3; loop_ub++) {
          V->data[(int)iter - 1] = T->data[(int)(left + (double)loop_ub) - 1];
          iter++;
        }
      }

      iter = (double)s / (((double)s - left) - right);
      for (i3 = 0; i3 < 2; i3++) {
        x[i3] = (V->size[i3] == 0);
      }

      y = true;
      loop_ub = 1;
      exitg1 = false;
      while ((!exitg1) && (loop_ub < 3)) {
        if (!x[loop_ub - 1]) {
          y = false;
          exitg1 = true;
        } else {
          loop_ub++;
        }
      }

      if (!y) {
        if (V->size[0] == 1) {
          size_J = V->size[1];
          size_JJ = iter * (double)V->size[1];
          if (rtIsNaN(size_JJ)) {
            i3 = j->size[0] * j->size[1];
            j->size[0] = 1;
            j->size[1] = 1;
            emxEnsureCapacity((emxArray__common *)j, i3, (int)sizeof(double));
            j->data[0] = rtNaN;
          } else if (size_JJ < 1.0) {
            i3 = j->size[0] * j->size[1];
            j->size[0] = 1;
            j->size[1] = 0;
            emxEnsureCapacity((emxArray__common *)j, i3, (int)sizeof(double));
          } else if (rtIsInf(size_JJ) && (1.0 == size_JJ)) {
            i3 = j->size[0] * j->size[1];
            j->size[0] = 1;
            j->size[1] = 1;
            emxEnsureCapacity((emxArray__common *)j, i3, (int)sizeof(double));
            j->data[0] = rtNaN;
          } else {
            i3 = j->size[0] * j->size[1];
            j->size[0] = 1;
            j->size[1] = (int)std::floor(size_JJ - 1.0) + 1;
            emxEnsureCapacity((emxArray__common *)j, i3, (int)sizeof(double));
            loop_ub = (int)std::floor(size_JJ - 1.0);
            for (i3 = 0; i3 <= loop_ub; i3++) {
              j->data[j->size[0] * i3] = 1.0 + (double)i3;
            }
          }

          i3 = d_j->size[0] * d_j->size[1];
          d_j->size[0] = 1;
          d_j->size[1] = j->size[1];
          emxEnsureCapacity((emxArray__common *)d_j, i3, (int)sizeof(double));
          loop_ub = j->size[0] * j->size[1];
          for (i3 = 0; i3 < loop_ub; i3++) {
            d_j->data[i3] = j->data[i3] - 0.5;
          }

          rdivide(d_j, iter, z);
          i3 = z->size[0] * z->size[1];
          z->size[0] = 1;
          emxEnsureCapacity((emxArray__common *)z, i3, (int)sizeof(double));
          loop_ub = z->size[0];
          s = z->size[1];
          loop_ub *= s;
          for (i3 = 0; i3 < loop_ub; i3++) {
            z->data[i3] += 0.5;
          }

          b_round(z);
          s = z->size[1];
          i3 = j->size[0] * j->size[1];
          j->size[0] = 1;
          j->size[1] = z->size[1];
          emxEnsureCapacity((emxArray__common *)j, i3, (int)sizeof(double));
          for (loop_ub = 0; loop_ub + 1 <= s; loop_ub++) {
            if (z->data[loop_ub] <= size_J) {
              iter = z->data[loop_ub];
            } else {
              iter = size_J;
            }

            j->data[loop_ub] = iter;
          }

          i3 = b_z->size[0] * b_z->size[1];
          b_z->size[0] = 1;
          b_z->size[1] = j->size[1];
          emxEnsureCapacity((emxArray__common *)b_z, i3, (int)sizeof(double));
          loop_ub = j->size[1];
          for (i3 = 0; i3 < loop_ub; i3++) {
            b_z->data[b_z->size[0] * i3] = V->data[(int)j->data[j->size[0] * i3]
              - 1];
          }
        } else {
          size_J = V->size[0];
          size_JJ = iter * (double)V->size[0];
          if (rtIsNaN(size_JJ)) {
            i3 = j->size[0] * j->size[1];
            j->size[0] = 1;
            j->size[1] = 1;
            emxEnsureCapacity((emxArray__common *)j, i3, (int)sizeof(double));
            j->data[0] = rtNaN;
          } else if (size_JJ < 1.0) {
            i3 = j->size[0] * j->size[1];
            j->size[0] = 1;
            j->size[1] = 0;
            emxEnsureCapacity((emxArray__common *)j, i3, (int)sizeof(double));
          } else if (rtIsInf(size_JJ) && (1.0 == size_JJ)) {
            i3 = j->size[0] * j->size[1];
            j->size[0] = 1;
            j->size[1] = 1;
            emxEnsureCapacity((emxArray__common *)j, i3, (int)sizeof(double));
            j->data[0] = rtNaN;
          } else {
            i3 = j->size[0] * j->size[1];
            j->size[0] = 1;
            j->size[1] = (int)std::floor(size_JJ - 1.0) + 1;
            emxEnsureCapacity((emxArray__common *)j, i3, (int)sizeof(double));
            loop_ub = (int)std::floor(size_JJ - 1.0);
            for (i3 = 0; i3 <= loop_ub; i3++) {
              j->data[j->size[0] * i3] = 1.0 + (double)i3;
            }
          }

          i3 = c_j->size[0] * c_j->size[1];
          c_j->size[0] = 1;
          c_j->size[1] = j->size[1];
          emxEnsureCapacity((emxArray__common *)c_j, i3, (int)sizeof(double));
          loop_ub = j->size[0] * j->size[1];
          for (i3 = 0; i3 < loop_ub; i3++) {
            c_j->data[i3] = j->data[i3] - 0.5;
          }

          rdivide(c_j, iter, z);
          i3 = z->size[0] * z->size[1];
          z->size[0] = 1;
          emxEnsureCapacity((emxArray__common *)z, i3, (int)sizeof(double));
          loop_ub = z->size[0];
          s = z->size[1];
          loop_ub *= s;
          for (i3 = 0; i3 < loop_ub; i3++) {
            z->data[i3] += 0.5;
          }

          b_round(z);
          s = z->size[1];
          i3 = j->size[0] * j->size[1];
          j->size[0] = 1;
          j->size[1] = z->size[1];
          emxEnsureCapacity((emxArray__common *)j, i3, (int)sizeof(double));
          for (loop_ub = 0; loop_ub + 1 <= s; loop_ub++) {
            if (z->data[loop_ub] <= size_J) {
              iter = z->data[loop_ub];
            } else {
              iter = size_J;
            }

            j->data[loop_ub] = iter;
          }

          i3 = T->size[0];
          T->size[0] = j->size[1];
          emxEnsureCapacity((emxArray__common *)T, i3, (int)sizeof(double));
          loop_ub = j->size[1];
          for (i3 = 0; i3 < loop_ub; i3++) {
            T->data[i3] = V->data[(int)j->data[j->size[0] * i3] - 1];
          }

          i3 = e_j->size[0];
          e_j->size[0] = j->size[1];
          emxEnsureCapacity((emxArray__common *)e_j, i3, (int)sizeof(double));
          loop_ub = j->size[1];
          for (i3 = 0; i3 < loop_ub; i3++) {
            e_j->data[i3] = j->data[j->size[0] * i3];
          }

          i3 = b_z->size[0] * b_z->size[1];
          b_z->size[0] = e_j->size[0];
          b_z->size[1] = 1;
          emxEnsureCapacity((emxArray__common *)b_z, i3, (int)sizeof(double));
          i3 = f_j->size[0];
          f_j->size[0] = j->size[1];
          emxEnsureCapacity((emxArray__common *)f_j, i3, (int)sizeof(double));
          loop_ub = j->size[1];
          for (i3 = 0; i3 < loop_ub; i3++) {
            f_j->data[i3] = j->data[j->size[0] * i3];
          }

          loop_ub = f_j->size[0];
          for (i3 = 0; i3 < loop_ub; i3++) {
            b_z->data[i3] = T->data[i3];
          }
        }

        if (1 > b_z->size[0]) {
          loop_ub = -1;
        } else {
          loop_ub = b_z->size[0] - 1;
        }

        for (i3 = 0; i3 <= loop_ub; i3++) {
          J->data[(i3 + J->size[0] * i) + J->size[0] * J->size[1] * b_j] =
            b_z->data[i3];
        }
      }
    }

    i++;
  }

  emxFree_real_T(&f_j);
  emxFree_real_T(&e_j);
  emxFree_real_T(&d_j);
  emxFree_real_T(&c_j);
  emxFree_real_T(&j);
  emxFree_real_T(&T);
  emxFree_real_T(&z);
  emxFree_real_T(&V);
  emxFree_real_T(&b_z);
}

//
// File trailer for strnImage.cpp
//
// [EOF]
//
