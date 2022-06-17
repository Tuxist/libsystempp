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

#pragma once

namespace sys {
    
    class allocator {
    private:
        class blockstore{
        private:
            blockstore(unsigned long size);
            bool          _Freed;
            unsigned long _Size;
            blockstore   *_prevBlock;
            blockstore   *_nextBlock;
            friend class allocator;
        };
        
        class heap {
        private:
            heap();
            ~heap() noexcept;
            unsigned long  _Total;
            unsigned long  _Free;
            unsigned long  _Count;
            blockstore    *_Block;
            heap          *_prevheap;
            heap          *_nextheap;
            friend class allocator;
        };
    public:
        static allocator& getInstance();
        heap *findunsedheap(unsigned long size);    
        void *alloc(unsigned long size);
        void *realloc(void* ptr,unsigned long size);    
        void  free(void* ptr);
        
        allocator(allocator const&)       = delete;
        void operator=(allocator const&)  = delete;
    private:
        allocator();
        ~allocator();
        void zero(void *s, unsigned n);
        void clearheaps();
        
        heap          *_firstheap;
        heap          *_lastheap;
        blockstore    *_curBlock;
        int            _Count;
    };
};
