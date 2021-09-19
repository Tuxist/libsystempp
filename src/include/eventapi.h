/*******************************************************************************
Copyright (c) 2014, Jan Koester jan.koester@gmx.net
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

#include "exception.h"
#include "config.h"
#include "os/os.h"
#include "connections.h"

#ifndef EVENTAPI_H
#define EVENTAPI_H

namespace libhttppp {
class EventApi {
    public:
        
        enum EventHandlerStatus{EVNOTREADY=-1,EVIN=0,EVOUT=1,EVUP=2,EVERR=3,EVWAIT=4};
        enum LockState{LOCKED=0,UNLOCKED=1,NOLOCK=2,ERRLOCK=3};
        
        virtual ~EventApi();
        virtual void initEventHandler()=0;
        virtual int    waitEventHandler()=0;
        virtual const char *getEventType()=0;
        virtual void ConnectEventHandler(int des)=0;
        virtual int  StatusEventHandler(int des)=0;
        virtual void ReadEventHandler(int des)=0;
        virtual void WriteEventHandler(int des)=0;
        virtual void CloseEventHandler(int des)=0;
        
        /*Multithreading options*/
        virtual int LockConnection(int des)=0;
        virtual void UnlockConnection(int des)=0;
        
        /*HTTP API Events*/
        virtual void RequestEvent(Connection *curcon)=0;
        virtual void ResponseEvent(Connection *curcon)=0;
        virtual void ConnectEvent(Connection *curcon)=0;
        virtual void DisconnectEvent(Connection *curcon)=0;
        
        /*Connection Ready to send Data 
         *DANGEROUS to burnout your cpu
         *only use this if know what you do!*/
        virtual void sendReady(Connection *curcon,bool ready)=0;
  };
};

#endif
