/*******************************************************************************
 * Copyright (c) 2021, Jan Koester jan.koester@gmx.net
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

#ifndef SYSCALL_H
#define SYSCALL_H

#ifdef __cplusplus
extern "C" {
#endif
    inline long syscall0(long n) {
        unsigned long ret;
        __asm__ __volatile__ ("syscall" : "=a"(ret) :
                                "a"(n)
                                : "rcx", "r11", "memory");
        return ret;
    }
    
    inline long syscall1(long n,long v1) {
        unsigned long ret;
        __asm__ __volatile__ ("syscall" : "=a"(ret) :
                                "a"(n), "D"(v1)
                                : "rcx", "r11", "memory");
        return ret;
    }

    inline long syscall2(long n,long v1,long v2) { 
        unsigned long ret;
        __asm__ __volatile__ ("syscall" : "=a"(ret) : 
                                "a"(n), "D"(v1), "S"(v2)
                               : "rcx", "r11", "memory");
        return ret;
    }
    
    inline long syscall3(long n,long v1,long v2,long v3) { 
        unsigned long ret;
        __asm__ __volatile__ ("syscall" : "=a"(ret) : 
                                "a"(n), "D"(v1), "S"(v2),"d"(v3)
                               : "rcx", "r11", "memory");
        return ret;
    }
    
    inline long syscall4(long n,long v1,long v2,long v3, long v4) { 
        unsigned long ret;
        register long r10 __asm__("r10") = v4;
        __asm__ __volatile__ ("syscall" : "=a"(ret) : 
                                "a"(n), "D"(v1), "S"(v2),"d"(v3),"r"(r10)
                               : "rcx", "r11", "memory");
        return ret;
    }
    
    inline long syscall5(long n,long v1,long v2,long v3, long v4,long v5) { 
        unsigned long ret;
        register long r10 __asm__("r10") = v4;
        register long r8 __asm__("r8") = v5;
        __asm__ __volatile__ ("syscall" : "=a"(ret) : 
                                "a"(n), "D"(v1), "S"(v2),"d"(v3),"r"(r10),"r"(r8)
                               : "rcx", "r11", "memory");
        return ret;
    }
    
    inline long syscall6(long n,long v1,long v2,long v3, long v4,long v5,long v6) { 
        unsigned long ret;
        register long r10 __asm__("r10") = v4;
        register long r8 __asm__("r8") = v5;
        register long r9 __asm__("r9") = v6;
        __asm__ __volatile__ ("syscall" : "=a"(ret) : 
                                "a"(n), "D"(v1), "S"(v2),"d"(v3),"r"(r10),"r"(r8),"r"(r9)
                               : "rcx", "r11", "memory");
        return ret;
    }
    
#ifdef __cplusplus
};
#endif

#endif
