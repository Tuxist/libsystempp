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

#include "include/file.h"
#include "include/exception.h"
#include "include/utils.h"
#include "include/console.h"
#include <unistd.h>

template<typename OUT>
void ConsoleWrite(int type,libsystempp::FileDescriptor *sout,
                           libsystempp::FileDescriptor *eout, 
                           OUT out,int size){
    switch(type){
        case SOUT:
            sout->write((void*)out,size);
            break;
        case SERR:
            eout->write((void*)out,size);
            break;
        default:
            break;
    }
}

libsystempp::_Console::_Console(){
    _OutFD = new FileDescriptor(STDOUT_FILENO);
    _ErrFD = new FileDescriptor(STDERR_FILENO);
}

libsystempp::_Console::~_Console(){
    delete _OutFD;
    delete _ErrFD;
}

libsystempp::_Console &libsystempp::_Console::endl(){
    ConsoleWrite(_OutType,_OutFD,_ErrFD,"\n",1);
    return *this;
}

libsystempp::_Console &libsystempp::_Console::operator<< (const char* out){
    if(!out)
        return *this;
    ConsoleWrite(_OutType,_OutFD,_ErrFD,out,getlen(out));
    return *this;    
}

libsystempp::_Console &libsystempp::_Console::operator<< (int out){
    char buf[255];
    itoa(out,buf);
    ConsoleWrite(_OutType,_OutFD,_ErrFD,buf,getlen(buf));
    return *this;
}

libsystempp::_Console &libsystempp::_Console::operator<< (unsigned long out){
    char buf[255];
    ultoa(out,buf);
    ConsoleWrite(_OutType,_OutFD,_ErrFD,buf,getlen(buf));
    return *this;
}

libsystempp::_Console &libsystempp::_Console::operator<< (unsigned int out){
    char buf[255];
    ultoa(out,buf);
    ConsoleWrite(_OutType,_OutFD,_ErrFD,buf,getlen(buf));
    return *this;
}

libsystempp::_Console &libsystempp::_Console::operator<< (char out){
    ConsoleWrite(_OutType,_OutFD,_ErrFD,&out,sizeof(char));
    return *this;
}

libsystempp::_Console & libsystempp::_Console::operator[](int out){
    _OutType=out;
    return *this;
}


libsystempp::_Console & libsystempp::_Console::operator<<(libsystempp::_Console& console){
    return console;
}

