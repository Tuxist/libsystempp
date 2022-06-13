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

#include "stddef.h"

#pragma once

// FILE
typedef unsigned long fpos_t;
// size_t

#ifdef __cplusplus
extern "C" {
#endif

struct FILE {
    
};

int remove(const char* filename);
int rename(const char* oldn, const char* newn);
FILE* tmpfile(void);
char* tmpnam(char* s);
int fclose(FILE* stream);
int fflush(FILE* stream);
FILE* fopen(const char* __restrict__ filename, const char* __restrict__ mode);
FILE* freopen(const char* __restrict__ filename, const char * __restrict__ mode,
              FILE * __restrict__ stream);
void setbuf(FILE* __restrict__ stream, char* __restrict__ buf);
int setvbuf(FILE* __restrict__ stream, char* __restrict__ buf, int mode, size_t size);
int fprintf(FILE* __restrict__ stream, const char* __restrict__ format, ...);
int fscanf(FILE* __restrict__ stream, const char * __restrict__ format, ...);
int printf(const char* __restrict__ format, ...);
int scanf(const char* __restrict__ format, ...);
int snprintf(char* __restrict__ s, size_t n, const char* __restrict__ format, ...);    // C99
int sprintf(char* __restrict__ s, const char* __restrict__ format, ...);
int sscanf(const char* __restrict__ s, const char* __restrict__ format, ...);
int vfprintf(FILE* __restrict__ stream, const char* __restrict__ format, va_list arg);
int vfscanf(FILE* __restrict__ stream, const char* __restrict__ format, va_list arg);  // C99
int vprintf(const char* __restrict__ format, va_list arg);
int vscanf(const char* __restrict__ format, va_list arg);                          // C99
int vsnprintf(char* __restrict__ s, size_t n, const char* __restrict__ format,         // C99
              va_list arg);
int vsprintf(char* __restrict__ s, const char* __restrict__ format, va_list arg);
int vsscanf(const char* __restrict__ s, const char* __restrict__ format, va_list arg); // C99
int fgetc(FILE* stream);
char* fgets(char* __restrict__ s, int n, FILE* __restrict__ stream);
int fputc(int c, FILE* stream);
int fputs(const char* __restrict__ s, FILE* __restrict__ stream);
int getc(FILE* stream);
int getchar(void);
char* gets(char* s);  // removed in C++14
int putc(int c, FILE* stream);
int putchar(int c);
int puts(const char* s);
int ungetc(int c, FILE* stream);
size_t fread(void* __restrict__ ptr, size_t size, size_t nmemb,
             FILE* __restrict__ stream);
size_t fwrite(const void* __restrict__ ptr, size_t size, size_t nmemb,
              FILE* __restrict__ stream);
int fgetpos(FILE* __restrict__ stream, fpos_t* __restrict__ pos);
int fseek(FILE* stream, long offset, int whence);
int fsetpos(FILE*stream, const fpos_t* pos);
long ftell(FILE* stream);
void rewind(FILE* stream);
void clearerr(FILE* stream);
int feof(FILE* stream);
int ferror(FILE* stream);
void perror(const char* s);

#ifdef __cplusplus
}
#endif
