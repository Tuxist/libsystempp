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

#include <systempp/sysconsole.h>
#include <systempp/sysalloc.h>

#include <config.h>

#include "syscall.h"
#include "sysbits.h"

#define MAP_GROWSDOWN           0x0100     /* stack-like segment */

#define MAP_SHARED              0x01       /* Share changes */
#define MAP_PRIVATE             0x02       /* Changes are private */
#define MAP_SHARED_VALIDATE     0x03

#define MAP_TYPE                0x0f       /* Mask for type of mapping */
#define MAP_FIXED               0x10       /* Interpret addr exactly */
#define MAP_ANONYMOUS           0x20       /* don't use a file */

#define PROT_READ               0x1        /* page can be read */
#define PROT_WRITE              0x2        /* page can be written */
#define PROT_EXEC               0x4        /* page can be executed */
#define PROT_SEM                0x8        /* page may be used for atomic ops */
/*			0x10                              reserved for arch-specific use */
/*			0x20	                          reserved for arch-specific use */
#define PROT_NONE               0x0        /* page can not be accessed */
#define PROT_GROWSDOWN          0x01000000 /* mprotect flag: extend change to start of growsdown vma */
#define PROT_GROWSUP            0x02000000 /* mprotect flag: extend change to end of growsup vma */

sys::allocator& sys::allocator::getInstance(){
   static  allocator instance;
   return instance;
}
  
void *sys::allocator::alloc(unsigned long size){
    _memlock.lock();
    unsigned long blksize=size+sizeof(heap);
    heap *block=(heap*)syscall6(__NR_mmap, 0,blksize, 
                                        PROT_READ | PROT_WRITE,MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    zero(block,sizeof(heap));
    block->_size=size;
    block->_total=blksize;
    block->_block=(char*)block+sizeof(heap);
    if(_lastheap)
        block->_prevheap=_lastheap;
    else
        block->_prevheap=nullptr;
    _lastheap=block;
    _memlock.unlock();
    return _lastheap->_block;
}
    
void *sys::allocator::realloc(void* ptr,unsigned long size){
    unsigned long cursize=0;
    char *oldptr=nullptr;
    
    _memlock.lock();
    for(heap *curstore=_lastheap; curstore; curstore=curstore->_prevheap){
        if((unsigned long)ptr==(unsigned long)curstore->_block){
            oldptr=curstore->_block;
        }
    }
    _memlock.unlock();
    
    char *newptr=(char*)alloc(size);
        
    unsigned long copysize = cursize < size ? cursize : size;
        
    for(unsigned long i=0; i<copysize; ++i){
        newptr[i]=oldptr[i];
    }
        
    free(oldptr);
        
    return newptr;
}
    
void sys::allocator::free(void* ptr){
    _memlock.lock();
    heap *nextheap=nullptr;
    for(heap *curheap=_lastheap; curheap; curheap=curheap->_prevheap){
        if(curheap->_block==ptr){
            if(_lastheap==curheap)
                _lastheap=curheap->_prevheap;
            if(nextheap)
                nextheap->_prevheap=curheap->_prevheap;
            syscall2(__NR_munmap,(unsigned long)curheap,curheap->_total);
            _memlock.unlock();
            return;
        }
        nextheap=curheap;
    }
    _memlock.unlock();
}
    
sys::allocator::allocator(){
    _lastheap=nullptr;
};

sys::allocator::~allocator(){
};
    
void sys::allocator::zero(void *s, unsigned n){
    char *str;
    str = (char *)s;
    while (n--)
        str[n] = 0;
}
    
void *operator new(unsigned long size){
    return sys::allocator::getInstance().alloc(size);
}

void *operator new[](unsigned long size){
    return sys::allocator::getInstance().alloc(size);
}

void operator delete (void* ptr) throw(){
    sys::allocator::getInstance().free(ptr);
}

void operator delete[] (void* ptr) throw(){
    sys::allocator::getInstance().free(ptr);
}

void operator delete(void* ptr,unsigned long amount){
    sys::allocator::getInstance().free(ptr);
}

void operator delete[](void* ptr,unsigned long amount){
    sys::allocator::getInstance().free(ptr);
}
