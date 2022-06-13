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

#include <syscall.h>
#include <sysbits.h>

#include <exception>
#include <new>
#include <typeinfo>


namespace std {
    bool uncaught_exception() noexcept { 
        return uncaught_exceptions() > 0; 
    }

    int uncaught_exceptions() noexcept{
//     return __cxa_uncaught_exception() ? 1 : 0;
    }
    
    exception_ptr::~exception_ptr() noexcept {
//   __cxa_decrement_exception_refcount(__ptr_);
    }

    exception_ptr::exception_ptr(const exception_ptr& other) noexcept
        : __ptr_(other.__ptr_){
//     __cxa_increment_exception_refcount(__ptr_);
    }

    exception_ptr& exception_ptr::operator=(const exception_ptr& other) noexcept{
        if (__ptr_ != other.__ptr_){
//          __cxa_increment_exception_refcount(other.__ptr_);
//          __cxa_decrement_exception_refcount(__ptr_);
            __ptr_ = other.__ptr_;
        }
        return *this;
    }

    nested_exception::nested_exception() noexcept
        : __ptr_(current_exception()){
    }

    nested_exception::~nested_exception() noexcept{
    }

    [[noreturn]] void nested_exception::rethrow_nested() const{
        if (__ptr_ == nullptr)
            terminate();
        rethrow_exception(__ptr_);
    }

    exception_ptr current_exception() noexcept{
        // be nicer if there was a constructor that took a ptr, then
        // this whole function would be just:
        //    return exception_ptr(__cxa_current_primary_exception());
        exception_ptr ptr;
//      ptr.__ptr_ = __cxa_current_primary_exception();
        return ptr;
}

    [[noreturn]] void rethrow_exception(exception_ptr p){
//      __cxa_rethrow_primary_exception(p.__ptr_);
    // if p.__ptr_ is NULL, above returns so we terminate
        terminate();
    }

    [[noreturn]] void terminate() noexcept{
        syscall1(__NR_exit,-1);
    };
}

