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

#include <systempp/syssocket.h>
#include <systempp/sysexception.h>
#include <systempp/sysconnection.h>

#include "config.h"

#pragma once

namespace sys {   
    class eventapi {
    public:
        
        enum EventHandlerStatus{EVIN=0,EVOUT=1,EVUP=2,EVERR=3,EVWAIT=4,EVCON=5};
        
        virtual ~eventapi();
        virtual void initEventHandler()=0;        
        virtual const char *getEventType()=0;
               
        /*EventHandler*/
        virtual int waitEventHandler(int &pos)=0;
        virtual void ConnectEventHandler(int pos)=0;
        virtual void ReadEventHandler(int pos)=0;
        virtual void WriteEventHandler(int pos)=0;
        virtual void CloseEventHandler(int pos)=0;
        
        /*HTTP API Events*/
        virtual void RequestEvent(con *curcon)=0;
        virtual void ResponseEvent(con *curcon)=0;
        virtual void ConnectEvent(con *curcon)=0;
        virtual void DisconnectEvent(con *curcon)=0;
        
        /*Connection Ready to send Data 
         *DANGEROUS to burnout your cpu
         *only use this if know what you do!*/
        virtual void sendReady(con *curcon,bool ready)=0;
    };

    class event {
    public:
        event(sys::ServerSocket *serversocket);
        void runEventloop();
        static void *WorkerThread(void *wrkevent);
        
        virtual ~event();
        static bool _Run;
        static bool _Restart;
    private:
        eventapi *_EAPI;
    };

};
