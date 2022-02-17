/*******************************************************************************
Copyright (c) 2021, Jan Koester jan.koester@gmx.net
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

#include <cstddef>

#include "config.h"

#pragma once

namespace libsystempp {
   class CharArray {
    public:
        CharArray();
        ~CharArray();

        const char at(std::size_t pos);
        
        void assign(const char *src,std::size_t srcsize);
        void assign(const char *src);
        
        void push_back(const char  src);
        
        void insert(std::size_t pos,char src);
        
        CharArray &operator+=(const char *src);
        CharArray &operator+=(CharArray arr);
        CharArray &operator=(const char *src);
        CharArray &operator=(CharArray arr);
        const char operator[](std::size_t pos);
        
        CharArray &operator<<(const char *src);
        CharArray &operator<<(int src);
        CharArray &operator<<(char src);
        CharArray &operator<<(std::size_t src);

        bool       operator==(const char *src);
        /*C functions*/
        const char        *c_str();
        std::size_t        to_cbuffer(char **buf);
        
        std::size_t        length();
        std::size_t        size();
        void               clear();
        void               shrink();
        void               resize(std::size_t size);
        std::size_t        substr(const char **substring,std::size_t spos,std::size_t size);
        void               substr(CharArray &substring,std::size_t spos,std::size_t size);
     private:
        char             *_c_str;
        char             *_Data;
        std::size_t       _DataSize;
        std::size_t       _ArraySize;
    }; 
};
