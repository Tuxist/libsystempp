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

#include <linux/sched.h>

#include "include/thread.h"
#include "include/exception.h"

libsystempp::Thread::Thread(){
  _nextThread=nullptr;
  _Thread=0;
}

libsystempp::Thread::~Thread(){
  delete _nextThread;
}

void libsystempp::Thread::Create(void *function(void*), void *arguments) {
  SystemException excep;
  int rth = syscall4(__NR_clone,_Thread, 
                        CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SYSVSEM
                        | CLONE_SIGHAND | CLONE_THREAD
                        | CLONE_SETTLS | CLONE_PARENT_SETTID
                        | CLONE_CHILD_CLEARTID | 0 ,
                     (long)function,(long)arguments);
  if (rth > 0) {
    excep[SystemException::Error] << "Can't create thread !";
    throw excep;
  }
}

// void libsystempp::Thread::Detach(){
// //     pthread_detach(_Thread);
// }
// 
// int libsystempp::Thread::getThreadID() {
// // 	return pthread_self();;
// }
// 
// int libsystempp::Thread::getPid(){
// //   return _Pid;  
// }
// 
// void libsystempp::Thread::setPid(int pid){
// //   _Pid=pid;
// }
// 
void libsystempp::Thread::Join(){
//   if(pthread_join(_Thread,&_Retval)<=0){
//     return;  
//   }else{
//     SystemException excep;  
//     excep[SystemException::Error] << "Can't join Thread!";
//   }
}
