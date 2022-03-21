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

#include "stddef.h"

#pragma once

#define EXIT_FAILURE 1 /* Failing exit status.  */
#define EXIT_SUCCESS 0 /* Successful exit status.  */

typedef void* div_t;
typedef void* ldiv_t;
typedef void* lldiv_t;

#ifdef __cplusplus
extern "C" {
#endif
    void          _Exit(int);
    long          a64l(const char *);
    void          abort(void);
    int           abs(int);
    void         *aligned_alloc( size_t alignment, size_t size );
    int           atexit(void (*)(void));
    double        atof(const char *);
    int           atoi(const char *);
    long          atol(const char *);
    long long     atoll(const char *);
    int           at_quick_exit(void (*func)(void));
    void         *bsearch(const void *, const void *, size_t, size_t,
                          int (*)(const void *, const void *));
    void         *calloc(size_t, size_t);
    div_t         div(int, int);
    double        drand48(void);
    double        erand48(unsigned short[3]);
    void          exit(int);
    void          free(void *);
    char         *getenv(const char *);
    int           getsubopt(char **, char *const *, char **);
    int           grantpt(int);
    char         *initstate(unsigned, char *, size_t);
    long          jrand48(unsigned short[3]);
    char         *l64a(long);
    long          labs(long);
    void          lcong48(unsigned short[7]);
    ldiv_t        ldiv(long, long);
    long long     llabs(long long);
    lldiv_t       lldiv(long long, long long);
    long          lrand48(void);
    void         *malloc(size_t);
    int           mblen(const char *, size_t);
    size_t        mbstowcs(wchar_t *restrict, const char *restrict2, size_t);
    int           mbtowc(wchar_t *restrict, const char *restrict2, size_t);
    int           mkstemp(char *);
    long          mrand48(void);
    long          nrand48(unsigned short[3]);
    int           posix_memalign(void **, size_t, size_t);
    int           posix_openpt(int);
    char         *ptsname(int);
    int           putenv(char *);
    void          quick_exit( int exit_code );
    void          qsort(void *, size_t, size_t, int (*)(const void *,
                                                        const void *));
    int           rand(void);
    int           rand_r(unsigned *);
    long          random(void);
    void         *realloc(void *, size_t);
    char         *realpath(const char *restrict, char *wrestrict2);
    unsigned short seed48(unsigned short[3]);
    int           setenv(const char *, const char *, int);
    void          setkey(const char *);
    char         *setstate(const char *);
    void          srand(unsigned);
    void          srand48(long);
    void          srandom(unsigned);
    double        strtod(const char *restrict, char **restrict2);
    float         strtof(const char *restrict, char **restrict2);
    long          strtol(const char *restrict, char **restrict2, int);
    long double   strtold(const char *restrict, char **restrict2);
    long long     strtoll(const char *restrict, char **restrict2, int);
    unsigned long strtoul(const char *restrict, char **restrict2, int);
    unsigned long long
    strtoull(const char *restrict, char **restrict2, int);
    int           system(const char *);
    int           unlockpt(int);
    int           unsetenv(const char *);
    size_t        wcstombs(char *restrict, const wchar_t *restrict2, size_t);
    int           wctomb(char *, wchar_t);
#ifdef __cplusplus
}
#endif
