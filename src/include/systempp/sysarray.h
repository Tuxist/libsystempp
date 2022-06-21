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

#include <string.h>

#pragma once

namespace sys {
    template<class T>
    class array {
    public:
        array operator=(T t) const;
        void push_back(T t);
        void clear();
    };
    template<>
    class array<char> {
    public:
        array(){
            _buf=nullptr;
            _bufsize=0;
        };

        ~array(){
            delete[] _buf;
        }
        
         array operator=(array t){
            clear();
            _buf=new char[t._bufsize];
            memcpy(_buf,t._buf,t._bufsize);
            _bufsize=t._bufsize;
            return *this;
        };
        
        array operator=(const char t){
            clear();
            _buf=new char[1];
            _buf[0]=t;
            _bufsize=1;
            return *this;
        };

        void write(const char *t,unsigned long size){
            clear();
            _bufsize=size;
            if(_bufsize<1)
                return;
            _buf=new char[_bufsize];
            memcpy(_buf,t,size);
        }
        
        array operator=(const char *t){
            unsigned long i;
            if(!t){
                clear();
                return *this;
            }
            for(i = 0; t[i] != '\0'; i++);
            write(t,i);
            return *this;
        };
        
        bool operator==(const char *t){
            if(!_buf)
                return false;
            if(_bufsize!=strlen(t))
                return false;
            if(strncmp(_buf,t,_bufsize)==0)
                return true;
            return false;
        }
        
        char& operator[](int pos){
            return _buf[pos];
        };
        
        unsigned long size(){
            return _bufsize;
        };
        
        void push_back(char t){
            char *temp=new char[_bufsize+1];
            memcpy(temp,_buf,_bufsize);
            temp[_bufsize]=t;
            delete[] _buf;
            _buf=temp;
            ++_bufsize;
        };
        
        void clear(){
            delete[] _buf;
            _bufsize=0;
            _buf=nullptr;
        };
        
        const char *c_str() const{
            char *_cstr=new char[_bufsize+1];
            memcpy(_cstr,_buf,_bufsize);
            _cstr[_bufsize]='\0';
            return _cstr;
        };
        
        size_t length(){
            return _bufsize;
        };
        
        bool empty(){
            if(!_buf)
                return true;
            return false;
        }
        
        sys::array<char> substr(size_t pos, size_t len) const{
            sys::array<char> sustr;
            for(size_t i=pos; i<len; ++i){
                sustr.push_back(_buf[i]);
            }
            return sustr;
        };
        
    private:
        char    *_buf;
        size_t   _bufsize; 
    };
    
    
};
