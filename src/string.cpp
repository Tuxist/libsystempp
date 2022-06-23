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
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#define SS (sizeof(size_t))
#define ALIGN (sizeof(size_t)-1)
#define ONES ((size_t)-1/UCHAR_MAX)
#define HIGHS (ONES * (UCHAR_MAX/2+1))
#define HASZERO(x) ((x)-ONES & ~(x) & HIGHS)

#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))

#define BITOP(a,b,op) \
 ((a)[(size_t)(b)/(8*sizeof *(a))] op (size_t)1<<((size_t)(b)%(8*sizeof *(a))))

template<class tmemcmp>
int tmemcmp(tmemcmp vl, tmemcmp vr, size_t n){
    const unsigned char *l=(const unsigned char *)vl;
    const unsigned char *r=(const unsigned char *)vr;
    for (; n && *l == *r; n--, l++, r++);
    return n ? *l-*r : 0;
}

template<class rtmemchr,class tmemchr>
rtmemchr tpmemchr(tmemchr src, int c, size_t n){
        const unsigned char *s =(const unsigned char*) src;
        c = (unsigned char)c;
        for (; ((uintptr_t)s & ALIGN) && n && *s != c; s++, n--);
        if (n && *s != c) {
            const size_t *w;
            size_t k = ONES * c;
            for (w = (size_t*)(const void *)s; n>=SS && !HASZERO(*w^k); w++, n-=SS);
            for (s = (unsigned char*)(const void *)w; n && *s != c; s++, n--);
        }
        return n ? (rtmemchr)s : 0;
}

template<class rtmemchr,class tmemchr>
rtmemchr tpmemrchr(tmemchr src, int c, size_t n){
    const unsigned char *s = (const unsigned char *) src;
    c = (unsigned char)c;
    while (n--) if (s[n]==c) 
        return (rtmemchr)(s+n);
    return 0;
}

template<class r,class t>
r twobyte_strstr(t h,t n){
    uint16_t nw = n[0]<<8 | n[1], hw = h[0]<<8 | h[1];
    for (h++; *h && hw != nw; hw = hw<<8 | *++h);
    return *h ? (r)h-1 : 0;
}

template<class r,class t>
r threebyte_strstr(t h, t n){
    uint32_t nw = n[0]<<24 | n[1]<<16 | n[2]<<8;
    uint32_t hw = h[0]<<24 | h[1]<<16 | h[2]<<8;
    for (h+=2; *h && hw != nw; hw = (hw|*++h)<<8);
    return *h ? (r)h-2 : 0;
}

template<class r,class t>
r fourbyte_strstr(t h, t n){
    uint32_t nw = n[0]<<24 | n[1]<<16 | n[2]<<8 | n[3];
    uint32_t hw = h[0]<<24 | h[1]<<16 | h[2]<<8 | h[3];
    for (h+=3; *h && hw != nw; hw = hw<<8 | *++h);
    return *h ? (r)h-3 : 0;
}



template<class r,class t>
r twoway_strstr(t h, t n){
     const unsigned char *z;
     size_t l, ip, jp, k, p, ms, p0, mem, mem0;
     size_t byteset[32 / sizeof(size_t)] = { 0 };
     size_t shift[256];
     
     /* Computing length of needle and fill shift table */
     for (l=0; n[l] && h[l]; l++)
         BITOP(byteset, n[l], |=), shift[n[l]] = l+1;
     if (n[l]) return 0; /* hit the end of h */
         
         /* Compute maximal suffix */
         ip = -1; jp = 0; k = p = 1;
     while (jp+k<l) {
         if (n[ip+k] == n[jp+k]) {
             if (k == p) {
                 jp += p;
                 k = 1;
             } else k++;
         } else if (n[ip+k] > n[jp+k]) {
             jp += k;
             k = 1;
             p = jp - ip;
         } else {
             ip = jp++;
             k = p = 1;
         }
     }
     ms = ip;
     p0 = p;
     
     /* And with the opposite comparison */
     ip = -1; jp = 0; k = p = 1;
     while (jp+k<l) {
         if (n[ip+k] == n[jp+k]) {
             if (k == p) {
                 jp += p;
                 k = 1;
             } else k++;
         } else if (n[ip+k] < n[jp+k]) {
             jp += k;
             k = 1;
             p = jp - ip;
         } else {
             ip = jp++;
             k = p = 1;
         }
     }
     if (ip+1 > ms+1) ms = ip;
     else p = p0;
     
     /* Periodic needle? */
     if (memcmp(n, n+p, ms+1)) {
         mem0 = 0;
         p = MAX(ms, l-ms-1) + 1;
     } else mem0 = l-p;
     mem = 0;
     
     /* Initialize incremental end-of-haystack pointer */
     z = (const unsigned char *) h;
     
     /* Search loop */
     for (;;) {
         /* Update incremental end-of-haystack pointer */
         if (z-(const unsigned char *)h < l) {
             /* Fast estimate for MIN(l,63) */
             size_t grow = l | 63;
             const unsigned char *z2 = (const unsigned char*)tpmemchr<const void*>(z, 0, grow);
             if (z2) {
                 z = z2;
                 if (z-(const unsigned char *)h < l) return 0;
             } else z += grow;
         }
         
         /* Check last byte first; advance by shift on mismatch */
         if (BITOP(byteset, h[l-1], &)) {
             k = l-shift[h[l-1]];
             //printf("adv by %zu (on %c) at [%s] (%zu;l=%zu)\n", k, h[l-1], h, shift[h[l-1]], l);
             if (k) {
                 if (mem0 && mem && k < p) k = l-p;
                 h += k;
                 mem = 0;
                 continue;
             }
         } else {
             h += l;
             mem = 0;
             continue;
         }
         
         /* Compare right half */
         for (k=MAX(ms+1,mem); n[k] && n[k] == h[k]; k++);
         if (n[k]) {
             h += k-ms;
             mem = 0;
             continue;
         }
         /* Compare left half */
         for (k=ms+1; k>mem && n[k-1] == h[k-1]; k--);
         if (k == mem) return (r)h;
         h += p;
         mem = mem0;
     }
}

extern "C" {

    void *memchr(const void *src, int c, size_t n){
        return tpmemchr<void*,const void*>(src,c,n);
    }

    void *memrchr(const void *src, int c, size_t n){
        return tpmemrchr<void*,const void*>(src,c,n);
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
    
    int memcmp(const void *vl, const void *vr, size_t n){
        return tmemcmp<const void*>(vl,vr,n);
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
    
    char *strchr(const char *s, int c){
        return tpmemchr<char*,const char*>(s,c,strlen(s));
    }
    
    char *strrchr(const char *s, int c){
        return tpmemrchr<char*,const char*>(s,c,strlen(s)+1);
    }
    
    char * strstr ( const char * str1, const char *str2){
        /* Return immediately on empty needle */
        if (!str2[0]) 
            return (char *)str1;
        
        /* Use faster algorithms for short needles */
        str1 = strchr(str1, *str2);
        if (!str1 || !str2[1]) 
            return (char *)str1;
        if (!str1[1]) 
            return 0;
        if (!str1[2]) 
            return twobyte_strstr<char*,const char*>(str1,str2);
        if (!str1[2]) 
            return 0;
        if (!str1[3]) 
            return threebyte_strstr<char*,const char*>(str1,str2);
        if (!str1[3]) 
            return 0;
        if (!str1[4]) 
            return fourbyte_strstr<char *,const char *>(str1,str2);
        
        return twoway_strstr<char *,const char *>(str1,str2);
    }
    
    size_t strcspn(const char *s, const char *c){
        const char *a = s;
        size_t byteset[32/sizeof(size_t)];
        
        if (!c[0]) 
            return strlen(s);
        if (!c[1]) 
            return (s=strchr(s, *c)) ? s-a : strlen(a);
        
        memset(byteset, 0, sizeof byteset);
        for (; *c && BITOP(byteset, *(unsigned char *)c, |=); c++);
        for (; *s && !BITOP(byteset, *(unsigned char *)s, &); s++);
        return s-a;
    }

    
    char *strpbrk(const char *s, const char *b){
        s += strcspn(s, b);
        return *s ? (char *)s : 0;
    }
};
