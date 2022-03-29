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

#include <vector>
#include <thread>
#include <unistd.h>

#include <systempp/syscall.h>
#include <systempp/sysbits.h>
#include <systempp/sysexception.h>

#include <config.h>

#define READEVENT 0
#define SENDEVENT 1

#include "epoll.h"

namespace libhttppp {
    class ThreadPool {
    public:
        ThreadPool(){
        
        };
        
        ~ThreadPool(){
            for(std::vector<std::thread*>::iterator thit=_Threads.begin(); thit!=_Threads.end(); ++thit){
                (*thit)->join();
            }   
        }
        
        void addjob(void *func(void*),void *args){
            _Threads.push_back((new std::thread(func,args)));
        };
        
        int size(){
            return _Threads.size();
        };
        
        void join(){
            for(;;){
                if(_Threads.empty()){
                    break; 
                }
                for(std::vector<std::thread*>::iterator thit=_Threads.begin(); thit!=_Threads.end(); ++thit){
                    if((*thit)->joinable()){
                        (*thit)->join();
                        _Threads.erase(thit);
                    }
                }
                usleep(1000);
            }
        };

    private:
        std::vector<std::thread*> _Threads;
    };
};


extern "C" {
    struct epoll_event;
};

namespace sys {    
    class EPOLL : public EventApi{
    public:
        EPOLL(sys::ServerSocket* serversocket);
        ~EPOLL();
        
        /*basic functions*/
        const char *getEventType();
        
        /*event handler function*/
        void        initEventHandler();
        Connection *waitEventHandler();
        void        ConnectEventHandler(Connection **curcon);
        int         StatusEventHandler(Connection *curcon);
        void        ReadEventHandler(Connection *curcon);
        void        WriteEventHandler(Connection *curcon);
        void        CloseEventHandler(Connection *curcon);
        
        /*HTTP API Events*/
        void RequestEvent(Connection *curcon);
        void ResponseEvent(Connection *curcon);
        void ConnectEvent(Connection *curcon);
        void DisconnectEvent(Connection *curcon);
        
        /*Connection Ready to send Data*/
        void sendReady(Connection *curcon,bool ready);
        
    private:
        void                           _setEpollEvents(Connection *curcon,int events);
        int                            _epollFD;
        int                            _waitFD;
        int                            _currFD;
        struct epoll_event            *_Events;
        sys::ServerSocket             *_ServerSocket;
        std::mutex                     _ELock;
    };
};

sys::EPOLL::EPOLL(sys::ServerSocket* serversocket) {
    HTTPException httpexception;
    _ServerSocket=serversocket;
    _waitFD=0;
    _currFD=0;
}

sys::EPOLL::~EPOLL(){
}

const char * sys::EPOLL::getEventType(){
    return "EPOLL";
}

void sys::EPOLL::initEventHandler(){
    HTTPException httpexception;
    try{
        _ServerSocket->setnonblocking();
        _ServerSocket->listenSocket();
    }catch(HTTPException &e){
        throw e;
    }
    
    struct epoll_event setevent= (struct epoll_event) {
        0
    };
 
    _epollFD =syscall1(__NR_epoll_create1,0);

    if (_epollFD < 0) {
        httpexception[HTTPException::Critical]<<"initEventHandler:" << "can't create epoll";
        throw httpexception;
    }

    setevent.events = EPOLLIN;

    if (syscall4(__NR_epoll_ctl,_epollFD,EPOLL_CTL_ADD,
        _ServerSocket->getSocket(),(unsigned long)&setevent) < 0) {
        httpexception[HTTPException::Critical] << "initEventHandler: can't create epoll";
        throw httpexception;
    }
    
    _Events = new epoll_event[_ServerSocket->getMaxconnections()];
    for(int i=0; i<_ServerSocket->getMaxconnections(); ++i)
        _Events[i].data = 0;
}

sys::Connection *sys::EPOLL::waitEventHandler(){
    std::lock_guard<std::mutex> lock(_ELock);
    
    ++_currFD;
    
    if(_waitFD<_currFD){
        _waitFD = syscall4(__NR_epoll_wait,_epollFD,
                          (unsigned long)_Events,_ServerSocket->getMaxconnections(), -1);
        if(_waitFD<0) {
            HTTPException httpexception;
            httpexception[HTTPException::Critical] << "initEventHandler: epoll wait failure";
            throw httpexception;
        }        
        _currFD = 0;
    }
    return (Connection*)_Events[_currFD].data;
}

int libhttppp::EPOLL::StatusEventHandler(Connection *curcon){
    if(!curcon)
        return EventHandlerStatus::EVCON;
    if(curcon->getSendSize()>0)
        return EventHandlerStatus::EVOUT;
    return EventHandlerStatus::EVIN;
}

void sys::EPOLL::ConnectEventHandler(Connection **curcon) {
    HTTPException httpexception;
    Connection *curct=new Connection(_ServerSocket,this);
    try {
        /*will create warning debug mode that normally because the check already connection
         * with this socket if getconnection throw they will be create a new one
         */
        _ServerSocket->acceptEvent(curct->getClientSocket());
        curct->getClientSocket()->setnonblocking();
        struct epoll_event setevent{0};
        setevent.events = EPOLLIN;
        setevent.data =(__u64) curct;
        if (syscall4(__NR_epoll_ctl,_epollFD,EPOLL_CTL_ADD,
            curct->getClientSocket()->getSocket(),(unsigned long) &setevent) < 0) {
            httpexception[HTTPException::Error] << "ConnectEventHandler: can't add socket to epoll";
            throw httpexception;
        }
        ConnectEvent(curct);
    }catch (sys::SystemException& e) {
        delete curct;
        httpexception[HTTPException::Error] << e.what();
        throw httpexception;
    }
    curcon=&curct;
}

void sys::EPOLL::ReadEventHandler(Connection *curcon){
    HTTPException httpexception;
    char buf[BLOCKSIZE];
    
    if(!curcon){
        httpexception[HTTPException::Error] << "ReadEventHandler: no valid data !";
        throw httpexception;
    }
    try{
        int rcvsize=_ServerSocket->recvData(curcon->getClientSocket(),&buf,BLOCKSIZE);
        curcon->addRecvQueue(buf,rcvsize);
    }catch(sys::SystemException &e){
        httpexception[HTTPException::Critical] << e.what();
        throw httpexception;
    }
    RequestEvent(curcon);
}

void sys::EPOLL::WriteEventHandler(Connection *curcon){
    HTTPException httpexception;
    
    if(!curcon){
         httpexception[HTTPException::Error] << "WriteEventHandler: no valid data !";
         throw httpexception;
    }
    try{
        ssize_t sended=_ServerSocket->sendData(curcon->getClientSocket(),
                                       (void*)curcon->getSendData()->getData(),
                                       curcon->getSendData()->getDataSize());
        curcon->resizeSendQueue(sended);
        if(!curcon->getSendData())
            _setEpollEvents(curcon,EPOLLIN);
    }catch(sys::SystemException &e){
        httpexception[HTTPException::Critical] << e.what();
        throw httpexception;
    }
    ResponseEvent(curcon);
}

void sys::EPOLL::CloseEventHandler(Connection *curcon){
    HTTPException httpexception;
    std::lock_guard<std::mutex> lock(_ELock);
    try {
        
        if(!curcon){
            httpexception[HTTPException::Error] << "CloseEvent empty DataPtr!";
            throw httpexception;
        }
        
        int ect=syscall4(__NR_epoll_ctl,_epollFD,EPOLL_CTL_DEL,curcon->getClientSocket()->getSocket(),0);
        
        if(ect<0) {
            httpexception[HTTPException::Error] << "CloseEvent can't delete Connection from epoll";
            throw httpexception;
        }
        
        DisconnectEvent(curcon);
        
        delete curcon;

        httpexception[HTTPException::Note] << "CloseEventHandler: Connection shutdown!";
    } catch(HTTPException &e) {
        httpexception[HTTPException::Warning] << "CloseEventHandler: Can't do Connection shutdown!";
    }
}


/*API Events*/

void libhttppp::sys::RequestEvent(Connection *curcon){
    return;
};

void libhttppp::sys::ResponseEvent(Connection *curcon){
    return;
};

void libhttppp::sys::ConnectEvent(Connection *curcon){
    return;
};

void libhttppp::sys::DisconnectEvent(Connection *curcon){
    return;
};

void libhttppp::sys::sendReady(libhttppp::Connection* curcon, bool ready){
    if(ready){
        _setEpollEvents(curcon,EPOLLIN | EPOLLOUT);
    }else{
        _setEpollEvents(curcon,EPOLLIN);
    }
}


void libhttppp::EPOLL::_setEpollEvents(Connection *curcon,int events){
    HTTPException httpexception;
    struct epoll_event setevent{ 0 };
    setevent.events = events;
    setevent.data =(__u64) curcon;
    if (syscall4(__NR_epoll_ctl,_epollFD,EPOLL_CTL_MOD, 
        curcon->getClientSocket()->getSocket(), (unsigned long)&setevent) < 0) {
        httpexception[HTTPException::Error] << "_setEpollEvents: can change socket!";
        throw httpexception;
    }
}
