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

#include "../include/file.h"
#include "syscall.h"

libsystempp::FileDescriptor::FileDescriptor(){
}

libsystempp::FileDescriptor::FileDescriptor(int fd){
    _FileDescriptor=fd;
}

libsystempp::FileDescriptor::~FileDescriptor(){
}

libsystempp::FileDescriptor & libsystempp::FileDescriptor::operator=(libsystempp::FileDescriptor value){
    _FileDescriptor=value._FileDescriptor;
    return *this;
}


libsystempp::FileDescriptor & libsystempp::FileDescriptor::operator=(int value){
    _FileDescriptor=value;
    return *this;
}

void libsystempp::FileDescriptor::open(const char *path, int opt){
}

int libsystempp::FileDescriptor::read(void *buf, int blocksize){
    return syscall3(__NR_read,_FileDescriptor,(long)buf,(long)blocksize);
}

int libsystempp::FileDescriptor::write(void* buf, int bufsize){
    return syscall3(__NR_write,_FileDescriptor,(long)buf,bufsize);
}

int libsystempp::FileDescriptor::fnctl(int opt){
    return 0;
}

void libsystempp::FileDescriptor::close(){
    
}
