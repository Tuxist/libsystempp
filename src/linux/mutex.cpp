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

#include "systempp/sysexception.h"

#include "sysbits.h"
#include "syscall.h"
#include "pthread.h"

#define FUTEX_WAIT  0
#define FUTEX_WAKE  1

namespace sys {
    class mutex{
    public:
        mutex() noexcept{
            _LockData= new std::atomic<uint32_t>;   
            _LockData->store(1);
        }
        
        ~mutex(){
            delete _LockData;
        }
        
        void lock(){
            long res;
            while(1){
                uint32_t ldat = 1;
                if (std::atomic_compare_exchange_strong(_LockData,&ldat,0))
                    break;
                
                res=syscall6(__NR_futex,(unsigned long)_LockData, FUTEX_WAIT, 0,0,0,0);
                
                if(res<0){
                    SystemException excep;
                    throw excep[SystemException::Error] << "Futex syscall for the lock failed!"; 
                }
            }
        }
        
        bool trylock(){
            uint32_t ldat = 1;
            
            if (std::atomic_compare_exchange_strong(_LockData,&ldat,0))
                return true;
            
            return false;
        }
        
        void unlock(){
            uint32_t ldat=0;
            if (std::atomic_compare_exchange_strong(_LockData,&ldat,1)){
                long res=syscall6(__NR_futex,(unsigned long)_LockData, FUTEX_WAKE, 1,0,0,0);
                
                if(res<0){
                    SystemException excep;
                    throw excep[SystemException::Error] << "Futex syscall for the unlock failed!"; 
                }
            }
        }
private:
        std::atomic<uint32_t> *_LockData;
    };
};
