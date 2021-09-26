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

#include "include/array.h"
#include "include/utils.h"
#include "include/exception.h"

libsystempp::CharArray::CharArray(){
    _InitArray();
}

libsystempp::CharArray::~CharArray(){
   clear();
}

void libsystempp::CharArray::assign(const char* src, unsigned long srcsize){
    unsigned long nsize=_DataSize+srcsize;
    char *buf=new char [nsize+1];
    unsigned long i=0;
    scopy(_Data,_Data+_DataSize,buf);
    scopy(src,src+srcsize,buf+_DataSize);
    _DataSize=nsize;
    delete[] _Data;
    buf[nsize]='\0';
    _Data=buf;
}

void libsystempp::CharArray::push_back(const char src){
    unsigned long nsize=_DataSize+1;
    char *buf=new char [nsize+2];
    unsigned long i;
    for(i = 0; i<_DataSize; ++i){
        buf[i]=_Data[i];
    }
    buf[i]=src;
    buf[nsize]='\0';
    _DataSize=nsize;
    delete[] _Data;
    _Data=buf;
}

void libsystempp::CharArray::assign(const char* src) {
    assign(src,getlen(src));
}

void libsystempp::CharArray::insert(unsigned long pos, char src){
    SystemException excp;
    if(pos < _DataSize){
        excp[SystemException::Critical] << "HtmlString: out of String";
        throw excp;
    }
    _Data[pos]=src;
}

void libsystempp::CharArray::clear(){
    delete[]   _Data;
    _InitArray();
}

libsystempp::CharArray &libsystempp::CharArray::operator+=(const char *src){
    assign(src);
    return *this;
}

libsystempp::CharArray & libsystempp::CharArray::operator+=(libsystempp::CharArray arr){
    this->assign(arr.c_str());
    return *this;
}


libsystempp::CharArray &libsystempp::CharArray::operator=(const char *src){
    clear();
    _DataSize=getlen(src);
    _Data = new char[_DataSize];
    for(unsigned long i = 0; i<_DataSize; ++i){
        _Data[i]=src[i];
    } 
    return *this;
}

libsystempp::CharArray &libsystempp::CharArray::operator=(libsystempp::CharArray arr){
    clear();
    scopy(arr._Data,arr._Data+arr._DataSize,this->_Data);
    this->_DataSize=arr._DataSize;
    return *this;
}

char &libsystempp::CharArray::operator[](unsigned long pos){
    return _Data[pos];
}

libsystempp::CharArray &libsystempp::CharArray::operator<<(const char* src){
    assign(src);
    return *this;
}

libsystempp::CharArray &libsystempp::CharArray::operator<<(int src){
    char *buf=new char[sizeof(int)+1];
    itoa(src,buf);
    assign(buf);
    delete[] buf;
    return *this;
}

libsystempp::CharArray &libsystempp::CharArray::operator<<(unsigned long src){
    char *buf=new char[sizeof(int)+1];
    ultoa(src,buf);
    assign(buf);
    delete[] buf;
    return *this;
}

libsystempp::CharArray &libsystempp::CharArray::operator<<(char src){
    push_back(src);
    return *this;
}

const char *libsystempp::CharArray::c_str() {
    return _Data;
}

unsigned long libsystempp::CharArray::size(){
    return _DataSize;
}
  

void libsystempp::CharArray::_InitArray(){
    _Data=nullptr;
    _DataSize=0;
} 
