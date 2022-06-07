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

#include <new>
#include "atomic_support.h"

#include <linux/mman.h>
#include <config.h>

#include "syscall.h"
#include "sysbits.h"

class Allocator {
private:
    class BlockStore{
    private:
        BlockStore(unsigned long size){
            _Size=size;
            _prevBlock=nullptr;
            _nextBlock=nullptr;
        };
        bool          _Freed;
        unsigned long _Size;
        BlockStore   *_prevBlock;
        BlockStore   *_nextBlock;
        friend class Allocator;
    };
    
    class Heap {
        private:
        Heap(){
            _prevHeap=nullptr;
            _nextHeap=nullptr;
        };
        ~Heap(){};
        unsigned long  _Total;
        unsigned long  _Free;
        unsigned long  _Count;
        BlockStore    *_Block;
        Heap          *_prevHeap;
        Heap          *_nextHeap;
        friend class Allocator;
    };
public:
    
    static Allocator& getInstance(){
            static  Allocator instance;
            return instance;
    }

    Heap *findunsedHeap(unsigned long size){
        for(Heap *curheap=_lastHeap; curheap; curheap=curheap->_prevHeap){
            if(curheap->_Block->_Freed==true && size<curheap->_Block->_Size)
                return curheap;
        }
        return nullptr;
    }
    
    void *sysalloc(unsigned long size){
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
    
    void sysfree(void* ptr){
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
    
    Allocator(Allocator const&)       = delete;
    void operator=(Allocator const&)  = delete;
private:
    Allocator(){};

    ~Allocator(){};
    
    void zero(void *s, unsigned n){
        char *str;
        str = (char *)s;
        while (n--)
            str[n] = 0;
    }
    
    void clearHeaps(){
       for(Heap *curheap=_firstHeap; curheap; curheap=curheap->_nextHeap){
            if(curheap->_Block->_Freed==true){
                curheap->_prevHeap->_nextHeap=curheap->_nextHeap;
                curheap->_nextHeap->_prevHeap=curheap->_prevHeap;
                syscall2(__NR_munmap,(unsigned long)curheap,curheap->_Total);
            }
        }
    };
    
    Heap          *_firstHeap;
    Heap          *_lastHeap;
    BlockStore    *_curBlock;
    int            _Count;
};


namespace std {

    const nothrow_t nothrow{};

    void __throw_bad_alloc(){
        _VSTD::abort();
    }
}

_LIBCPP_WEAK
void *
operator new(std::size_t size) _THROW_BAD_ALLOC
{

    return Allocator::getInstance().sysalloc(size);
}

_LIBCPP_WEAK
void*
operator new(size_t size, const std::nothrow_t&) noexcept
{
    void* p = nullptr;
#ifndef _LIBCPP_NO_EXCEPTIONS
    try
    {
#endif // _LIBCPP_NO_EXCEPTIONS
        p = ::operator new(size);
#ifndef _LIBCPP_NO_EXCEPTIONS
    }
    catch (...)
    {
    }
#endif // _LIBCPP_NO_EXCEPTIONS
    return p;
}

_LIBCPP_WEAK
void*
operator new[](size_t size) _THROW_BAD_ALLOC
{
    return ::operator new(size);
}

_LIBCPP_WEAK
void*
operator new[](size_t size, const std::nothrow_t&) noexcept
{
    void* p = nullptr;
#ifndef _LIBCPP_NO_EXCEPTIONS
    try
    {
#endif // _LIBCPP_NO_EXCEPTIONS
        p = ::operator new[](size);
#ifndef _LIBCPP_NO_EXCEPTIONS
    }
    catch (...)
    {
    }
#endif // _LIBCPP_NO_EXCEPTIONS
    return p;
}

_LIBCPP_WEAK
void
operator delete(void* ptr) noexcept
{
    ::Allocator::getInstance().sysfree(ptr);
}

_LIBCPP_WEAK
void
operator delete(void* ptr, const std::nothrow_t&) noexcept
{
    ::operator delete(ptr);
}

_LIBCPP_WEAK
void
operator delete(void* ptr, size_t) noexcept
{
    ::operator delete(ptr);
}

_LIBCPP_WEAK
void
operator delete[] (void* ptr) noexcept
{
    ::operator delete(ptr);
}

_LIBCPP_WEAK
void
operator delete[] (void* ptr, const std::nothrow_t&) noexcept
{
    ::operator delete[](ptr);
}

_LIBCPP_WEAK
void
operator delete[] (void* ptr, size_t) noexcept
{
    ::operator delete[](ptr);
}

#if !defined(_LIBCPP_HAS_NO_LIBRARY_ALIGNED_ALLOCATION)

_LIBCPP_WEAK
void *
operator new(std::size_t size, std::align_val_t alignment) _THROW_BAD_ALLOC
{
    if (size == 0)
        size = 1;
    if (static_cast<size_t>(alignment) < sizeof(void*))
      alignment = std::align_val_t(sizeof(void*));

    // Try allocating memory. If allocation fails and there is a new_handler,
    // call it to try free up memory, and try again until it succeeds, or until
    // the new_handler decides to terminate.
    //
    // If allocation fails and there is no new_handler, we throw bad_alloc
    // (or return nullptr if exceptions are disabled).
    void* p;
    while ((p = std::__libcpp_aligned_alloc(static_cast<std::size_t>(alignment), size)) == nullptr)
    {
        std::new_handler nh = std::get_new_handler();
        if (nh)
            nh();
        else {
#ifndef _LIBCPP_NO_EXCEPTIONS
            throw std::bad_alloc();
#else
            break;
#endif
        }
    }
    return p;
}

_LIBCPP_WEAK
void*
operator new(size_t size, std::align_val_t alignment, const std::nothrow_t&) noexcept
{
    void* p = nullptr;
#ifndef _LIBCPP_NO_EXCEPTIONS
    try
    {
#endif // _LIBCPP_NO_EXCEPTIONS
        p = ::operator new(size, alignment);
#ifndef _LIBCPP_NO_EXCEPTIONS
    }
    catch (...)
    {
    }
#endif // _LIBCPP_NO_EXCEPTIONS
    return p;
}

_LIBCPP_WEAK
void*
operator new[](size_t size, std::align_val_t alignment) _THROW_BAD_ALLOC
{
    return ::operator new(size, alignment);
}

_LIBCPP_WEAK
void*
operator new[](size_t size, std::align_val_t alignment, const std::nothrow_t&) noexcept
{
    void* p = nullptr;
#ifndef _LIBCPP_NO_EXCEPTIONS
    try
    {
#endif // _LIBCPP_NO_EXCEPTIONS
        p = ::operator new[](size, alignment);
#ifndef _LIBCPP_NO_EXCEPTIONS
    }
    catch (...)
    {
    }
#endif // _LIBCPP_NO_EXCEPTIONS
    return p;
}

_LIBCPP_WEAK
void
operator delete(void* ptr, std::align_val_t) noexcept
{
    std::__libcpp_aligned_free(ptr);
}

_LIBCPP_WEAK
void
operator delete(void* ptr, std::align_val_t alignment, const std::nothrow_t&) noexcept
{
    ::operator delete(ptr, alignment);
}

_LIBCPP_WEAK
void
operator delete(void* ptr, size_t, std::align_val_t alignment) noexcept
{
    ::operator delete(ptr, alignment);
}

_LIBCPP_WEAK
void
operator delete[] (void* ptr, std::align_val_t alignment) noexcept
{
    ::operator delete(ptr, alignment);
}

_LIBCPP_WEAK
void
operator delete[] (void* ptr, std::align_val_t alignment, const std::nothrow_t&) noexcept
{
    ::operator delete[](ptr, alignment);
}

_LIBCPP_WEAK
void
operator delete[] (void* ptr, size_t, std::align_val_t alignment) noexcept
{
    ::operator delete[](ptr, alignment);
}

#endif 
