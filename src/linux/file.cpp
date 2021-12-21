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

#include <linux/fcntl.h>
#include <linux/types.h>

#include "sysexception.h"
#include "sysfile.h"
#include "syscall.h"
#include "sysbits.h"

libsystempp::FileDescriptor::FileDescriptor(){
}

libsystempp::FileDescriptor::FileDescriptor(int fd){
    _FD=fd;
}

libsystempp::FileDescriptor::~FileDescriptor(){
}

void libsystempp::FileDescriptor::operator=(libsystempp::FileDescriptor value){
    _FD=value._FD;
}


void libsystempp::FileDescriptor::operator=(int value){
    _FD=value;
}

void libsystempp::FileDescriptor::open(const char *path, int opt){
    SystemException excep;
    if(syscall4(__NR_open,(unsigned long)_FD,(unsigned long)path,0,opt)>0)
        throw excep[SystemException::Error] << "Can't open file: " << path;
}

int libsystempp::FileDescriptor::read(void *buf, int bufsize){
    return syscall3(__NR_read,(unsigned long)_FD,(unsigned long)buf,(long)bufsize);
}

int libsystempp::FileDescriptor::write(void* buf, int bufsize){
    return syscall3(__NR_write,_FD,(unsigned long)buf,bufsize);
}

int libsystempp::FileDescriptor::setFcntl(int opt){
    return (int)syscall3(__NR_fcntl, _FD, F_SETFL, opt);
}

int libsystempp::FileDescriptor::getFcntl(){
    return (int)syscall3(__NR_fcntl, _FD, F_GETFL, 0);
}

void libsystempp::FileDescriptor::close(){
    SystemException excep;
    if(syscall1(__NR_open,(unsigned long)_FD)>0)
        throw excep[SystemException::Error] << "Can't close file socket: " << _FD;
}

libsystempp::File::File(const char* path){
}

libsystempp::File::~File(){
}

void libsystempp::File::chown(const char *user, const char* grp){
}

void libsystempp::File::chmod(long perm){
}

void libsystempp::File::touch(long perm){
}

void libsystempp::File::rmfile(){
}

void libsystempp::File::close(){
}

libsystempp::Directory::Directory(){
}

libsystempp::Directory::~Directory(){
}

void libsystempp::Directory::chmod(long perm){
}

void libsystempp::Directory::chown(const char* user, const char* grp){
}

void libsystempp::Directory::rmdir(const char* name){
}
