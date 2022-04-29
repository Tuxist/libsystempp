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

#include "syscall.h"
#include "sysbits.h"

# ifndef SYS_futex
#  define SYS_futex __NR_futex
# endif

#include <iostream>
#include <vector>
#include <mutex>
#include <thread>

#include "systempp/sysexception.h"
#include "systempp/syseventapi.h"
#include "systempp/sysutils.h"
#include "systempp/sysinfo.h"

#include <config.h>

#define READEVENT 0
#define SENDEVENT 1

#define EPOLLIN 0x001
#define EPOLLOUT 0x004

#define EPOLL_CTL_ADD 1 
#define EPOLL_CTL_DEL 2
#define EPOLL_CTL_MOD 3 

#define BLOCKSIZE 16384

typedef union epoll_data {
    void    *ptr;
    int      fd;
    uint32_t u32;
    uint64_t u64;
} epoll_data_t;


struct epoll_event {
    uint32_t         events; /* epoll events (bit mask) */
    epoll_data_t     data; /* User data */
};

namespace sys {
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
               
            }
        };

    private:
        std::vector<std::thread*> _Threads;
    };
};

namespace sys {    
    class EPOLL : public EventApi{
    public:
        EPOLL(sys::ServerSocket* serversocket){
            _ServerSocket=serversocket;
            _WaitPos=0;
        };
        
        virtual ~EPOLL(){
        };
        
        /*basic functions*/
        const char *getEventType(){
            return "EPOLL";
        }
        
        /*event handler function*/
        void initEventHandler(){
            SystemException exception;
            try{
                _ServerSocket->setnonblocking();
                _ServerSocket->listenSocket();
            }catch(SystemException &e){
                throw e;
            }
            
            struct epoll_event setevent= (struct epoll_event) {
                0
            };
            
            _epollFD =syscall1(__NR_epoll_create1,0);
            
            if (_epollFD < 0) {
                exception[SystemException::Critical]<<"initEventHandler:" << "can't create epoll";
                throw exception;
            }
            
            setevent.events = EPOLLIN;
            
            if (syscall4(__NR_epoll_ctl,_epollFD,EPOLL_CTL_ADD,
                _ServerSocket->getSocket(),(unsigned long)&setevent) < 0) {
                    exception[SystemException::Critical] << "initEventHandler: can't create epoll";
                    throw exception;
            }
                
            _Events = new epoll_event[_ServerSocket->getMaxconnections()];
            for(int i=0; i<_ServerSocket->getMaxconnections(); ++i)
                _Events[i].data.ptr = nullptr;
        };
        
        sys::Connection *waitEventHandler(){
            std::lock_guard<std::mutex> lock(_ELock);

            if(_WaitPos<1){
                _WaitPos=syscall4(__NR_epoll_wait,_epollFD,
                                  (unsigned long)_Events,_ServerSocket->getMaxconnections(), -1);      
                
                if(_WaitPos<1) {
                    SystemException exception;
                    exception[SystemException::Error] << "initEventHandler: epoll wait failure";
                    throw exception;
                }
            }

            --_WaitPos;
            
            Connection *rcon=(sys::Connection*)_Events[_WaitPos].data.ptr;
            
            if(!rcon)
                rcon = new Connection(_ServerSocket,this);

            return rcon;
        };

        void ConnectEventHandler(sys::Connection *curcon){
            SystemException exception;
            try {
                if(!curcon){
                    exception[SystemException::Error] << "ConnectEventHandler: no valid data !";
                    throw exception;
                }
                /*will create warning debug mode that normally because the check already connection
                 * with this socket if getconnection throw they will be create a new one
                 */
                _ServerSocket->acceptEvent(curcon->getClientSocket());
                curcon->getClientSocket()->setnonblocking();
                struct epoll_event setevent{0};
                setevent.events = EPOLLIN;
                setevent.data.ptr = curcon;
                if (syscall4(__NR_epoll_ctl,_epollFD,EPOLL_CTL_ADD,
                    curcon->getClientSocket()->getSocket(),(unsigned long)&setevent) < 0) {
                    exception[SystemException::Error] << "ConnectEventHandler: can't add socket to epoll";
                    throw exception;
                }
                ConnectEvent(curcon);
            }catch (sys::SystemException& e) {
                exception[SystemException::Error] << e.what();
                throw exception;
            }
        };
        
        
        int StatusEventHandler(Connection *curcon){
            if(curcon->getClientSocket()->getSocket()==-1)
                return EventHandlerStatus::EVCON;
            if(curcon->getSendSize()>0)
                return EventHandlerStatus::EVOUT;
            return EventHandlerStatus::EVIN;
        };
        
        void ReadEventHandler(Connection *curcon){
            SystemException except;
            char buf[BLOCKSIZE];
            
            if(!curcon){
                except[SystemException::Error] << "ReadEventHandler: no valid data !";
                throw except;
            }
            try{
                int rcvsize=_ServerSocket->recvData(curcon->getClientSocket(),&buf,BLOCKSIZE);
                curcon->addRecvQueue(buf,rcvsize);
            }catch(sys::SystemException &e){
                except[SystemException::Critical] << e.what();
                throw except;
            }
            RequestEvent(curcon);
        };

        void WriteEventHandler(Connection *curcon){
            SystemException exception;
            
            if(!curcon){
                exception[SystemException::Error] << "WriteEventHandler: no valid data !";
                throw exception;
            }
            try{
                ssize_t sended=_ServerSocket->sendData(curcon->getClientSocket(),
                                                       (void*)curcon->getSendData()->getData(),
                                                       curcon->getSendData()->getDataSize());
                curcon->resizeSendQueue(sended);
                if(!curcon->getSendData())
                    _setEpollEvents(curcon,EPOLLIN);
            }catch(sys::SystemException &e){
                exception[SystemException::Critical] << e.what();
                throw exception;
            }
            ResponseEvent(curcon);    
        };
        
        void CloseEventHandler(sys::Connection *curcon){
            SystemException except;
            try {
                
                if(!curcon){
                    except[SystemException::Error] << "CloseEvent empty DataPtr!";
                    throw except;
                }
                
                int ect=syscall4(__NR_epoll_ctl,_epollFD,EPOLL_CTL_DEL,
                                 curcon->getClientSocket()->getSocket(),0);
                
                if(ect<0) {
                    except[SystemException::Error] << "CloseEvent can't delete Connection from epoll";
                    throw except;
                }
                
                DisconnectEvent(curcon);
                
                delete curcon;
                
                except[SystemException::Note] << "CloseEventHandler: Connection shutdown!";
            } catch(SystemException &e) {
                except[SystemException::Warning] << "CloseEventHandler: Can't do Connection shutdown!";
            }
        };
        
        /*HTTP API Events*/
        void RequestEvent(Connection *curcon){
           return; 
        };
        
        void ResponseEvent(Connection *curcon){
           return; 
        };
        
        void ConnectEvent(Connection *curcon){
           return; 
        };
        
        void DisconnectEvent(Connection *curcon){
           return; 
        };
        
        /*Connection Ready to send Data*/
        void sendReady(Connection *curcon,bool ready){
            if(ready){
                _setEpollEvents(curcon,EPOLLIN | EPOLLOUT);
            }else{
                _setEpollEvents(curcon,EPOLLIN);
            }
        };
        
    private:
        void _setEpollEvents(Connection *curcon,int events){
            SystemException except;
            struct epoll_event setevent{ 0 };
            setevent.events = events;
            setevent.data.ptr = curcon;
            if (syscall4(__NR_epoll_ctl,_epollFD,EPOLL_CTL_MOD, 
                curcon->getClientSocket()->getSocket(), (unsigned long)&setevent) < 0) {
                except[SystemException::Error] << "_setEpollEvents: can change socket!";
                throw except;
            }
        };
        
        int                            _WaitPos;
        int                            _epollFD;
        struct epoll_event            *_Events;
        sys::ServerSocket             *_ServerSocket;
        std::mutex                     _ELock;
    };
    
    bool Event::_Run=true;
    bool Event::_Restart=false;

    Event::Event(sys::ServerSocket* serversocket){
        _EAPI = new EPOLL(serversocket);
    }

    Event::~Event(){
    }

    EventApi::~EventApi(){
    }

    void Event::runEventloop(){
        sys::CpuInfo cpuinfo;
        size_t thrs = 1;
        _EAPI->initEventHandler();
MAINWORKERLOOP:
        ThreadPool thpool;
        for (size_t i = 0; i < thrs; i++) {
            try{
                thpool.addjob(WorkerThread, (void*)_EAPI);
            }catch(SystemException &e){
                throw e;
            }
        }
        
        thpool.join();
        
        if(sys::Event::_Restart){
            sys::Event::_Restart=false;
            goto MAINWORKERLOOP;
        }
    }

    void * sys::Event::WorkerThread(void* wrkevent){
        EventApi *eventptr=((EventApi*)wrkevent);
        SystemException excep;
        while (sys::Event::_Run) {
            try {
                Connection *i=eventptr->waitEventHandler();
                try {
                    switch(eventptr->StatusEventHandler(i)) {
                        case EPOLL::EVCON:
                            eventptr->ConnectEventHandler(i);
                            break;
                        case EPOLL::EVIN:
                            eventptr->ReadEventHandler(i);
                            break;
                        case EPOLL::EVOUT:
                            eventptr->WriteEventHandler(i);
                            break;
                        default:
                            excep[SystemException::Error] << "no action try to close";
                            throw excep;
                    }
                } catch(SystemException &e) {
                    eventptr->CloseEventHandler(i);
                    if(e.getErrorType()==SystemException::Critical) {
                        throw e;
                    }
                }
            } catch(SystemException &e) {
                switch(e.getErrorType()) {
                    case SystemException::Critical:
                        std::cerr<< e.what() << std::endl;
                        break;
                }
            }
        }
        return nullptr;
    }
};
