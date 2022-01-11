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

#include "sysarray.h"
#include "sysutils.h"
#include "sysexception.h"

libsystempp::CharArray::CharArray(){
    _Data=nullptr;
    _c_str=nullptr;
    _DataSize=0;
    _ArraySize=0;
}

libsystempp::CharArray::~CharArray(){
   delete[]   _Data;
   delete[]   _c_str;
}

void libsystempp::CharArray::assign(const char* src, unsigned long srcsize){
    if(srcsize==0)
        return;
    unsigned long nsize=_DataSize+srcsize;
    if(_ArraySize<nsize)
        resize(nsize);
    scopy(src,src+srcsize,_Data+_DataSize);
    _DataSize=nsize;
}

void libsystempp::CharArray::push_back(const char src){
    unsigned long nsize=_DataSize+1;
    if(nsize>_ArraySize)
        resize(nsize);
    _Data[_DataSize++]=src;
}

void libsystempp::CharArray::assign(const char* src) {
    if(src)
        assign(src,getlen(src));
}

void libsystempp::CharArray::insert(unsigned long pos, char src){
    SystemException excp;
    if(pos < _DataSize){
        excp[SystemException::Critical] << "CharArray: out of Bound!";
        throw excp;
    }
    _Data[pos]=src;
}

void libsystempp::CharArray::clear(){
    delete[]   _Data;
    delete[]   _c_str;
    _Data=nullptr;
    _c_str=nullptr;
    _DataSize=0;
    _ArraySize=0;
}

libsystempp::CharArray &libsystempp::CharArray::operator+=(const char *src){
    assign(src);
    return *this;
}

libsystempp::CharArray & libsystempp::CharArray::operator+=(libsystempp::CharArray arr){
    assign(arr.c_str(),arr.length());
    return *this;
}


libsystempp::CharArray &libsystempp::CharArray::operator=(const char *src){
    clear();
    if(src)
        assign(src,getlen(src));
    return *this;
}

libsystempp::CharArray &libsystempp::CharArray::operator=(libsystempp::CharArray arr){
    clear();
    assign(arr.c_str(),arr.length());
    return *this;
}

const char libsystempp::CharArray::operator[](unsigned long pos){
    SystemException excep;
    if(pos>_DataSize)
        throw excep[SystemException::Error] << "position out of bound!";
    return _Data[pos];
}

libsystempp::CharArray &libsystempp::CharArray::operator<<(const char* src){
    assign(src);
    return *this;
}

libsystempp::CharArray &libsystempp::CharArray::operator<<(int src){
    char buf[255];
    itoa(src,buf);
    assign(buf);
    return *this;
}

libsystempp::CharArray &libsystempp::CharArray::operator<<(unsigned long src){
    char buf[512];
    ultoa(src,buf);
    assign(buf);
    return *this;
}

libsystempp::CharArray &libsystempp::CharArray::operator<<(char src){
    push_back(src);
    return *this;
}

/*new better behavior*/
unsigned long libsystempp::CharArray::to_cbuffer(char ** buf){
    if(_DataSize<=0)
        return 0;
    char *temp=new char[_DataSize+1];
    scopy(_Data,_Data+_DataSize,temp);
    temp[_DataSize]='\0';
    *buf=temp;
    return _DataSize;
}

/*old behavior*/
const char *libsystempp::CharArray::c_str() {
    delete[] _c_str;
    _c_str=nullptr;
    if(to_cbuffer(&_c_str)==0)
        _c_str=nullptr;
    return _c_str;
}

unsigned long libsystempp::CharArray::size(){
    return _ArraySize;
}

unsigned long libsystempp::CharArray::length(){
    return _DataSize;
}

void libsystempp::CharArray::shrink(){
    if(_DataSize==0)
        return;
    char *newdata= new char[_DataSize];
    scopy(_Data,_Data+_DataSize,newdata);
    delete[] _Data;
    _Data=newdata;
    _ArraySize=_DataSize;
}

void libsystempp::CharArray::resize(unsigned long size){
    SystemException excep;
    if(size<_DataSize)
        throw excep[SystemException::Error] << "CharArray to small not resizing: " 
                                            << _DataSize << " to: " << size;
    
    if(size==0){
        clear();
        return;
    }
    char *newdata = new char[size];
    if(_DataSize>0){
        scopy(_Data,_Data+_DataSize,newdata);
    }
    delete[] _Data;
    _Data=newdata;
    _ArraySize=size;
}

bool libsystempp::CharArray::operator==(const char* src){
    if (getlen(src) != _ArraySize)
        return false;
    for(int i=0; i<_ArraySize; ++i){
        if(_Data[i]!=src[i])
            return false;
    }
    return true;
}

void libsystempp::CharArray::substr(libsystempp::CharArray& substring, unsigned long spos,unsigned long size){
    substring.clear();
    if(_DataSize<=0)
        return;
    substring._DataSize=size;
    substring._Data=new char[substring._DataSize];
    scopy(_Data+spos,_Data+size,substring._Data);
}
