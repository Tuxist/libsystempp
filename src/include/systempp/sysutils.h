/*******************************************************************************
Copyright (c) 2018, Jan Koester jan.koester@gmx.net
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

#include "config.h"

#pragma once

namespace sys {
   const char *scopy(const char* first, const char* last, char* des);
   unsigned int getlen(const char *str);
   void rscopy(const char* first, const char* last, char** des);
   void reverse(char s[]);
   int atoi (const char* nptr);
   long long atoll(const char* nptr);
   void itoa(int n, char s[]);
   void ultoa(unsigned long n, char s[]);
   void zero(void *s, unsigned n);
   unsigned long atoul(char* str);
   int ncompare(const char *src,unsigned long ssize,const char *comp,unsigned long csize);
   unsigned long append(char** src, const char* append);
   unsigned long append(char** src, const char append);
   unsigned long cleannewline(const char *src,unsigned long srcsize,char **dest);
   int substr(const char *src,char **dest,unsigned long spos,unsigned long endpos);
   int rfind(const char *src,unsigned long len,const char find);
};
