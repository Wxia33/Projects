/*
 * BFPSolver.c
 *
 * Code generation for function 'BFPSolver'
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
int32_T BFPSolver(int32_T xmin, int32_T xmax)
{
  int32_T Out;
  int32_T ffm;
  int32_T ffM;
  int32_T qY;
  int32_T fmid;
  int32_T exitg2;
  int32_T saturatedUnaryMinus;
  int32_T b_fmid;
  uint32_T y;
  uint32_T b_y;
  uint32_T q;
  int32_T exitg1;
  int32_T c_fmid;

  /*  Bisection False Position Method Algorithm */
  /*  Find the roots of F(x) = 0 given a search interval [xmin xmax]; */
  /*  Tolerance level */
  /*  Make sure the function has at least one zero */
  ffm = fun(xmin);
  ffM = fun(xmax);

  /*  Bisection algortihm */
  /*  Loop */
  if (ffm > 0) {
    /*  Decrea function */
    qY = xmin + xmax;
    if ((xmin < 0) && ((xmax < 0) && (qY >= 0))) {
      qY = MIN_int32_T;
    } else {
      if ((xmin > 0) && ((xmax > 0) && (qY <= 0))) {
        qY = MAX_int32_T;
      }
    }

    Out = (int32_T)rt_roundd_snf(0.5 * (real_T)qY);
    fmid = fun(Out);
    do {
      exitg2 = 0;
      qY = ffm - ffM;
      if ((ffm < 0) && ((ffM >= 0) && (qY >= 0))) {
        qY = MIN_int32_T;
      } else {
        if ((ffm >= 0) && ((ffM < 0) && (qY < 0))) {
          qY = MAX_int32_T;
        }
      }

      if (qY > 1.0E-6) {
        if (fmid <= MIN_int32_T) {
          saturatedUnaryMinus = MAX_int32_T;
        } else {
          saturatedUnaryMinus = -fmid;
        }

        if (fmid < 0) {
          b_fmid = saturatedUnaryMinus;
        } else {
          b_fmid = fmid;
        }

        if (b_fmid > 1.0E-6) {
          qY = xmax - xmin;
          if ((xmax < 0) && ((xmin >= 0) && (qY >= 0))) {
            qY = MIN_int32_T;
          } else {
            if ((xmax >= 0) && ((xmin < 0) && (qY < 0))) {
              qY = MAX_int32_T;
            }
          }

          fmid = ffM - ffm;
          if ((ffM < 0) && ((ffm >= 0) && (fmid >= 0))) {
            fmid = MIN_int32_T;
          } else {
            if ((ffM >= 0) && ((ffm < 0) && (fmid < 0))) {
              fmid = MAX_int32_T;
            }
          }

          if (fmid == 0) {
            if (qY == 0) {
              saturatedUnaryMinus = 0;
            } else if (qY < 0) {
              saturatedUnaryMinus = MIN_int32_T;
            } else {
              saturatedUnaryMinus = MAX_int32_T;
            }
          } else if (fmid == 1) {
            saturatedUnaryMinus = qY;
          } else if (fmid == -1) {
            if (qY <= MIN_int32_T) {
              saturatedUnaryMinus = MAX_int32_T;
            } else {
              saturatedUnaryMinus = -qY;
            }
          } else {
            if (qY >= 0) {
              y = (uint32_T)qY;
            } else if (qY == MIN_int32_T) {
              y = 2147483648U;
            } else {
              y = (uint32_T)-qY;
            }

            if (fmid >= 0) {
              b_y = (uint32_T)fmid;
            } else if (fmid == MIN_int32_T) {
              b_y = 2147483648U;
            } else {
              b_y = (uint32_T)-fmid;
            }

            if (b_y == (uint32_T)0) {
              q = MAX_uint32_T;
            } else {
              q = y / b_y;
            }

            y -= q * b_y;
            if ((y > 0U) && (y >= (b_y >> 1U) + (b_y & 1U))) {
              q++;
            }

            saturatedUnaryMinus = (int32_T)q;
            if ((qY < 0) != (fmid < 0)) {
              saturatedUnaryMinus = -saturatedUnaryMinus;
            }
          }

          fmid = mul_s32_s32_s32_sat(saturatedUnaryMinus, ffm);
          Out = xmin - fmid;
          if ((xmin < 0) && ((fmid >= 0) && (Out >= 0))) {
            Out = MIN_int32_T;
          } else {
            if ((xmin >= 0) && ((fmid < 0) && (Out < 0))) {
              Out = MAX_int32_T;
            }
          }

          fmid = fun(Out);
          if (fmid > 0) {
            xmin = Out;
            ffm = fun(Out);
          } else {
            xmax = Out;
            ffM = fun(Out);
          }
        } else {
          exitg2 = 1;
        }
      } else {
        exitg2 = 1;
      }
    } while (exitg2 == 0);
  } else {
    /*  Increas function */
    qY = xmin + xmax;
    if ((xmin < 0) && ((xmax < 0) && (qY >= 0))) {
      qY = MIN_int32_T;
    } else {
      if ((xmin > 0) && ((xmax > 0) && (qY <= 0))) {
        qY = MAX_int32_T;
      }
    }

    Out = (int32_T)rt_roundd_snf(0.5 * (real_T)qY);
    fmid = fun(Out);
    do {
      exitg1 = 0;
      qY = ffM - ffm;
      if ((ffM < 0) && ((ffm >= 0) && (qY >= 0))) {
        qY = MIN_int32_T;
      } else {
        if ((ffM >= 0) && ((ffm < 0) && (qY < 0))) {
          qY = MAX_int32_T;
        }
      }

      if (qY > 1.0E-6) {
        if (fmid <= MIN_int32_T) {
          saturatedUnaryMinus = MAX_int32_T;
        } else {
          saturatedUnaryMinus = -fmid;
        }

        if (fmid < 0) {
          c_fmid = saturatedUnaryMinus;
        } else {
          c_fmid = fmid;
        }

        if (c_fmid > 1.0E-6) {
          qY = xmax - xmin;
          if ((xmax < 0) && ((xmin >= 0) && (qY >= 0))) {
            qY = MIN_int32_T;
          } else {
            if ((xmax >= 0) && ((xmin < 0) && (qY < 0))) {
              qY = MAX_int32_T;
            }
          }

          fmid = ffM - ffm;
          if ((ffM < 0) && ((ffm >= 0) && (fmid >= 0))) {
            fmid = MIN_int32_T;
          } else {
            if ((ffM >= 0) && ((ffm < 0) && (fmid < 0))) {
              fmid = MAX_int32_T;
            }
          }

          if (fmid == 0) {
            if (qY == 0) {
              saturatedUnaryMinus = 0;
            } else if (qY < 0) {
              saturatedUnaryMinus = MIN_int32_T;
            } else {
              saturatedUnaryMinus = MAX_int32_T;
            }
          } else if (fmid == 1) {
            saturatedUnaryMinus = qY;
          } else if (fmid == -1) {
            if (qY <= MIN_int32_T) {
              saturatedUnaryMinus = MAX_int32_T;
            } else {
              saturatedUnaryMinus = -qY;
            }
          } else {
            if (qY >= 0) {
              y = (uint32_T)qY;
            } else if (qY == MIN_int32_T) {
              y = 2147483648U;
            } else {
              y = (uint32_T)-qY;
            }

            if (fmid >= 0) {
              b_y = (uint32_T)fmid;
            } else if (fmid == MIN_int32_T) {
              b_y = 2147483648U;
            } else {
              b_y = (uint32_T)-fmid;
            }

            if (b_y == (uint32_T)0) {
              q = MAX_uint32_T;
            } else {
              q = y / b_y;
            }

            y -= q * b_y;
            if ((y > 0U) && (y >= (b_y >> 1U) + (b_y & 1U))) {
              q++;
            }

            saturatedUnaryMinus = (int32_T)q;
            if ((qY < 0) != (fmid < 0)) {
              saturatedUnaryMinus = -saturatedUnaryMinus;
            }
          }

          fmid = mul_s32_s32_s32_sat(saturatedUnaryMinus, ffm);
          Out = xmin - fmid;
          if ((xmin < 0) && ((fmid >= 0) && (Out >= 0))) {
            Out = MIN_int32_T;
          } else {
            if ((xmin >= 0) && ((fmid < 0) && (Out < 0))) {
              Out = MAX_int32_T;
            }
          }

          fmid = fun(Out);
          if (fmid > 0) {
            xmax = Out;
            ffM = fun(Out);
          } else {
            xmin = Out;
            ffm = fun(Out);
          }
        } else {
          exitg1 = 1;
        }
      } else {
        exitg1 = 1;
      }
    } while (exitg1 == 0);
  }

  return Out;
}

/* End of code generation (BFPSolver.c) */
