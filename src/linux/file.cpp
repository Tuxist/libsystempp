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

#include <systempp/sysfile.h>

#include "sysbits.h"
#include "syscall.h"

#define F_GETFD     1   /* get close_on_exec */
#define F_SETFD     2   /* set/clear close_on_exec */
#define F_GETFL     3   /* get file->f_flags */
#define F_SETFL     4   /* set file->f_flags */

sys::file::file(){
}

sys::file::file(int fd){
    _FD=fd;
}

sys::file::~file(){
}

sys::file & sys::file::operator=(sys::file value){
    _FD=value._FD;
    return *this;
}


sys::file & sys::file::operator=(int value){
    _FD=value;
    return *this;
}

void sys::file::open(const char *path, int opt){
}

int sys::file::read(void *buf, int bufsize){
    return syscall3(__NR_read,(unsigned long)_FD,(unsigned long)buf,(long)bufsize);
}

int sys::file::write(void* buf, int bufsize){
    return syscall3(__NR_write,_FD,(unsigned long)buf,bufsize);
}

int sys::file::setFcntl(int opt){
    return (int)syscall3(__NR_fcntl, _FD, F_SETFL, opt);
}

int sys::file::getFcntl(){
    return (int)syscall3(__NR_fcntl, _FD, F_GETFL, 0);
}

void sys::file::close(){
    
}

