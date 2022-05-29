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

#include "systempp/syscall.h"
#include "systempp/systhread.h"
#include "systempp/syssleep.h"
#include "systempp/sysexception.h"

#include <linux/mman.h>
#include <linux/sched.h>
#include <linux/time.h>


#define STACKSIZE (4096 * 1024)

extern "C" {
    int __clone(void*(*fn)(void*),void*stack,int flags,void*arg,...);
}


sys::Thread::Thread(void *function(void*),void *args){
    _nextThread = nullptr;
    
    _Stack = (char*)syscall6(__NR_mmap,0,STACKSIZE, PROT_READ | PROT_WRITE,
                                    MAP_PRIVATE| MAP_ANONYMOUS | MAP_GROWSDOWN, -1, 0);
    
    if ((unsigned long)_Stack == -1){
        SystemException excep;
        throw excep[SystemException::Error] << "Can't allocate Memory for the thread!";
    }
    
    const unsigned long clone_flags = CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND
                                        | CLONE_THREAD | CLONE_SYSVSEM | CLONE_SETTLS
                                        | CLONE_PARENT_SETTID | CLONE_CHILD_CLEARTID | CLONE_DETACHED;
    
    _ParentPid = __clone(function,_Stack+STACKSIZE,clone_flags,args,&_ThreadPid,this,&_ThreadPid);
    
    if(_ParentPid < 0){
        SystemException excep;
        throw excep[SystemException::Error] << "Can't start thread clone failed!";
    }
    
}

sys::Thread::~Thread(){
    delete _nextThread;
}

sys::Thread * sys::Thread::nextThread(){
    return _nextThread;
}


void sys::Thread::join(){
    while(_ParentPid==_ThreadPid){
         Sleep(1);
    };    
}

bool sys::Thread::joinable(){
    if(_ParentPid==_ThreadPid)
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
