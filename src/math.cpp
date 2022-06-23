/* origin: FreeBSD /usr/src/lib/msun/src/e_acos.c */
/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunSoft, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */
/* acos(x)
 * Method :
 *      acos(x)  = pi/2 - asin(x)
 *      acos(-x) = pi/2 + asin(x)
 * For |x|<=0.5
 *      acos(x) = pi/2 - (x + x*x^2*R(x^2))     (see asin.c)
 * For x>0.5
 *      acos(x) = pi/2 - (pi/2 - 2asin(sqrt((1-x)/2)))
 *              = 2asin(sqrt((1-x)/2))
 *              = 2s + 2s*z*R(z)        ...z=(1-x)/2, s=sqrt(z)
 *              = 2f + (2c + 2s*z*R(z))
 *     where f=hi part of s, and c = (z-f*f)/(s+f) is the correction term
 *     for f so that f+c ~ sqrt(z).
 * For x<-0.5
 *      acos(x) = pi - 2asin(sqrt((1-|x|)/2))
 *              = pi - 0.5*(s+s*z*R(z)), where z=(1-|x|)/2,s=sqrt(z)
 *
 * Special cases:
 *      if x is NaN, return x itself;
 *      if |x|>1, return NaN with invalid signal.
 *
 * Function needed: sqrt
 */

/* origin: FreeBSD /usr/src/lib/msun/src/e_acos.c */
/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunSoft, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */
/* acos(x)
 * Method :
 *      acos(x)  = pi/2 - asin(x)
 *      acos(-x) = pi/2 + asin(x)
 * For |x|<=0.5
 *      acos(x) = pi/2 - (x + x*x^2*R(x^2))     (see asin.c)
 * For x>0.5
 *      acos(x) = pi/2 - (pi/2 - 2asin(sqrt((1-x)/2)))
 *              = 2asin(sqrt((1-x)/2))
 *              = 2s + 2s*z*R(z)        ...z=(1-x)/2, s=sqrt(z)
 *              = 2f + (2c + 2s*z*R(z))
 *     where f=hi part of s, and c = (z-f*f)/(s+f) is the correction term
 *     for f so that f+c ~ sqrt(z).
 * For x<-0.5
 *      acos(x) = pi - 2asin(sqrt((1-|x|)/2))
 *              = pi - 0.5*(s+s*z*R(z)), where z=(1-|x|)/2,s=sqrt(z)
 *
 * Special cases:
 *      if x is NaN, return x itself;
 *      if |x|>1, return NaN with invalid signal.
 *
 * Function needed: sqrt
 */

#include "math.h"

#define SQRTH 0.70710678118654752440L

/* Get two 32 bit ints from a double.  */
#define EXTRACT_WORDS(hi,lo,d)                    \
do {                                              \
    union {double f; uint64_t i;} __u;              \
    __u.f = (d);                                    \
    (hi) = __u.i >> 32;                             \
    (lo) = (uint32_t)__u.i;                         \
} while (0)

/* Get the more significant 32 bit int from a double.  */
#define GET_HIGH_WORD(hi,d)                       \
do {                                              \
    union {double f; uint64_t i;} __u;              \
    __u.f = (d);                                    \
    (hi) = __u.i >> 32;                             \
} while (0)

/* Get the less significant 32 bit int from a double.  */
#define GET_LOW_WORD(lo,d)                        \
do {                                              \
    union {double f; uint64_t i;} __u;              \
    __u.f = (d);                                    \
    (lo) = (uint32_t)__u.i;                         \
} while (0)

/* Set a double from two 32 bit ints.  */
#define INSERT_WORDS(d,hi,lo)                     \
do {                                              \
    union {double f; uint64_t i;} __u;              \
    __u.i = ((uint64_t)(hi)<<32) | (uint32_t)(lo);  \
    (d) = __u.f;                                    \
} while (0)

/* Set the more significant 32 bits of a double from an int.  */
#define SET_HIGH_WORD(d,hi)                       \
do {                                              \
    union {double f; uint64_t i;} __u;              \
    __u.f = (d);                                    \
    __u.i &= 0xffffffff;                            \
    __u.i |= (uint64_t)(hi) << 32;                  \
    (d) = __u.f;                                    \
} while (0)

/* Set the less significant 32 bits of a double from an int.  */
#define SET_LOW_WORD(d,lo)                        \
do {                                              \
    union {double f; uint64_t i;} __u;              \
    __u.f = (d);                                    \
    __u.i &= 0xffffffff00000000ull;                 \
    __u.i |= (uint32_t)(lo);                        \
    (d) = __u.f;                                    \
} while (0)

/* Get a 32 bit int from a float.  */
#define GET_FLOAT_WORD(w,d)                       \
do {                                              \
    union {float f; uint32_t i;} __u;               \
    __u.f = (d);                                    \
    (w) = __u.i;                                    \
} while (0)

/* Set a float from a 32 bit int.  */
#define SET_FLOAT_WORD(d,w)                         \
do {                                                \
    union {float f; uint32_t i;} __u;               \
    __u.i = (w);                                    \
    (d) = __u.f;                                    \
} while (0)

typedef union
{
  long double value;
  struct
  {
    uint32_t lsw;
    uint32_t msw;
    int sign_exponent:16;
    unsigned int empty1:16;
    unsigned int empty0:32;
  } parts;
} ieee_long_double_shape_type;

#define GET_LDOUBLE_WORDS(exp,ix0,ix1,d)            \
do {                                                \
    ieee_long_double_shape_type ew_u;               \
    ew_u.value = (d);                               \
    (exp) = ew_u.parts.sign_exponent;               \
    (ix0) = ew_u.parts.msw;                         \
    (ix1) = ew_u.parts.lsw;                         \
} while (0)

inline long double __polevll(long double x, const long double *P, int n)
{
	long double y;

	y = *P++;
	do {
		y = y * x + *P++;
	} while (--n);

	return y;
}

/*
 * Polynomial evaluator:
 *  x^n  +  P[0] x^(n-1)  +  P[1] x^(n-2)  +  ...  +  P[n]
 */
inline long double __p1evll(long double x, const long double *P, int n)
{
	long double y;

	n -= 1;
	y = x + *P++;
	do {
		y = y * x + *P++;
	} while (--n);

	return y;
}

extern "C" {
    static const double
    pi      = 3.14159265358979311600e+00, /* 0x400921FB, 0x54442D18 */
    pio2_hi = 1.57079632679489655800e+00; /* 0x3FF921FB, 0x54442D18 */
    static const volatile double
    pio2_lo = 6.12323399573676603587e-17; /* 0x3C91A626, 0x33145C07 */
    static const double
    pS0 =  1.66666666666666657415e-01, /* 0x3FC55555, 0x55555555 */
    pS1 = -3.25565818622400915405e-01, /* 0xBFD4D612, 0x03EB6F7D */
    pS2 =  2.01212532134862925881e-01, /* 0x3FC9C155, 0x0E884455 */
    pS3 = -4.00555345006794114027e-02, /* 0xBFA48228, 0xB5688F3B */
    pS4 =  7.91534994289814532176e-04, /* 0x3F49EFE0, 0x7501B288 */
    pS5 =  3.47933107596021167570e-05, /* 0x3F023DE1, 0x0DFDF709 */
    qS1 = -2.40339491173441421878e+00, /* 0xC0033A27, 0x1C8A2D4B */
    qS2 =  2.02094576023350569471e+00, /* 0x40002AE5, 0x9C598AC8 */
    qS3 = -6.88283971605453293030e-01, /* 0xBFE6066C, 0x1B8D0159 */
    qS4 =  7.70381505559019352791e-02; /* 0x3FB3B8C5, 0xB12E9282 */
    
    static const double tiny = 1.0e-300;
    
    static const long double ln2 = 6.931471805599453094287e-01L; /* 0x3FFE, 0xB17217F7, 0xD1CF79AC */
    
    static const long double C1 = 6.9314575195312500000000E-1L;
    static const long double C2 = 1.4286068203094172321215E-6L;
    
    int __fpclassify(double x){
        union dshape {
            double value;
            uint64_t bits;
        };
        union dshape u = { x };
        int e = u.bits>>52 & 0x7ff;
        if (!e) return u.bits<<1 ? FP_SUBNORMAL : FP_ZERO;
        if (e==0x7ff) return u.bits<<12 ? FP_NAN : FP_INFINITE;
        return FP_NORMAL;
    }
    
    int __fpclassifyf(float x){
        union fshape {
            float value;
            uint32_t bits;
        };
        union fshape u = { x };
        int e = u.bits>>23 & 0xff;
        if (!e) return u.bits<<1 ? FP_SUBNORMAL : FP_ZERO;
        if (e==0xff) return u.bits<<9 ? FP_NAN : FP_INFINITE;
        return FP_NORMAL;
    }

    int __fpclassifyl(long double x){
        union ldshape {
            long double value;
            struct {
                uint64_t m;
                uint16_t exp:15;
                uint16_t sign:1;
                uint16_t pad;
            } bits;
        };
        union ldshape u = { x };
        int e = u.bits.exp;
        if (!e) {
            if (u.bits.m >> 63) return FP_NAN;
            else if (u.bits.m) return FP_SUBNORMAL;
            else return FP_ZERO;
        }
        if (e == 0x7fff)
            return u.bits.m & (uint64_t)-1>>1 ? FP_NAN : FP_INFINITE;
        return u.bits.m & (uint64_t)1<<63 ? FP_NORMAL : FP_NAN;
    }
    
    int sqrt(int x){
        double z;
        int32_t sign = (int)0x80000000;
        int32_t ix0,s0,q,m,t,i;
        uint32_t r,t1,s1,ix1,q1;
        
        EXTRACT_WORDS(ix0, ix1, x);
        
        /* take care of Inf and NaN */
        if ((ix0&0x7ff00000) == 0x7ff00000) {
            return x*x + x;  /* sqrt(NaN)=NaN, sqrt(+inf)=+inf, sqrt(-inf)=sNaN */
        }
        /* take care of zero */
        if (ix0 <= 0) {
            if (((ix0&~sign)|ix1) == 0)
                return x;  /* sqrt(+-0) = +-0 */
                if (ix0 < 0)
                    return (x-x)/(x-x);  /* sqrt(-ve) = sNaN */
        }
        /* normalize x */
        m = ix0>>20;
        if (m == 0) {  /* subnormal x */
            while (ix0 == 0) {
                m -= 21;
                ix0 |= (ix1>>11);
                ix1 <<= 21;
            }
            for (i=0; (ix0&0x00100000) == 0; i++)
                ix0<<=1;
            m -= i - 1;
            ix0 |= ix1>>(32-i);
            ix1 <<= i;
        }
        m -= 1023;    /* unbias exponent */
        ix0 = (ix0&0x000fffff)|0x00100000;
        if (m & 1) {  /* odd m, double x to make it even */
            ix0 += ix0 + ((ix1&sign)>>31);
            ix1 += ix1;
        }
        m >>= 1;      /* m = [m/2] */
        
        /* generate sqrt(x) bit by bit */
        ix0 += ix0 + ((ix1&sign)>>31);
        ix1 += ix1;
        q = q1 = s0 = s1 = 0;  /* [q,q1] = sqrt(x) */
        r = 0x00200000;        /* r = moving bit from right to left */
        
        while (r != 0) {
            t = s0 + r;
            if (t <= ix0) {
                s0   = t + r;
                ix0 -= t;
                q   += r;
            }
            ix0 += ix0 + ((ix1&sign)>>31);
            ix1 += ix1;
            r >>= 1;
        }
        
        r = sign;
        while (r != 0) {
            t1 = s1 + r;
            t  = s0;
            if (t < ix0 || (t == ix0 && t1 <= ix1)) {
                s1 = t1 + r;
                if ((t1&sign) == sign && (s1&sign) == 0)
                    s0++;
                ix0 -= t;
                if (ix1 < t1)
                    ix0--;
                ix1 -= t1;
                q1 += r;
            }
            ix0 += ix0 + ((ix1&sign)>>31);
            ix1 += ix1;
            r >>= 1;
        }
        
        /* use floating add to find out rounding direction */
        if ((ix0|ix1) != 0) {
            z = 1.0 - tiny; /* raise inexact flag */
            if (z >= 1.0) {
                z = 1.0 + tiny;
                if (q1 == (uint32_t)0xffffffff) {
                    q1 = 0;
                    q++;
                } else if (z > 1.0) {
                    if (q1 == (uint32_t)0xfffffffe)
                        q++;
                    q1 += 2;
                } else
                    q1 += q1 & 1;
            }
        }
        ix0 = (q>>1) + 0x3fe00000;
        ix1 = q1>>1;
        if (q&1)
            ix1 |= sign;
        ix0 += m << 20;
        INSERT_WORDS(z, ix0, ix1);
        return z;
    }
    
    double acos(double x){
        double z,p,q,r,w,s,c,df;
        int32_t hx,ix;
        
        GET_HIGH_WORD(hx, x);
        ix = hx & 0x7fffffff;
        if (ix >= 0x3ff00000) {  /* |x| >= 1 */
            uint32_t lx;
            
            GET_LOW_WORD(lx,x);
            if ((ix-0x3ff00000 | lx) == 0) {  /* |x|==1 */
                if (hx > 0) return 0.0;  /* acos(1) = 0  */
                    return pi + 2.0*pio2_lo; /* acos(-1)= pi */
            }
            return (x-x)/(x-x);  /* acos(|x|>1) is NaN */
        }
        if (ix < 0x3fe00000) {   /* |x| < 0.5 */
            if (ix <= 0x3c600000)  /* |x| < 2**-57 */
                return pio2_hi + pio2_lo;
            z = x*x;
            p = z*(pS0+z*(pS1+z*(pS2+z*(pS3+z*(pS4+z*pS5)))));
            q = 1.0+z*(qS1+z*(qS2+z*(qS3+z*qS4)));
            r = p/q;
            return pio2_hi - (x - (pio2_lo-x*r));
        } else if (hx < 0) {     /* x < -0.5 */
            z = (1.0+x)*0.5;
            p = z*(pS0+z*(pS1+z*(pS2+z*(pS3+z*(pS4+z*pS5)))));
            q = 1.0+z*(qS1+z*(qS2+z*(qS3+z*qS4)));
            s = sqrt(z);
            r = p/q;
            w = r*s-pio2_lo;
            return pi - 2.0*(s+w);
        } else {                 /* x > 0.5 */
            z = (1.0-x)*0.5;
            s = sqrt(z);
            df = s;
            SET_LOW_WORD(df,0);
            c  = (z-df*df)/(s+df);
            p = z*(pS0+z*(pS1+z*(pS2+z*(pS3+z*(pS4+z*pS5)))));
            q = 1.0+z*(qS1+z*(qS2+z*(qS3+z*qS4)));
            r = p/q;
            w = r*s+c;
            return 2.0*(df+w);
        }
    }
    
    float sqrtf(float x){
        float z;
        int32_t sign = (int)0x80000000;
        int32_t ix,s,q,m,t,i;
        uint32_t r;
        
        GET_FLOAT_WORD(ix, x);
        
        /* take care of Inf and NaN */
        if ((ix&0x7f800000) == 0x7f800000)
            return x*x + x; /* sqrt(NaN)=NaN, sqrt(+inf)=+inf, sqrt(-inf)=sNaN */
            
            /* take care of zero */
            if (ix <= 0) {
                if ((ix&~sign) == 0)
                    return x;  /* sqrt(+-0) = +-0 */
                    if (ix < 0)
                        return (x-x)/(x-x);  /* sqrt(-ve) = sNaN */
            }
            /* normalize x */
            m = ix>>23;
            if (m == 0) {  /* subnormal x */
                for (i = 0; (ix&0x00800000) == 0; i++)
                    ix<<=1;
                m -= i - 1;
            }
            m -= 127;  /* unbias exponent */
            ix = (ix&0x007fffff)|0x00800000;
            if (m&1)  /* odd m, double x to make it even */
                ix += ix;
            m >>= 1;  /* m = [m/2] */
            
            /* generate sqrt(x) bit by bit */
            ix += ix;
            q = s = 0;       /* q = sqrt(x) */
            r = 0x01000000;  /* r = moving bit from right to left */
            
            while (r != 0) {
                t = s + r;
                if (t <= ix) {
                    s = t+r;
                    ix -= t;
                    q += r;
                }
                ix += ix;
                r >>= 1;
            }
            
            /* use floating add to find out rounding direction */
            if (ix != 0) {
                z = 1.0f - tiny; /* raise inexact flag */
                if (z >= 1.0f) {
                    z = 1.0f + tiny;
                    if (z > 1.0f)
                        q += 2;
                    else
                        q += q & 1;
                }
            }
            ix = (q>>1) + 0x3f000000;
            ix += m << 23;
            SET_FLOAT_WORD(z, ix);
            return z;
    }
    
    long double sqrtl(long double x){
        /* FIXME: implement sqrtl in C. At least this works for now on
        * ARM (which uses ld64), the only arch without sqrtl asm
        * that's supported so far. */
        return sqrt(x);
    }

    
    float acosf(float x){
        float z,p,q,r,w,s,c,df;
        int32_t hx,ix;
        
        GET_FLOAT_WORD(hx, x);
        ix = hx & 0x7fffffff;
        if (ix >= 0x3f800000) {  /* |x| >= 1 */
            if (ix == 0x3f800000) {  /* |x| == 1 */
                if (hx > 0) return 0.0f;  /* acos(1) = 0 */
                    return pi + 2.0f*pio2_lo;  /* acos(-1)= pi */
            }
            return (x-x)/(x-x);  /* acos(|x|>1) is NaN */
        }
        if (ix < 0x3f000000) {   /* |x| < 0.5 */
            if (ix <= 0x32800000) /* |x| < 2**-26 */
                return pio2_hi + pio2_lo;
            z = x*x;
            p = z*(pS0+z*(pS1+z*pS2));
            q = 1.0f+z*qS1;
            r = p/q;
            return pio2_hi - (x - (pio2_lo-x*r));
        } else if (hx < 0) {     /* x < -0.5 */
            z = (1.0f+x)*0.5f;
            p = z*(pS0+z*(pS1+z*pS2));
            q = 1.0f+z*qS1;
            s = sqrtf(z);
            r = p/q;
            w = r*s-pio2_lo;
            return pi - 2.0f*(s+w);
        } else {                 /* x > 0.5 */
            int32_t idf;
            
            z = (1.0f-x)*0.5f;
            s = sqrtf(z);
            df = s;
            GET_FLOAT_WORD(idf,df);
            SET_FLOAT_WORD(df,idf&0xfffff000);
            c  = (z-df*df)/(s+df);
            p = z*(pS0+z*(pS1+z*pS2));
            q = 1.0f+z*qS1;
            r = p/q;
            w = r*s+c;
            return 2.0f*(df+w);
        }        
    }
  
  double nan(const char *s){
        return __builtin_nanf("");
  }
  
  long double frexpl(long double x, int *e){
      union { long double ld; uint16_t hw[5]; } y = { x };
      int ee = y.hw[4]&0x7fff;
      
      if (!ee) {
          if (x) {
              x = frexpl(x*0x1p64, e);
              *e -= 64;
          } else *e = 0;
          return x;
      } else if (ee == 0x7fff) {
          return x;
      }
      
      *e = ee - 0x3ffe;
      y.hw[4] &= 0x8000;
      y.hw[4] |= 0x3ffe;
      return y.ld;
  }
  
  static const long double P[] = {
      4.5270000862445199635215E-5L,
      4.9854102823193375972212E-1L,
      6.5787325942061044846969E0L,
      2.9911919328553073277375E1L,
      6.0949667980987787057556E1L,
      5.7112963590585538103336E1L,
      2.0039553499201281259648E1L,
  };

  static const long double Q[] = {
      /* 1.0000000000000000000000E0,*/
      1.5062909083469192043167E1L,
      8.3047565967967209469434E1L,
      2.2176239823732856465394E2L,
      3.0909872225312059774938E2L,
      2.1642788614495947685003E2L,
      6.0118660497603843919306E1L,
  };
  
  static const long double R[4] = {
      1.9757429581415468984296E-3L,
      -7.1990767473014147232598E-1L,
      1.0777257190312272158094E1L,
      -3.5717684488096787370998E1L,
  };

  static const long double S[4] = {
      /* 1.00000000000000000000E0L,*/
      -2.6201045551331104417768E1L,
      1.9361891836232102174846E2L,
      -4.2861221385716144629696E2L,
  };
  
  long double logl(long double x){
      long double y, z;
      int e;
      
      if (isnan(x))
          return x;
      if (x == INFINITY)
          return x;
      if (x <= 0.0) {
          if (x == 0.0)
              return -INFINITY;
          return __builtin_nanf("");
      }
      
      /* separate mantissa from exponent */
      /* Note, frexp is used so that denormal numbers
       * will be handled properly.
       */
      x = frexpl(x, &e);
      
      /* logarithm using log(x) = z + z**3 P(z)/Q(z),
       * where z = 2(x-1)/x+1)
       */
      if (e > 2 || e < -2) {
          if (x < SQRTH) {  /* 2(2x-1)/(2x+1) */
              e -= 1;
              z = x - 0.5;
              y = 0.5 * z + 0.5;
          } else {  /*  2 (x-1)/(x+1)   */
              z = x - 0.5;
              z -= 0.5;
              y = 0.5 * x  + 0.5;
          }
          x = z / y;
          z = x*x;
          z = x * (z * __polevll(z, R, 3) / __p1evll(z, S, 3));
          z = z + e * C2;
          z = z + x;
          z = z + e * C1;
          return z;
      }
      
      /* logarithm using log(1+x) = x - .5x**2 + x**3 P(x)/Q(x) */
      if (x < SQRTH) {
          e -= 1;
          x = 2.0*x - 1.0;
      } else {
          x = x - 1.0;
      }
      z = x*x;
      y = x * (z * __polevll(x, P, 6) / __p1evll(x, Q, 6));
      y = y + e * C2;
      z = y - 0.5*z;
      /* Note, the sum of above terms does not exceed x/4,
       * so it contributes at most about 1/4 lsb to the error.
       */
      z = z + x;
      z = z + e * C1; /* This sum has an error of 1/2 lsb. */
      return z;
  }
  
  long double log1pl(long double xm1){
      long double x, y, z;
      int e;
      
      if (isnan(xm1))
          return xm1;
      if (xm1 == INFINITY)
          return xm1;
      if (xm1 == 0.0)
          return xm1;
      
      x = xm1 + 1.0;
      
      /* Test for domain errors.  */
      if (x <= 0.0) {
          if (x == 0.0)
              return -INFINITY;
          return NAN;
      }
      
      /* Separate mantissa from exponent.
       *  Use frexp so that denormal numbers will be handled properly.  */
      x = frexpl(x, &e);
      
      /* logarithm using log(x) = z + z^3 P(z)/Q(z),
       *  where z = 2(x-1)/x+1)  */
      if (e > 2 || e < -2) {
          if (x < SQRTH) { /* 2(2x-1)/(2x+1) */
              e -= 1;
              z = x - 0.5;
              y = 0.5 * z + 0.5;
          } else { /*  2 (x-1)/(x+1)   */
              z = x - 0.5;
              z -= 0.5;
              y = 0.5 * x  + 0.5;
          }
          x = z / y;
          z = x*x;
          z = x * (z * __polevll(z, R, 3) / __p1evll(z, S, 3));
          z = z + e * C2;
          z = z + x;
          z = z + e * C1;
          return z;
      }
      
      /* logarithm using log(1+x) = x - .5x**2 + x**3 P(x)/Q(x) */
      if (x < SQRTH) {
          e -= 1;
          if (e != 0)
              x = 2.0 * x - 1.0;
          else
              x = xm1;
      } else {
          if (e != 0)
              x = x - 1.0;
          else
              x = xm1;
      }
      z = x*x;
      y = x * (z * __polevll(x, P, 6) / __p1evll(x, Q, 6));
      y = y + e * C2;
      z = y - 0.5 * z;
      z = z + x;
      z = z + e * C1;
      return z;
  }
  
  long double acoshl(long double x){
      long double t;
      uint32_t se,i0,i1;
      
      GET_LDOUBLE_WORDS(se, i0, i1, x);
      if (se < 0x3fff || se & 0x8000) {  /* x < 1 */
          return (x-x)/(x-x);
      } else if (se >= 0x401d) {  /* x > 2**30 */
          if (se >= 0x7fff)  /* x is inf or NaN */
              return x+x;
          return logl(x) + ln2;  /* acoshl(huge) = logl(2x) */
      } else if (((se-0x3fff)|i0|i1) == 0) {
          return 0.0;            /* acosh(1) = 0 */
      } else if (se > 0x4000) {  /* x > 2 */
          t = x*x;
          return logl(2.0*x - 1.0/(x + sqrtl(t - 1.0)));
      }
      /* 1 < x <= 2 */
      t = x - 1.0;
      return log1pl(t + sqrtl(2.0*t + t*t));
  }
};
