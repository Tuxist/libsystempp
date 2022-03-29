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
#include <mutex>
#include <thread>
#include <unistd.h>

#include <systempp/syscall.h>
#include <systempp/sysbits.h>
#include <systempp/sysexception.h>
#include <systempp/syseventapi.h>
#include <systempp/sysutils.h>

#include <config.h>

#define READEVENT 0
#define SENDEVENT 1

#define EPOLLIN 0x001
#define EPOLLOUT 0x004

#define EPOLL_CTL_ADD 1 
#define EPOLL_CTL_DEL 2
#define EPOLL_CTL_MOD 3 

struct epoll_event {
    uint32_t events; /* epoll events (bit mask) */
    uint64_t data; /* User data */
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
                usleep(1000);
            }
        };

    private:
        std::vector<std::thread*> _Threads;
    };
};

const char* sys::ConnectionData::getData(){
  return _Data;
}

size_t sys::ConnectionData::getDataSize(){
  return _DataSize;
}

sys::ConnectionData *sys::ConnectionData::nextConnectionData(){
  return _nextConnectionData;
}

sys::ConnectionData::ConnectionData(const char*data,size_t datasize)  {
    SystemException excep;
    _Data = new char[datasize];
    sys::scopy(data,data+datasize,_Data);
    _DataSize=datasize;
    _nextConnectionData=nullptr;
}

sys::ConnectionData::~ConnectionData() {
    delete[] _Data;
    delete _nextConnectionData;
}

sys::ClientSocket *sys::Connection::getClientSocket(){
  return _ClientSocket;
}

/** \brief a method to add Data to Sendqueue
  * \param data an const char* to add to sendqueue
  * \param datasize an size_t to set datasize
  * \return the last ConnectionData Block from Sendqueue
  * 
  * This method does unbelievably useful things.  
  * And returns exceptionally the new connection data block.
  * Use it everyday with good health.
  */

sys::ConnectionData *sys::Connection::addSendQueue(const char*data,size_t datasize){
    SystemException excep;
    size_t written=0;
    if(datasize==0)
        return nullptr;
    for(size_t cursize=datasize; cursize>0; cursize=datasize-written){
        if(cursize>Blocksize){
            cursize=Blocksize;
        }
        if(!_SendDataFirst){
            _SendDataFirst= new ConnectionData(data+written,cursize);
            _SendDataLast=_SendDataFirst;
        }else{
            _SendDataLast->_nextConnectionData=new ConnectionData(data+written,cursize);
            _SendDataLast=_SendDataLast->_nextConnectionData;
        }
        written+=cursize;
    }
#ifdef DEBUG
    std::cout << "Written:" << written << " Datasize: " << datasize  << std::endl;
#endif
    if(datasize!=written)
        throw excep[SystemException::Critical] << "something goes wrong in addsendque !";
    _SendDataSize+=datasize;
    _EventApi->sendReady(this,true);
    return _SendDataLast;
}

void sys::Connection::cleanSendData(){
   delete _SendDataFirst;
   _SendDataFirst=nullptr;
   _SendDataLast=nullptr;
   _SendDataSize=0;
}

sys::ConnectionData *sys::Connection::resizeSendQueue(size_t size){
    try{
        return _resizeQueue(&_SendDataFirst,&_SendDataLast,&_SendDataSize,size);
    }catch(SystemException &e){
        throw e; 
    }
}

sys::ConnectionData* sys::Connection::getSendData(){
  return _SendDataFirst;
}

size_t sys::Connection::getSendSize(){
  return _SendDataSize;
}

sys::ConnectionData *sys::Connection::addRecvQueue(const char *data,size_t datasize){
    if(datasize<=0){
        SystemException exception;
        exception[SystemException::Error] << "addRecvQueue wrong datasize";
        throw exception;
    }
    if(!_ReadDataFirst){
        _ReadDataFirst= new ConnectionData(data,datasize);
        _ReadDataLast=_ReadDataFirst;
    }else{
        _ReadDataLast->_nextConnectionData=new ConnectionData(data,datasize);
        _ReadDataLast=_ReadDataLast->_nextConnectionData;
    }
    _ReadDataSize+=datasize;
    return _ReadDataLast;
}

void sys::Connection::cleanRecvData(){
   delete _ReadDataFirst;
  _ReadDataFirst=nullptr;
  _ReadDataLast=nullptr;
  _ReadDataSize=0;
}


sys::ConnectionData *sys::Connection::resizeRecvQueue(size_t size){
    try{
        return _resizeQueue(&_ReadDataFirst,&_ReadDataLast,&_ReadDataSize,size);
    }catch(SystemException &e){
        throw e; 
    }
}

sys::ConnectionData *sys::Connection::getRecvData(){
  return _ReadDataFirst;
}

size_t sys::Connection::getRecvSize(){
  return _ReadDataSize;
}

sys::ConnectionData *sys::Connection::_resizeQueue(ConnectionData** firstdata, ConnectionData** lastdata,
                                                               size_t *qsize, size_t size){
    SystemException exception;
    if(!*firstdata || size > *qsize){
        exception[SystemException::Error] << "_resizeQueue wrong datasize or ConnectionData";
        throw exception;
    }
    #ifdef DEBUG
    size_t delsize=0,presize=*qsize;
    #endif
    (*qsize)-=size;
HAVEDATA:
    if((*firstdata)->getDataSize() <=size){
        #ifdef DEBUG
        delsize+=(*firstdata)->getDataSize();;
        #endif
        size-=(*firstdata)->getDataSize();
        ConnectionData *newdat=(*firstdata)->_nextConnectionData;
        (*firstdata)->_nextConnectionData=nullptr;
        if(*firstdata==*lastdata)
            (*lastdata)=nullptr; 
        delete *firstdata;
        *firstdata=newdat;
        if(*firstdata)
            goto HAVEDATA;
    }
    if(size>0){
        #ifdef DEBUG
        delsize+=size;
        #endif
        for(size_t i=0; i<((*firstdata)->getDataSize()-size); ++i){
            (*firstdata)->_Data[i]=(*firstdata)->_Data[size+i];
        }
        (*firstdata)->_DataSize-=size;
        *firstdata=(*firstdata);
    }
    #ifdef DEBUG
    std::cout << " delsize: "    << delsize
                                 << " Calculated Blocksize: " << (presize-delsize) 
                                 << std::endl;
    if((presize-delsize)!=*qsize)
        throw exception[SystemException::Critical] << "_resizeQueue: Calculated wrong size";
    #endif
    
    return *firstdata;
}
                                                               
int sys::Connection::copyValue(ConnectionData* startblock, int startpos, 
                          ConnectionData* endblock, int endpos, char** buffer){
  size_t copysize=0,copypos=0;
  for(ConnectionData *curdat=startblock; curdat; curdat=curdat->nextConnectionData()){
    if(curdat==endblock){
      copysize+=endpos;
      break;
    }
    copysize+=curdat->getDataSize();
  }
  copysize-=startpos;
  char *buf;
  buf = new char[(copysize+1)]; //one more for termination
  for(ConnectionData *curdat=startblock; curdat; curdat=curdat->nextConnectionData()){
    if(curdat==startblock && curdat==endblock){
      sys::scopy(curdat->_Data+startpos,curdat->_Data+(endpos-startpos),buf+copypos);
    }else if(curdat==startblock){
      sys::scopy(curdat->_Data+startpos,curdat->_Data+(curdat->getDataSize()-startpos),buf+copypos);
      copypos+=curdat->getDataSize()-startpos;
    }else if(curdat==endblock){
      sys::scopy(curdat->_Data,curdat->_Data+endpos,buf+copypos);
      copypos+=endpos;
    }else{
      sys::scopy(curdat->_Data,curdat->_Data+curdat->getDataSize(),buf+copypos);
      copypos+=curdat->getDataSize();
    }
    if(curdat==endblock)
      break;
  }
  buf[copysize]='\0';
  *buffer=buf;
  return copysize; //not include termination
}

int sys::Connection::searchValue(ConnectionData* startblock, ConnectionData** findblock, 
                                       const char* keyword){
    return searchValue(startblock, findblock, keyword,sys::getlen(keyword));
}
                                       
int sys::Connection::searchValue(ConnectionData* startblock, ConnectionData** findblock, 
                                       const char* keyword,size_t keylen){
    size_t fpos=0,fcurpos=0;
    for(ConnectionData *curdat=startblock; curdat; curdat=curdat->nextConnectionData()){
        for(size_t pos=0; pos<curdat->getDataSize(); ++pos){
            if(keyword[fcurpos]==curdat->_Data[pos]){
                if(fcurpos==0){
                    fpos=pos;
                    *findblock=curdat;
                }
                fcurpos++;
            }else{
                fcurpos=0;
                fpos=0;
                *findblock=nullptr;
            }
            if(fcurpos==keylen)
                return fpos;
        }
    }
    return -1;
}

sys::Connection::Connection(sys::ServerSocket *servsock,EventApi *event){
    Blocksize=16384;
    _ClientSocket=new sys::ClientSocket();
    _ServerSocket = servsock;
    _ReadDataFirst=nullptr;
    _ReadDataLast=nullptr;
    _ReadDataSize=0;
    _SendDataFirst=nullptr;
    _SendDataLast=nullptr;
    _SendDataSize=0;
    _EventApi=event;
}

sys::Connection::~Connection(){
    delete _ClientSocket;
    delete _ReadDataFirst;
    delete _SendDataFirst;
}

namespace sys {    
    class EPOLL : public EventApi{
    public:
        EPOLL(sys::ServerSocket* serversocket){
            _ServerSocket=serversocket;
            _waitFD=0;
            _currFD=0;
        };
        
        ~EPOLL();
        
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
                    _Events[i].data = 0;
        };
        
        sys::Connection *waitEventHandler(){
            std::lock_guard<std::mutex> lock(_ELock);
            
            ++_currFD;
            
            if(_waitFD<_currFD){
                _waitFD = syscall4(__NR_epoll_wait,_epollFD,
                                   (unsigned long)_Events,_ServerSocket->getMaxconnections(), -1);
                if(_waitFD<0) {
                    SystemException exception;
                    exception[SystemException::Critical] << "initEventHandler: epoll wait failure";
                    throw exception;
                }        
                _currFD = 0;
            }
            return (Connection*)_Events[_currFD].data;
        };

        void ConnectEventHandler(sys::Connection **curcon){
            SystemException exception;
            Connection *curct=new Connection(_ServerSocket,this);
            try {
                /*will create warning debug mode that normally because the check already connection
                 * with this socket if getconnection throw they will be create a new one
                 */
                _ServerSocket->acceptEvent(curct->getClientSocket());
                curct->getClientSocket()->setnonblocking();
                struct epoll_event setevent{0};
                setevent.events = EPOLLIN;
                setevent.data =(uint64_t) curct;
                if (syscall4(__NR_epoll_ctl,_epollFD,EPOLL_CTL_ADD,
                    curct->getClientSocket()->getSocket(),(unsigned long) &setevent) < 0) {
                    exception[SystemException::Error] << "ConnectEventHandler: can't add socket to epoll";
                throw exception;
                    }
                    ConnectEvent(curct);
            }catch (sys::SystemException& e) {
                delete curct;
                exception[SystemException::Error] << e.what();
                throw exception;
            }
            curcon=&curct;
        };
        
        
        int StatusEventHandler(Connection *curcon){
            if(!curcon)
                return EventHandlerStatus::EVCON;
            if(curcon->getSendSize()>0)
                return EventHandlerStatus::EVOUT;
            return EventHandlerStatus::EVIN;
        };
        
        void ReadEventHandler(Connection *curcon){
            SystemException except;
            char *buf = new char[curcon->Blocksize];
            
            if(!curcon){
                except[SystemException::Error] << "ReadEventHandler: no valid data !";
                throw except;
            }
            try{
                int rcvsize=_ServerSocket->recvData(curcon->getClientSocket(),&buf,curcon->Blocksize);
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
            std::lock_guard<std::mutex> lock(_ELock);
            try {
                
                if(!curcon){
                    except[SystemException::Error] << "CloseEvent empty DataPtr!";
                    throw except;
                }
                
                int ect=syscall4(__NR_epoll_ctl,_epollFD,EPOLL_CTL_DEL,curcon->getClientSocket()->getSocket(),0);
                
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
            setevent.data =(uint64_t) curcon;
            if (syscall4(__NR_epoll_ctl,_epollFD,EPOLL_CTL_MOD, 
                curcon->getClientSocket()->getSocket(), (unsigned long)&setevent) < 0) {
                except[SystemException::Error] << "_setEpollEvents: can change socket!";
                throw except;
            }   
        };
        
        int                            _epollFD;
        int                            _waitFD;
        int                            _currFD;
        struct epoll_event            *_Events;
        sys::ServerSocket             *_ServerSocket;
        std::mutex                     _ELock;
    };
};
