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

#include <cstddef>

#include "include/utils.h"

const  char *libsystempp::scopy(const char* first, const char* last, char* des){
    while (first != last) {
        *des++ = *first++;
    }
    return des;
}

unsigned int libsystempp::getlen(const char *str) {
    unsigned int len = 0;
    while ((*str++) != '\0') {
        ++len;
    }
    return len;
}

void libsystempp::rscopy(const char* first, const char* last, char** des){
    size_t rs=last-first;
    *des=new char[rs+1];
    scopy(first,last,*des);
    *des[rs]='\0';
}

void libsystempp::reverse(char s[]){
    int i, j;
    char c;
    
    for (i = 0, j = getlen(s)-1; i<j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}    

void libsystempp::itoa(int n, char s[]){
        int i, sign;
        if ((sign = n) < 0) 
            n = -n;
        i = 0;
        do {       /* generate digits in reverse order */
            s[i++] = n % 10 + '0';   /* get next digit */
        } while ((n /= 10) > 0);     /* delete it */
        if (sign < 0)
            s[i++] = '-';
        s[i] = '\0';
        reverse(s);
}

void libsystempp::ultoa(unsigned long n, char s[]){
        unsigned int i, sign;
        i = 0;
        do {       /* generate digits in reverse order */
            s[i++] = n % 10 + '0';   /* get next digit */
        } while ((n /= 10) > 0);     /* delete it */
        s[i] = '\0';
        reverse(s);
}
