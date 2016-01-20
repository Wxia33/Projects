/*
 * fun.c
 *
 * Code generation for function 'fun'
 *
 * C source code generated on: Thu Jul 25 22:25:07 2013
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "BFPSolver.h"
#include "fun.h"
#include "BFPSolver_rtwutil.h"

/* Function Definitions */
int32_T fun(int32_T x)
{
  int32_T y;
  real_T d0;
  int32_T ak;
  int32_T b_y;
  uint32_T bku;
  int32_T exitg1;
  d0 = rt_roundd_snf((real_T)x - 1.5);
  if (d0 >= -2.147483648E+9) {
    ak = (int32_T)d0;
  } else {
    ak = MIN_int32_T;
  }

  b_y = 1;
  bku = 3U;
  do {
    exitg1 = 0;
    if ((bku & 1U) != 0U) {
      b_y = mul_s32_s32_s32_sat(ak, b_y);
    }

    bku >>= 1U;
    if ((int32_T)bku == 0) {
      exitg1 = 1;
    } else {
      ak = mul_s32_s32_s32_sat(ak, ak);
    }
  } while (exitg1 == 0);

  y = b_y - 1;
  if ((b_y < 0) && (y >= 0)) {
    y = MIN_int32_T;
  }

  return y;
}

/* End of code generation (fun.c) */
