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

#include "stdarg.h"
#include "stddef.h"

#pragma once

typedef void* fpos_t;
typedef void* off_t;
typedef void* FILE;

#ifdef __cplusplus
extern "C" {
#endif
    void *stderr;
    void *stdout;
    
    void     clearerr(FILE *);
    char    *ctermid(char *);
    int      fclose(FILE *);
    FILE    *fdopen(int, const char *);
    int      feof(FILE *);
    int      ferror(FILE *);
    int      fflush(FILE *);
    int      fgetc(FILE *);
    int      fgetpos(FILE *, fpos_t *);
    char    *fgets(char *, int, FILE *);
    int      fileno(FILE *);
    void     flockfile(FILE *);
    FILE    *fopen(const char *, const char *);
    int      fprintf(FILE *, const char *, ...);
    int      fputc(int, FILE *);
    int      fputs(const char *, FILE *);
    size_t   fread(void *, size_t, size_t, FILE *);
    FILE    *freopen(const char *, const char *, FILE *);
    int      fscanf(FILE *, const char *, ...);
    int      fseek(FILE *, long int, int);
    int      fseeko(FILE *, off_t, int);
    int      fsetpos(FILE *, const fpos_t *);
    long int ftell(FILE *);
    off_t    ftello(FILE *);
    int      ftrylockfile(FILE *);
    void     funlockfile(FILE *);
    size_t   fwrite(const void *, size_t, size_t, FILE *);
    int      getc(FILE *);
    int      getchar(void);
    int      getc_unlocked(FILE *);
    int      getchar_unlocked(void);
    char    *gets(char *);
    int      getw(FILE *);
    int      pclose(FILE *);
    void     perror(const char *);
    FILE    *popen(const char *, const char *);
    int      printf(const char *, ...);
    int      putc(int, FILE *);
    int      putchar(int);
    int      putc_unlocked(int, FILE *);
    int      putchar_unlocked(int);
    int      puts(const char *);
    int      putw(int, FILE *);
    int      remove(const char *);
    int      rename(const char *, const char *);
    void     rewind(FILE *);
    int      scanf(const char *, ...);
    void     setbuf(FILE *, char *);
    int      setvbuf(FILE *, char *, int, size_t);
    int      snprintf(char *, size_t, const char *, ...);
    int      sprintf(char *, const char *, ...);
    int      sscanf(const char *, const char *, int n,...);
    char    *tempnam(const char *, const char *);
    FILE    *tmpfile(void);
    char    *tmpnam(char *);
    int      ungetc(int, FILE *);
    int      vfprintf(FILE *, const char *, va_list);
    int      vprintf(const char *, va_list);
    int      vsnprintf(char *, size_t, const char *, va_list);
    int      vsprintf(char *, const char *, va_list);
    int      vsscanf ( const char * s, const char * format, va_list arg );
#ifdef __cplusplus
}
#endif
