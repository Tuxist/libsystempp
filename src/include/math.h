/*******************************************************************************
 * Copyright (c) 2022, Jan Koester jan.koester@gmx.net
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 * Neither the name of the <organization> nor the
 *      names of its contributors may be used to endorse or promote products
 *      derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************/

#include <stdint.h>

#pragma once

#define NAN       __builtin_nanf("")
#define INFINITY  __builtin_inff()

#define FP_NAN       0
#define FP_INFINITE  1
#define FP_ZERO      2
#define FP_SUBNORMAL 3
#define FP_NORMAL    4

union __float_repr { float __f; uint32_t __i; };
union __double_repr { double __f; uint64_t __i; };

#define __FLOAT_BITS(f) (((union __float_repr){ (float)(f) }).__i)
#define __DOUBLE_BITS(f) (((union __double_repr){ (double)(f) }).__i)

#define fpclassify(x) ( \
    sizeof(x) == sizeof(float) ? __fpclassifyf(x) : \
    sizeof(x) == sizeof(double) ? __fpclassify(x) : \
    __fpclassifyl(x) )

#define isinf(x) ( \
    sizeof(x) == sizeof(float) ? (__FLOAT_BITS(x) & 0x7fffffff) == 0x7f800000 : \
    sizeof(x) == sizeof(double) ? (__DOUBLE_BITS(x) & (uint64_t)-1>>1) == (uint64_t)0x7ff<<52 : \
    __fpclassifyl(x) == FP_INFINITE)

#define isnan(x) ( \
    sizeof(x) == sizeof(float) ? (__FLOAT_BITS(x) & 0x7fffffff) > 0x7f800000 : \
    sizeof(x) == sizeof(double) ? (__DOUBLE_BITS(x) & (uint64_t)-1>>1) > (uint64_t)0x7ff<<52 : \
    __fpclassifyl(x) == FP_NAN)

#define isnormal(x) ( \
    sizeof(x) == sizeof(float) ? ((__FLOAT_BITS(x)+0x00800000) & 0x7fffffff) >= 0x01000000 : \
    sizeof(x) == sizeof(double) ? ((__DOUBLE_BITS(x)+((uint64_t)1<<52)) & (uint64_t)-1>>1) >= (__uint64_t)1<<53 : \
    __fpclassifyl(x) == FP_NORMAL)

#define isfinite(x) ( \
    sizeof(x) == sizeof(float) ? (__FLOAT_BITS(x) & 0x7fffffff) < 0x7f800000 : \
    sizeof(x) == sizeof(double) ? (__DOUBLE_BITS(x) & (uint64_t)-1>>1) < (uint64_t)0x7ff<<52 : \
    __fpclassifyl(x) > FP_INFINITE)

#define signbit(x) ( \
    sizeof(x) == sizeof(float) ? (int)(__FLOAT_BITS(x)>>31) : \
    sizeof(x) == sizeof(double) ? (int)(__DOUBLE_BITS(x)>>63) : \
    __signbitl(x) )

#define isunordered(x,y) (isnan((x)) ? ((void)(y),1) : isnan((y)))

#ifdef __cplusplus
extern "C" {
#endif
    int __fpclassify(double);
    int __fpclassifyf(float);
    int __fpclassifyl(long double);

    int sqrt(int x);
    float sqrtf(float x);
    long double sqrtl(long double x);
    double acos(double x);
    float acosf(float x);
    long double frexpl(long double x, int *e);
    long double logl(long double x);
    long double acoshl(long double x);
    /*tommorow implement*/
    long double acosl(long double);
    float asinf(float);
    long double asinl(long double);
    
    
#ifdef __cplusplus
}
#endif
