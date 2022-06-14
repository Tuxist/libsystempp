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

Allocator& Allocator::getInstance(){
   static  Allocator instance;
   return instance;
}

Allocator::Heap *Allocator::findunsedHeap(unsigned long size){
    for(Heap *curheap=_lastHeap; curheap; curheap=curheap->_prevHeap){
        if(curheap->_Block->_Freed==true && size<curheap->_Block->_Size)
            return curheap;
    }
    return nullptr;
}
    
void *Allocator::alloc(unsigned long size){
    unsigned long fsize=sizeof(Heap)+sizeof(BlockStore)+size;
    Heap *heap=findunsedHeap(size);
    if(!heap)
        heap=(Heap*)syscall6(__NR_mmap, 0,sizeof(Heap)+size+sizeof(BlockStore), 
                             PROT_READ | PROT_WRITE,MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    zero(heap,sizeof(Heap));
    size+=sizeof(BlockStore);
    heap->_Free=size;
    heap->_Count=_Count++;
    heap->_Total=fsize;
    if(heap->_Block)
        heap->_Block->_nextBlock=(BlockStore*)((unsigned long)heap+sizeof(Heap));
    else
        heap->_Block=(BlockStore*)((unsigned long)heap+sizeof(Heap));
    heap->_Block->_Size=size;
    heap->_Block->_Freed=false;
    heap->_Block->_prevBlock=_curBlock;
    if(!_firstHeap){
        _firstHeap=heap;
        _lastHeap=_firstHeap;
    }else{
        _lastHeap=_lastHeap->_nextHeap;
        _lastHeap=heap;
    }
    return (void*)((unsigned long)heap->_Block+sizeof(BlockStore));
}
    
void *Allocator::realloc(void* ptr,unsigned long size){
    unsigned long cursize=0;
    char *oldptr=nullptr;
    for(BlockStore *curstore=_curBlock; curstore; curstore=curstore->_prevBlock){
        if((unsigned long)ptr==(unsigned long)curstore+sizeof(BlockStore)){
            cursize+=curstore->_Size;
            oldptr=(char*)curstore+sizeof(BlockStore);
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
    
void Allocator::free(void* ptr){
    BlockStore *delBlock=nullptr;
    unsigned long _TotalSize=0;

    for(BlockStore *curstore=_curBlock; curstore; curstore=curstore->_prevBlock){
        if((unsigned long)ptr==(unsigned long)curstore+sizeof(BlockStore)){
            curstore->_Freed=true;
            delBlock=curstore;
        }
        _TotalSize+=curstore->_Size;
    }
        
    if(_TotalSize>MAXHEAPSIZE)
        clearHeaps();
}
    
Allocator::Allocator(){
};

Allocator::~Allocator(){
};
    
void Allocator::zero(void *s, unsigned n){
    char *str;
    str = (char *)s;
    while (n--)
        str[n] = 0;
}
    
void Allocator::clearHeaps(){
    for(Heap *curheap=_firstHeap; curheap; curheap=curheap->_nextHeap){
        if(curheap->_Block->_Freed==true){
            curheap->_prevHeap->_nextHeap=curheap->_nextHeap;
            curheap->_nextHeap->_prevHeap=curheap->_prevHeap;
            syscall2(__NR_munmap,(unsigned long)curheap,curheap->_Total);
        }
    }
};

void *operator new(unsigned long size){
    return Allocator::getInstance().alloc(size);
}

void *operator new[](unsigned long size){
    return Allocator::getInstance().alloc(size);
}

void operator delete (void* ptr) throw(){
    Allocator::getInstance().free(ptr);
}

void operator delete[] (void* ptr) throw(){
    Allocator::getInstance().free(ptr);
}

void operator delete(void* ptr,unsigned long amount){
    Allocator::getInstance().free(ptr);
}

void operator delete[](void* ptr,unsigned long amount){
    Allocator::getInstance().free(ptr);
}
