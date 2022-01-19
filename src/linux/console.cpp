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

#include "sysexception.h"
#include "sysutils.h"
#include "sysconsole.h"

libsystempp::FileDescriptor STDIN(SYSIN);
libsystempp::FileDescriptor STDOUT(SYSOUT);
libsystempp::FileDescriptor STDERR(SYSERR);


libsystempp::_Console CONERR(STDERR);
libsystempp::_Console CONOUT(STDOUT);
libsystempp::_Console CONIN(STDIN);

const char *libsystempp::_Console::endl="\n";

libsystempp::_Console::_Console(){
}

libsystempp::_Console::_Console(FileDescriptor &fd){
    _FD=fd;
}

libsystempp::_Console::~_Console(){
}

libsystempp::_Console &libsystempp::_Console::operator<< (const char* out){
    if(!out)
        return *this;
    _FD.write((void*)out,getlen(out));
    return *this;    
}

libsystempp::_Console & libsystempp::_Console::operator<<(libsystempp::CharArray out){
    operator<<(out.c_str()); 
    return *this;
}

libsystempp::_Console &libsystempp::_Console::operator<< (int out){
    char buf[255];
    itoa(out,buf);
    _FD.write((void*)buf,getlen(buf));
    return *this;
}

libsystempp::_Console &libsystempp::_Console::operator<< (unsigned long out){
    char buf[255];
    ultoa(out,buf);
    _FD.write(buf,getlen(buf));
    return *this;
}

libsystempp::_Console &libsystempp::_Console::operator<< (unsigned int out){
    char buf[255];
    ultoa(out,buf);
    _FD.write(buf,getlen(buf));
    return *this;
}

libsystempp::_Console &libsystempp::_Console::operator<< (char out){
    _FD.write(&out,sizeof(char));
    return *this;
}

libsystempp::_Console & libsystempp::_Console::operator<<(libsystempp::_Console& console){
    return console;
}

libsystempp::_Console & libsystempp::_Console::operator>>(char ** in){
    char input;
    while(_FD.read(&input, 1) > 0){
        if(input=='\n')
            break;
        append(in,input);
    }
    return *this;
}

libsystempp::_Console & libsystempp::_Console::operator>>(libsystempp::CharArray& in){
    char input;
    while(_FD.read(&input, 1) > 0){
        if(input=='\n')
            break;
        in.push_back(input);
    }
    return *this;
}
