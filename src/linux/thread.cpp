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

extern "C" {
    #include "wrapper.h"
}

#include <algorithm>

#include "../include/thread.h"
#include "../include/sysinfo.h"

extern "C" {
    __attribute__((weak)) void *__dso_handle;
}

class libsystempp::MutexData {
private:
    wrapper_mutex *_Mutex;
    friend class Mutex;
};

libsystempp::Mutex::Mutex(){
    _MutexData=new MutexData;
    wrapper_mutex_init(&_MutexData->_Mutex);
}

libsystempp::Mutex::~Mutex(){
    wrapper_mutex_destroy(&_MutexData->_Mutex);
    delete _MutexData;
}

void libsystempp::Mutex::Lock(){
    wrapper_mutex_lock(&_MutexData->_Mutex);
}

void libsystempp::Mutex::tryLock(){
    if(wrapper_mutex_trylock(&_MutexData->_Mutex)!=0)
        throw "Can't Lock Mutex";
}

void libsystempp::Mutex::unLock(){
    wrapper_mutex_unlock(&_MutexData->_Mutex);
}

bool libsystempp::Mutex::isLocked(){
  if(wrapper_mutex_is_locked(&_MutexData->_Mutex)==1)
      return true;
  return false;
}

int libsystempp::ThreadAttribute::getPolicy(){
    return _Policy;
}
void libsystempp::ThreadAttribute::setPolicy(int policy){
    _Policy=policy;
}

int libsystempp::ThreadAttribute::getScope(){
    return _Scope;
}

void libsystempp::ThreadAttribute::setScope(int scope){
    _Scope=scope;
}

void  *libsystempp::ThreadAttribute::getStackAddr(){
    return _StackAddr;
}

void libsystempp::ThreadAttribute::setStackAddr(void *stackaddr){
    _StackAddr=stackaddr;
}

size_t libsystempp::ThreadAttribute::getStackSize(){
    return _StackSize;
}

void libsystempp::ThreadAttribute::setStackSize(size_t stacksize){
    _StackSize=stacksize;
}

void libsystempp::ThreadAttribute::setGuardSize(size_t guardsize){
    _GuardSize=guardsize;
}

size_t libsystempp::ThreadAttribute::getGuardSize(){
    return _GuardSize;
}

libsystempp::Thread::Thread(void *method,void *args){
   if(wrapper_clone2(method,this,sizeof(this),
       wrapper_clone_get_clone_thread_flag(),args)<0)
       throw "Clone Failed";
}

libsystempp::Thread::Thread(void *args){
    Thread((void*)&libsystempp::Thread::Run,args);
}

libsystempp::Thread::~Thread(){
}

bool libsystempp::Thread::JoinAble(){
    return true;
}

void *libsystempp::Thread::Run(void *args){
    return NULL;
}

libsystempp::ThreadPool::ThreadPool(size_t size){
    _Mutex = new Mutex();
    _ThreadPoolSize = size;
    _ThreadPool = new Thread*[size];
}

libsystempp::ThreadPool::~ThreadPool(){
    delete _Mutex;
    delete[] _ThreadPool;
}

void libsystempp::ThreadPool::addThread(Thread *thread){
    _Mutex->Lock();
    _Mutex->unLock();
}
