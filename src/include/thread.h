/*******************************************************************************
Copyright (c) 2020, Jan Koester jan.koester@gmx.net
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

#include <cstddef>

#ifndef THREAD_H
#define THREAD_H

namespace libsystempp {
    class MutexData;
    
    class Mutex {
    public:
        Mutex();
        ~Mutex();
        void Lock();
        void tryLock();
        void unLock();
        bool isLocked();
    private:
        MutexData *_MutexData;
    };

    class ThreadAttribute {
    public:
//         enum Policy {
//             SCHED_FIFO=0, 
//             SCHED_RR=1, 
//             SCHED_OTHER=2
//         };
        int    getPolicy();
        void   setPolicy(int policy);
        int    getScope();
        void   setScope(int scope);
        void  *getStackAddr();
        void   setStackAddr(void *stackaddr);
        size_t getStackSize();
        void   setStackSize(size_t stacksize);
        void   setGuardSize(size_t guardsize);
        size_t getGuardSize();
    private:
        ThreadAttribute();
        ~ThreadAttribute();
        int    _Policy;
        int    _Scope;
        void  *_StackAddr;
        size_t _StackSize;
        size_t _GuardSize;
        friend class Scheduler;
    };
    
    class Thread {
    public:
        Thread();
        virtual ~Thread();
        void                  Detach();
        int                   DetachState();
        void                  Join(void *rval=NULL);
        bool                  JoinAble();
        ThreadAttribute      *getinheritsched();
        void                  setinheritsched(ThreadAttribute *attr);
        void                  Create(void *function(void*), void *arguments);
    private:
        ThreadAttribute      *_ThreadAttribute;
        long                  _Thread;
        Thread               *_nextThread;
    };
    
    class ThreadPool {
    public:
        ThreadPool(size_t size);
        ~ThreadPool();
        void               getThread();
    private:
        void              _resize(size_t size);
        Thread          **_ThreadPool;
        size_t            _ThreadPoolSize;
        Mutex            *_Mutex;
        friend class Thread;
    };
};

#endif
