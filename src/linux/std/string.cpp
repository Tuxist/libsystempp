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

#include <string>
#include <exception>

#include "systempp/sysutils.h"
#include "systempp/sysexception.h"

std::string::string(){
    _Data=nullptr;
    _c_str=nullptr;
    _DataSize=0;
    _ArraySize=0;
}

std::string::~string(){
   delete[]   _Data;
   delete[]   _c_str;
}

const char std::string::at(std::size_t pos){
    sys::SystemException excep;
    if(pos>_DataSize)
        throw excep[sys::SystemException::Error] << "position out of bound!";
    return _Data[pos];
}


void std::string::assign(const char* src, std::size_t srcsize){
    if(srcsize==0)
        return;
    std::size_t nsize=_DataSize+srcsize;
    if(_ArraySize<nsize)
        resize(nsize);
    sys::scopy(src,src+srcsize,_Data+_DataSize);
    _DataSize=nsize;
}

void std::string::push_back(const char src){
    std::size_t nsize=_DataSize+1;
    if(nsize>_ArraySize)
        resize(nsize);
    _Data[_DataSize++]=src;
}

void std::string::assign(const char* src) {
    if(src)
        assign(src,sys::getlen(src));
}

void std::string::insert(std::size_t pos, char src){
    sys::SystemException excp;
    if(pos < _DataSize){
        excp[sys::SystemException::Critical] << "string: out of Bound!";
        throw excp;
    }
    _Data[pos]=src;
}

void std::string::clear(){
    delete[]   _Data;
    delete[]   _c_str;
    _Data=nullptr;
    _c_str=nullptr;
    _DataSize=0;
    _ArraySize=0;
}

std::string &std::string::operator+=(const char *src){
    assign(src);
    return *this;
}

std::string & std::string::operator+=(std::string arr){
    assign(arr.c_str(),arr.length());
    return *this;
}


std::string &std::string::operator=(const char *src){
    clear();
    if(src)
        assign(src,sys::getlen(src));
    return *this;
}

std::string &std::string::operator=(std::string arr){
    clear();
    assign(arr.c_str(),arr.length());
    return *this;
}

const char std::string::operator[](std::size_t pos){
    return at(pos);
}

/*new better behavior*/
std::size_t std::string::to_cbuffer(char ** buf){
    if(_DataSize<=0)
        return 0;
    char *temp=new char[_DataSize+1];
    sys::scopy(_Data,_Data+_DataSize,temp);
    temp[_DataSize]='\0';
    *buf=temp;
    return _DataSize;
}

/*old behavior*/
const char *std::string::c_str() {
    delete[] _c_str;
    _c_str=nullptr;
    if(to_cbuffer(&_c_str)==0)
        _c_str=nullptr;
    return _c_str;
}

std::size_t std::string::size(){
    return _ArraySize;
}

std::size_t std::string::length(){
    return _DataSize;
}

void std::string::shrink(){
    if(_DataSize==0)
        return;
    char *newdata= new char[_DataSize];
    sys::scopy(_Data,_Data+_DataSize,newdata);
    delete[] _Data;
    _Data=newdata;
    _ArraySize=_DataSize;
}

void std::string::resize(std::size_t size){
    sys::SystemException excep;
    if(size<_DataSize)
        throw excep[sys::SystemException::Error] << "string to small not resizing: " 
                                            << _DataSize << " to: " << size;
    
    if(size==0){
        clear();
        return;
    }
    char *newdata = new char[size];
    if(_DataSize>0){
        sys::scopy(_Data,_Data+_DataSize,newdata);
    }
    delete[] _Data;
    _Data=newdata;
    _ArraySize=size;
}

bool std::string::operator==(const char* src){
    if (sys::getlen(src) != _ArraySize)
        return false;
    for(int i=0; i<_ArraySize; ++i){
        if(_Data[i]!=src[i])
            return false;
    }
    return true;
}

std::size_t std::string::substr(const char **substring, std::size_t spos, std::size_t size){
    substring=nullptr;
    if(_DataSize<=0)
        return 0;
    char *ndata=new char[size];
    sys::scopy(_Data+spos,_Data+(spos+size),ndata);
    *substring=ndata;
    return size;
}


void std::string::substr(std::string& substring, std::size_t spos,std::size_t size){
    substring.clear();
    if(_DataSize<=0)
        return;
    substring._DataSize=size;
    substring._ArraySize=size;
    substring._Data=new char[substring._ArraySize];
    sys::scopy(_Data+spos,_Data+(spos+size),substring._Data);
}

