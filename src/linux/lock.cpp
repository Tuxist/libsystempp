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

#include <stdint.h>
#include <atomic>

#include <linux/futex.h>
#include <linux/mman.h>

#include "include/sysexception.h"
#include "include/syslock.h"

#include "sysbits.h"
#include "syscall.h"

class libsystempp::LockData {
private:
    std::atomic<uint32_t> addr;
    std::atomic<uint32_t> futex;
    friend class Lock;
};

libsystempp::Lock::Lock(){
    _LockData= new LockData;
    _LockData->addr.store(syscall6(__NR_mmap,0,sizeof(_LockData->addr) * 2, PROT_READ | PROT_WRITE,
                       MAP_ANONYMOUS | MAP_SHARED, -1, 0));
    
    if(_LockData->addr.load()==-1){
        SystemException excep;
        throw excep[SystemException::Error] << "Can't allocate Memory for the lock"; 
    }
    
    _LockData->futex.store(_LockData->addr.load());
}

libsystempp::Lock::~Lock(){
    delete _LockData;
}

void libsystempp::Lock::lock(){
    long res;
    while(1){
        if (_LockData->futex.exchange(0)){
            break; 
        }
        res=syscall6(__NR_futex,(unsigned long)_LockData->futex.load(), FUTEX_WAIT, 0,0,0,0);
        
        if(res==-1){
            SystemException excep;
            throw excep[SystemException::Error] << "Futex syscall for the lock failed!"; 
        }  
    }
}

bool libsystempp::Lock::trylock(){
    if (_LockData->futex.exchange(0)){
        return false; 
    }
    long res=syscall6(__NR_futex,(unsigned long)_LockData->futex.load(), FUTEX_WAIT, 0,0,0,0);
    
    if(res==-1){
        SystemException excep;
        throw excep[SystemException::Error] << "Futex syscall for the trylock failed!"; 
    }
    
    return true;
}

void libsystempp::Lock::unlock(){
    long res=0;
    if (_LockData->futex.exchange(1)){
        res=syscall6(__NR_futex,(unsigned long)_LockData->futex.load(), FUTEX_WAKE, 0,0,0,0);
    }
    if(res==-1){
        SystemException excep;
        throw excep[SystemException::Error] << "Futex syscall for the unlock failed!"; 
    }
}
