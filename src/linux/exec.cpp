/*******************************************************************************
Copyright (c) 2022, Jan Koester jan.koester@gmx.net
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

#include <stddef.h>

#include <linux/types.h>
#include <linux/signal.h>
#include <linux/resource.h>
#include <linux/wait.h>

#include "systempp/sysexec.h"
#include "systempp/syscall.h"
#include "systempp/sysbits.h"
#include "systempp/sysexception.h"

sys::Exec::Exec(const char *wrkdir[],const char *filename,const char *argv[]){
    SystemException exception;
    _Pid=syscall0(__NR_fork);
    if(_Pid <0)
        exception[SystemException::Error] << "Exec fork failed !";
    else if(_Pid==0)
        syscall3(__NR_execve,(unsigned long)filename,(unsigned long)argv,(unsigned long)wrkdir);
}

sys::Exec::~Exec(){
}

int sys::Exec::join(){
    struct siginfo sinfo;
    struct rusage rusage;
    if(_Pid!=0)
        return syscall5(__NR_waitid,P_PID,_Pid,(unsigned long)&sinfo,WEXITED,(unsigned long) &rusage);
    return 0;
}
