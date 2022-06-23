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

#include <stddef.h>

extern "C" {
    wchar_t *wmemchr(const wchar_t *s, wchar_t c, size_t n){
        for (; n && *s != c; n--, s++);
        return n ? (wchar_t *)s : 0;
    }
    
    size_t wcslen(const wchar_t *s){
        const wchar_t *a;
        for (a=s; *s; s++);
        return s-a;
    }
    
    size_t wcsnlen(const wchar_t *s, size_t n){
        const wchar_t *z = wmemchr(s, 0, n);
        if (z) n = z-s;
        return n;
    }
    
    wchar_t *wcschr(const wchar_t *s, wchar_t c){
        if (!c) return (wchar_t *)s + wcslen(s);
        for (; *s && *s != c; s++);
        return *s ? (wchar_t *)s : 0;
    }
    
    size_t wcscspn(const wchar_t *s, const wchar_t *c){
        const wchar_t *a;
        if (!c[0]) return wcslen(s);
        if (!c[1]) return (s=wcschr(a=s, *c)) ? s-a : wcslen(a);
        for (a=s; *s && !wcschr(c, *s); s++);
        return s-a;
    }
    
    wchar_t *wcspbrk(const wchar_t *s, const wchar_t *b){
        s += wcscspn(s, b);
        return *s ? (wchar_t *)s : nullptr;
    }
    
    int wmemcmp(const wchar_t *l, const wchar_t *r, size_t n){
        for (; n && *l==*r; n--, l++, r++);
        return n ? *l-*r : 0;
    }
    
    wchar_t *wcsrchr(const wchar_t *s, wchar_t c){
        const wchar_t *p;
        for (p=s+wcslen(s); p>=s && *p!=c; p--);
        return p>=s ? (wchar_t *)p : 0;
    }
    
    inline wchar_t *naive_wcsstr(const wchar_t *h, const wchar_t *n){
        size_t i;
        for (i=0; n[i] && h[i]; i++)
            for (   ; n[i] != h[i]; h++, i=0);
        return n[i] ? 0 : (wchar_t *)h;
    }
    
    #define MAX(a,b) ((a)>(b)?(a):(b))
    #define MIN(a,b) ((a)<(b)?(a):(b))
    
    inline wchar_t *twoway_wcsstr(const wchar_t *h, const wchar_t *n){
        const wchar_t *z;
        size_t l, ip, jp, k, p, ms, p0, mem, mem0;
        
        /* Computing length of needle */
        for (l=0; n[l] && h[l]; l++);
        if (n[l]) 
            return 0; /* hit the end of h */
            
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
        if (wmemcmp(n, n+p, ms+1)) {
            mem0 = 0;
            p = MAX(ms, l-ms-1) + 1;
        } else mem0 = l-p;
        mem = 0;
        
        /* Initialize incremental end-of-haystack pointer */
        z = h;
        
        /* Search loop */
        for (;;) {
            /* Update incremental end-of-haystack pointer */
            if (z-h < l) {
                /* Fast estimate for MIN(l,63) */
                size_t grow = l | 63;
                const wchar_t *z2 = wmemchr(z, 0, grow);
                if (z2) {
                    z = z2;
                    if (z-h < l) return 0;
                } else z += grow;
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
            if (k == mem) return (wchar_t *)h;
            h += p;
            mem = mem0;
        }
    }
    
    wchar_t *wcsstr(const wchar_t *h, const wchar_t *n){
        /* Return immediately on empty needle or haystack */
        if (!n[0]) return (wchar_t *)h;
        if (!h[0]) return 0;
        
        /* Use faster algorithms for short needles */
        h = wcschr(h, *n);
        if (!h || !n[1]) return (wchar_t *)h;
        if (!h[1]) return 0;
        if (!n[2] || !n[3] || !n[4]) return naive_wcsstr(h, n);
        
        return twoway_wcsstr(h, n);
    }
};
