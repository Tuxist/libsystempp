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

#include "syscall.h"
#include "sysconsole.h"

#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/resource.h>
#include <linux/mman.h>

#include "systhread.h"
#include "sysexception.h"
#include "sysbits.h"

#define STACKSIZE 4096

class libsystempp::ThInfo {
private:
    int *_Ptid;
    int *_Ctid;
    friend class libsystempp::Thread;
};

libsystempp::Thread::Thread(){
  _ThInfo = new ThInfo();
  _Stack=nullptr;
  nextThread=nullptr;
}

libsystempp::Thread::~Thread(){
    delete _ThInfo;
    delete nextThread;
}

void libsystempp::Thread::Create(void *function(void*), void *arguments) {
    SystemException excep;
    
    _Stack =  (char*)syscall6(__NR_mmap,0,STACKSIZE, PROT_READ | PROT_WRITE,
                 MAP_PRIVATE | MAP_ANONYMOUS | MAP_GROWSDOWN, -1, 0);
    
    if ((unsigned long)_Stack == -1){
        excep[SystemException::Error] << "Can't create thread !";
        throw excep;
    }
  
    int pid = syscall5(__NR_clone,CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND |
                    CLONE_PARENT | CLONE_THREAD | CLONE_IO | 0 ,(unsigned long)&_Stack+STACKSIZE,
                     (unsigned long)_ThInfo->_Ptid,(unsigned long)_ThInfo->_Ctid,0);

    if(pid == -1){
       SystemException excep;
       excep[SystemException::Error] << "Can't create Thread!";
    }
    
    if (pid == 0) {
        function(arguments);
        return;
    }
}


void libsystempp::Thread::Join(){
    struct rusage rusa;
    int state=0;
//     int ret=syscall4(__NR_wait4,_ThInfo->_Ctid,(long)&state,0,(long)&rusa);
//     if(ret < 0){
//        SystemException excep;
//        excep[SystemException::Error] << "Can't join Thread!";
//     }
    for(;;);
}
