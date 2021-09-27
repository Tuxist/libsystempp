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

#include "sysutils.h"
#include "sysexception.h"

#include "confdb_internal.h"

libsystempp::ConfData::ConfData(){
    _Parent=nullptr;
    _prevConfData=nullptr;
    _nextConfData=nullptr;
}

libsystempp::ConfData::~ConfData(){
    delete _prevConfData;
    delete _nextConfData;
}


void libsystempp::ConfData::setKey(class CharArray *key){
    _Key=*key;
}

void libsystempp::ConfData::setKey(const char *key){
    _Key=key;
}

const char  *libsystempp::ConfData::getKey(){
    return _Key.c_str();
}
        
void libsystempp::ConfData::setValue(class CharArray *value){
    _Value=*value;
}

void libsystempp::ConfData::setValue(const char *value){
    _Value=value;        
}

void libsystempp::ConfData::setValue(int value){
    char buf[255];
    itoa(value,buf);
    _Value=buf;
}

void libsystempp::ConfData::setValue(unsigned int value){
    char buf[512];
    ultoa(value,buf);
    _Value=buf;    
}
        
const char*  libsystempp::ConfData::getValue(){
    return _Value.c_str();
}
        
int libsystempp::ConfData::getValueInt(){
    SystemException excep;
    char buf[255];
    if(_Value.size()>255)
        throw excep[SystemException::Error] << "ConfData::getValueInt: cannot convert to int";
    scopy(_Value.c_str(),_Value.c_str()+_Value.size(),buf);
    return atoi(buf);
}
        
unsigned int libsystempp::ConfData::getValueUInt(){
    SystemException excep;
    char buf[512];
    if(_Value.size()>512)
        throw excep[SystemException::Error] << "ConfData::getValueInt: cannot convert to int";
    scopy(_Value.c_str(),_Value.c_str()+_Value.size(),buf);
    return atoul(buf);           
}
