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

#include "sysbits.h"
#include "syscall.h"

#include "systempp/systhread.h"
#include "systempp/syssleep.h"
#include "systempp/sysexception.h"

#include <pthread.h>

#define MAP_GROWSDOWN           0x0100     /* stack-like segment */

#define MAP_SHARED              0x01       /* Share changes */
#define MAP_PRIVATE             0x02       /* Changes are private */
#define MAP_SHARED_VALIDATE     0x03

#define MAP_TYPE                0x0f       /* Mask for type of mapping */
#define MAP_FIXED               0x10       /* Interpret addr exactly */
#define MAP_ANONYMOUS           0x20       /* don't use a file */

#define PROT_READ               0x1        /* page can be read */
#define PROT_WRITE              0x2        /* page can be written */
#define PROT_EXEC               0x4        /* page can be executed */
#define PROT_SEM                0x8        /* page may be used for atomic ops */
/*			0x10                              reserved for arch-specific use */
/*			0x20	                          reserved for arch-specific use */
#define PROT_NONE               0x0        /* page can not be accessed */
#define PROT_GROWSDOWN          0x01000000 /* mprotect flag: extend change to start of growsdown vma */
#define PROT_GROWSUP            0x02000000 /* mprotect flag: extend change to end of growsup vma */

#define CSIGNAL                 0x000000ff /* signal mask to be sent at exit */
#define CLONE_VM                0x00000100 /* set if VM shared between processes */
#define CLONE_FS                0x00000200 /* set if fs info shared between processes */
#define CLONE_FILES             0x00000400 /* set if open files shared between processes */
#define CLONE_SIGHAND           0x00000800 /* set if signal handlers and blocked signals shared */
#define CLONE_PIDFD             0x00001000 /* set if a pidfd should be placed in parent */
#define CLONE_PTRACE            0x00002000 /* set if we want to let tracing continue on the child too */
#define CLONE_VFORK             0x00004000 /* set if the parent wants the child to wake it up on mm_release */
#define CLONE_PARENT            0x00008000 /* set if we want to have the same parent as the cloner */
#define CLONE_THREAD            0x00010000 /* Same thread group? */
#define CLONE_NEWNS             0x00020000 /* New mount namespace group */
#define CLONE_SYSVSEM           0x00040000 /* share system V SEM_UNDO semantics */
#define CLONE_SETTLS            0x00080000 /* create a new TLS for the child */
#define CLONE_PARENT_SETTID     0x00100000 /* set the TID in the parent */
#define CLONE_CHILD_CLEARTID    0x00200000 /* clear the TID in the child */
#define CLONE_DETACHED          0x00400000 /* Unused, ignored */
#define CLONE_UNTRACED          0x00800000 /* set if the tracing process can't force CLONE_PTRACE on this clone */
#define CLONE_CHILD_SETTID      0x01000000 /* set the TID in the child */
#define CLONE_NEWCGROUP         0x02000000 /* New cgroup namespace */
#define CLONE_NEWUTS            0x04000000 /* New utsname namespace */
#define CLONE_NEWIPC            0x08000000 /* New ipc namespace */
#define CLONE_NEWUSER           0x10000000 /* New user namespace */
#define CLONE_NEWPID            0x20000000 /* New pid namespace */
#define CLONE_NEWNET            0x40000000 /* New network namespace */
#define CLONE_IO                0x80000000 /* Clone io context */


#define STACKSIZE (4096 * 4096)

struct user_desc {
	unsigned int  entry_number;
	unsigned int  base_addr;
	unsigned int  limit;
	unsigned int  seg_32bit:1;
	unsigned int  contents:2;
	unsigned int  read_exec_only:1;
	unsigned int  limit_in_pages:1;
	unsigned int  seg_not_present:1;
	unsigned int  useable:1;
#ifdef __x86_64__
	unsigned int  lm:1;
#endif
};

extern "C" {
    int __clone(void*(*fn)(void*),void*stack,int flags,void*arg,...);
}

sys::Thread::Thread(void *function(void*),void *args){
    _Stack = (char*)syscall6(__NR_mmap,0,STACKSIZE, PROT_READ | PROT_WRITE,
                                    MAP_PRIVATE| MAP_ANONYMOUS | MAP_GROWSDOWN, -1, 0);
    
    if ((unsigned long)_Stack == -1){
        SystemException excep;
        throw excep[SystemException::Error] << "Can't allocate Memory for the thread!";
    }
    
    const unsigned long clone_flags = CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND
                                        | CLONE_THREAD | CLONE_SYSVSEM | CLONE_SETTLS
                                        | CLONE_PARENT_SETTID | CLONE_CHILD_CLEARTID | CLONE_DETACHED;
    
    int tls=0;
                                            
    int ret = __clone(function,_Stack+STACKSIZE,clone_flags,args,&_ParentPid,&tls,&_ThreadPid);
    
    if(ret < 0){
        SystemException excep;
        throw excep[SystemException::Error] << "Can't start thread clone failed!";
    }

    
}

sys::Thread::~Thread(){
}

sys::Thread * sys::Thread::nextThread(){
    return _nextThread;
}


void sys::Thread::join(){
    while(_ParentPid!=_ThreadPid){
         Sleep(1);
    };    
}

bool sys::Thread::joinable(){
    if(_ParentPid!=_ThreadPid)
        return false;
    return true;
}

sys::ThreadPool::ThreadPool(){
  _firstThread=nullptr;
  _lastThread=_firstThread;
}

sys::ThreadPool::~ThreadPool(){
    delete _firstThread;
}

void sys::ThreadPool::addjob(sys::Thread *addthread){
    if(!_firstThread){
        _firstThread=addthread;
        _lastThread=_firstThread;
    }else{
        _lastThread->_nextThread=addthread;
        _lastThread=_lastThread->_nextThread;
    }
}

void sys::ThreadPool::deljob(sys::Thread* delthread){
    sys::Thread *prevth=nullptr;
    for(sys::Thread *curth=_firstThread; curth; curth=curth->nextThread()){
        if(curth==delthread){
            if(prevth)
                prevth->_nextThread=curth->nextThread();
            else
                _firstThread=curth->nextThread();
            curth->_nextThread=nullptr;
            delete curth;
        }
        prevth=curth;
    }
}

void sys::ThreadPool::join(){
    for(;;){
        for(sys::Thread *curth=_firstThread; curth; curth=curth->nextThread()){
            if(curth->joinable()){
                deljob(curth);
            }
        }
        if(!_firstThread)
            return;
        Sleep(1);
    }
}

extern "C" {
   int pthread_attr_init(pthread_attr_t *){
        return 0; 
   };
   
   int pthread_attr_destroy(pthread_attr_t *){
        return 0;
   }
   
   int pthread_join(pthread_t thread, void **){
       try{
            ((sys::Thread*)thread)->join();
            return 0;
       }catch(...){
           return -1;
       }
   }
   
   int pthread_create(pthread_t* __restrict__ thread,
                          const pthread_attr_t* __restrict__ attr,
                          void *(*start_routine)(void *),
                          void* __restrict__ arg){
       try{
           *thread = new sys::Thread(start_routine,arg);
           return 0;
       }catch(...){
            return -1;
       }
   }
};
