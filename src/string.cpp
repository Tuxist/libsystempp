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


#define SS (sizeof(size_t))
#define ALIGN (sizeof(size_t)-1)
#define ONES ((size_t)-1/UCHAR_MAX)
#define HIGHS (ONES * (UCHAR_MAX/2+1))
#define HASZERO(x) ((x)-ONES & ~(x) & HIGHS)

extern "C" {

    void *memchr(const void *src, int c, size_t n){
        const unsigned char *s =(const unsigned char*) src;
        c = (unsigned char)c;
        for (; ((uintptr_t)s & ALIGN) && n && *s != c; s++, n--);
        if (n && *s != c) {
            const size_t *w;
            size_t k = ONES * c;
            for (w = (const void *)s; n>=SS && !HASZERO(*w^k); w++, n-=SS);
            for (s = (const void *)w; n && *s != c; s++, n--);
        }
        return n ? (void *)s : 0;
    }
    
    void memcpy(void *dest, const void * src, size_t n){
        for (int i=0; i<n; i++)
            ((char *)dest)[i] = ((char*)src)[i];
    };
    
    void memmove(void *dest, void *src, size_t n){  
        char *temp = new char[n];
        for (int i=0; i<n; i++)
            temp[i] = ((char*)src)[i];
  
        for (int i=0; i<n; i++)
            ((char*)dest)[i] = temp[i];
  
        delete [] temp;
    };
    
    void * memset ( void * ptr, int value, size_t num ){
        char *s =(char*) ptr;
        while(num--)
            *s++ = value;
        return s;
    };
    
    size_t strlen(const char *ptr){
        if(!ptr)
            return 0;
        size_t len = 0;
        while ((*ptr++) != '\0') {
            ++len;
        }
        return len;
    }    
    
    int strncmp ( const char * str1, const char * str2, size_t num ){
        const char *l=str1, *r=str2;
        if (!num--) 
            return 0;
        for (; *l && *r && num && *l == *r ; l++, r++, num--);
        return *l - *r;
    }
};
