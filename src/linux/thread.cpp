/*******************************************************************************
Copyright (c) 2018, Jan Koester jan.koester@gmx.net
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

#include "syscall.h"
#include "sysconsole.h"

#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/resource.h>

#include "systhread.h"
#include "sysexception.h"
#include "sysbits.h"

union sigval {
    int sival_int;
    void *sival_ptr;
};

typedef struct {
    int si_signo;
    int si_code;
    union sigval si_value;
    int si_errno;
    int si_pid;
    int si_uid;
    void *si_addr;
    int si_status;
    int si_band;
} siginfo_t;


libsystempp::Thread::Thread(){
  nextThread=nullptr;
  _Thread=0;
}

libsystempp::Thread::~Thread(){
    ::operator delete((void*)_Thread);
    delete nextThread;
}

void libsystempp::Thread::Create(void *function(void*), void *arguments) {
  SystemException excep;
  Console[SYSOUT] << sizeof(function) << Console[SYSOUT].endl;
  _Thread = (long)new void*[65536];
  _Pid = syscall5(__NR_clone,CLONE_SIGHAND|CLONE_FS|CLONE_VM|CLONE_FILES|CLONE_THREAD,
                  _Thread,(long)function,(long)arguments,0);
  if (_Pid > 0) {
    excep[SystemException::Error] << "Can't create thread !";
    throw excep;
  }
}

int libsystempp::Thread::getPid(){
    return _Pid;
}


// void libsystempp::Thread::Detach(){
// //     pthread_detach(_Thread);
// }
//
 
void libsystempp::Thread::Join(){
    siginfo_t pinf;
    struct rusage rusa;
    int ret=syscall5(__NR_waitid,P_PID,_Pid,(long)&pinf,WEXITED|WSTOPPED,(long)&rusa);
    Console[SYSOUT] << _Pid << Console[SYSOUT].endl;
    if(ret > 0){
       SystemException excep;
       excep[SystemException::Error] << "Can't join Thread!";
    }
}
