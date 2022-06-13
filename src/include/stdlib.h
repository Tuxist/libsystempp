/*******************************************************************************
Copyright (c) 2022, Jan Koester jan.koester@gmx.net
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/

#include <stddef.h>

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct {
        int quot;
        int rem;
    } div_t;

    typedef struct {
        long int quot;
        long int rem;
    } ldiv_t;

    typedef struct {
        long long quot;
        long long rem;
    } lldiv_t;

    double atof(const char *s);
    int atoi(const char *s);
    long atol(const char *s);
    long long atoll( const char *str );
    
    double strtod(const char *s, char **endp);
    long strtol(const char *s, char **endp, int base);
    long double strtold (const char* str, char** endptr);
    unsigned long strtoul(const char *s, char **endp, int base);
    long long int strtoll (const char* str, char** endptr, int base);
    unsigned long long int strtoull (const char* str, char** endptr, int base);
    float strtof (const char* str, char** endptr);
    
    int rand (void);
    void srand (unsigned int seed);
    
    void* calloc (size_t num, size_t size);
    void free (void* ptr);
    void* malloc (size_t size);
    void* realloc (void* ptr, size_t size);
    void *aligned_alloc( size_t alignment, size_t size );
    
    void abort(void);
    int atexit(void (*fcn)(void));
    int at_quick_exit (void (*func)(void));
    char* getenv (const char* name);
    _Noreturn void quick_exit (int status);
    int system (const char* command);
    void exit(int status);
    void _Exit(int status);
    
    void* bsearch (const void* key, const void* base,
               size_t num, size_t size,
               int (*compar)(const void*,const void*));
    
    void qsort (void* base, size_t num, size_t size,
            int (*compar)(const void*,const void*));
    
    int abs (int n);
    div_t div(int num, int denom);
    long int labs (long int n);
    ldiv_t ldiv(long num, long denom);
    long long int llabs (long long int n);
    lldiv_t lldiv (long long int numer, long long int denom);
    
    int mblen (const char* pmb, size_t max);
    int mbtowc (wchar_t* pwc, const char* pmb, size_t max);
    int wctomb (char* pmb, wchar_t wc);
    
    size_t mbstowcs (wchar_t* dest, const char* src, size_t max);
    size_t wcstombs (char* dest, const wchar_t* src, size_t max);
    
    
    
#ifdef __cplusplus
}
#endif
