/*******************************************************************************
Copyright (c) 2019, Jan Koester jan.koester@gmx.net
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

#include <cstdlib>

#include <systempp/syssignal.h>

#include "include/alloc.h"

void* std::calloc(std::size_t nmemb, std::size_t size){
    return Allocator::getInstance().sysalloc((nmemb*size));
};

void std::free(void* ptr){
    Allocator::getInstance().sysfree(ptr);
};

void* std::malloc(std::size_t size){
    return Allocator::getInstance().sysalloc(size);
};

void* std::realloc(void* ptr, std::size_t size){
    return Allocator::getInstance().sysrealloc(ptr,size);
};

void std::abort(void){
};

extern "C"{
    void* calloc(std::size_t nmemb, std::size_t size){
       return std::calloc(nmemb,size); 
    };
    
    void free(void* ptr){
        std::free(ptr);
    };
    
    void *malloc(std::size_t size){
        return std::malloc(size);
    };
    
    void *realloc(void* ptr, std::size_t size){
        return std::realloc(ptr,size);
    };
    
    void abort(void){
        std::abort();
    };
};
