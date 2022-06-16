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

sys::allocator::heap *sys::allocator::findunsedheap(unsigned long size){
    for(heap *curheap=_lastheap; curheap; curheap=curheap->_prevheap){
        if(curheap->_Block->_Freed==true && size<curheap->_Block->_Size)
            return curheap;
    }
    return nullptr;
}
    
void *sys::allocator::alloc(unsigned long size){
    unsigned long fsize=sizeof(heap)+sizeof(blockstore)+size;
    heap *myheap=findunsedheap(size);
    if(!myheap)
        myheap=(allocator::heap*)syscall6(__NR_mmap, 0,sizeof(heap)+size+sizeof(blockstore), 
                                        PROT_READ | PROT_WRITE,MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    zero(myheap,sizeof(heap));
    size+=sizeof(blockstore);
    myheap->_Free=size;
    myheap->_Count=_Count++;
    myheap->_Total=fsize;
    myheap->_Block=(blockstore*)((unsigned long)myheap+sizeof(heap));
    myheap->_Block->_Size=size;
    myheap->_Block->_Freed=false;
    myheap->_Block->_prevBlock=_curBlock;
    if(!_firstheap){
        _firstheap=myheap;
        _lastheap=_firstheap;
    }else{
        _lastheap=_lastheap->_nextheap;
        _lastheap=myheap;
    }
    return (void*)((unsigned long)myheap->_Block+sizeof(blockstore));
}
    
void *sys::allocator::realloc(void* ptr,unsigned long size){
    unsigned long cursize=0;
    char *oldptr=nullptr;
    for(blockstore *curstore=_curBlock; curstore; curstore=curstore->_prevBlock){
        if((unsigned long)ptr==(unsigned long)curstore+sizeof(blockstore)){
            cursize+=curstore->_Size;
            oldptr=(char*)curstore+sizeof(blockstore);
        }
    }
        
    char *newptr=(char*)alloc(size);
        
    unsigned long copysize = cursize < size ? cursize : size;
        
    for(unsigned long i=0; i<copysize; ++i){
        newptr[i]=oldptr[i];
    }
        
    free(oldptr);
        
    return newptr;
}
    
void sys::allocator::free(void* ptr){
    blockstore *delBlock=nullptr;
    unsigned long _TotalSize=0;

    for(blockstore *curstore=_curBlock; curstore; curstore=curstore->_prevBlock){
        if((unsigned long)ptr==(unsigned long)curstore+sizeof(blockstore)){
            curstore->_Freed=true;
            delBlock=curstore;
        }
        _TotalSize+=curstore->_Size;
    }
        
    if(_TotalSize>MAXHEAPSIZE)
        clearheaps();
}
    
sys::allocator::allocator(){
    _lastheap=nullptr;
    _firstheap=nullptr;
};

sys::allocator::~allocator(){
};
    
void sys::allocator::zero(void *s, unsigned n){
    char *str;
    str = (char *)s;
    while (n--)
        str[n] = 0;
}
    
void sys::allocator::clearheaps(){
    for(heap *curheap=_firstheap; curheap; curheap=curheap->_nextheap){
        if(curheap->_Block->_Freed==true){
            curheap->_prevheap->_nextheap=curheap->_nextheap;
            curheap->_nextheap->_prevheap=curheap->_prevheap;
            syscall2(__NR_munmap,(unsigned long)curheap,curheap->_Total);
        }
    }
};

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
