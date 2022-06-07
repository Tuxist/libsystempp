/*******************************************************************************
 * Copyright (c) 2022, Jan Koester jan.koester@gmx.net
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 * Neither the name of the <organization> nor the
 *      names of its contributors may be used to endorse or promote products
 *      derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************/

#include "systempp/sysutils.h"
#include "systempp/syssocket.h"
#include "systempp/sysexception.h"
#include "systempp/sysconnection.h"
#include "systempp/syseventapi.h"

const char* sys::ConnectionData::getData(){
  return _Data;
}

std::size_t sys::ConnectionData::getDataSize(){
  return _DataSize;
}

sys::ConnectionData *sys::ConnectionData::nextConnectionData(){
  return _nextConnectionData;
}

sys::ConnectionData::ConnectionData(const char*data,std::size_t datasize)  {
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

sys::ConnectionData *sys::Connection::addSendQueue(const char*data,std::size_t datasize){
    if(datasize<=0){
        SystemException exception;
        exception[SystemException::Error] << "addRecvQueue wrong datasize";
        throw exception;
    }
    if(!_SendDataFirst){
        _SendDataFirst= new ConnectionData(data,datasize);
        _SendDataLast=_SendDataFirst;
    }else{
        _SendDataLast->_nextConnectionData=new ConnectionData(data,datasize);
        _SendDataLast=_SendDataLast->_nextConnectionData;
    }
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

sys::ConnectionData *sys::Connection::resizeSendQueue(std::size_t size){
    try{
        return _resizeQueue(&_SendDataFirst,&_SendDataLast,&_SendDataSize,size);
    }catch(SystemException &e){
        throw e; 
    }
}

sys::ConnectionData* sys::Connection::getSendData(){
  return _SendDataFirst;
}

std::size_t sys::Connection::getSendSize(){
  return _SendDataSize;
}

sys::ConnectionData *sys::Connection::addRecvQueue(const char *data,std::size_t datasize){
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


sys::ConnectionData *sys::Connection::resizeRecvQueue(std::size_t size){
    try{
        return _resizeQueue(&_ReadDataFirst,&_ReadDataLast,&_ReadDataSize,size);
    }catch(SystemException &e){
        throw e; 
    }
}

sys::ConnectionData *sys::Connection::getRecvData(){
  return _ReadDataFirst;
}

std::size_t sys::Connection::getRecvSize(){
  return _ReadDataSize;
}

sys::ConnectionData *sys::Connection::_resizeQueue(ConnectionData** firstdata, ConnectionData** lastdata,
                                                               std::size_t *qsize, std::size_t size){
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
        for(std::size_t i=0; i<((*firstdata)->getDataSize()-size); ++i){
            (*firstdata)->_Data[i]=(*firstdata)->_Data[size+i];
        }
        (*firstdata)->_DataSize-=size;
        *firstdata=(*firstdata);
    }
    #ifdef DEBUG
    std::cout << " delsize: "    << delsize
                                 << " Calculated Blocksize: " << (presize-delsize) 
                                 << std::endl;
//     if((presize-delsize)!=*qsize)
//         throw exception[SystemException::Critical] << "_resizeQueue: Calculated wrong size";
    #endif
    
    return *firstdata;
}
                                                               
int sys::Connection::copyValue(ConnectionData* startblock, int startpos, 
                          ConnectionData* endblock, int endpos, char** buffer){
    std::size_t copysize=0,copypos=0;
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
                                       const char* keyword,std::size_t keylen){
    std::size_t fpos=0,fcurpos=0;
    for(ConnectionData *curdat=startblock; curdat; curdat=curdat->nextConnectionData()){
        for(std::size_t pos=0; pos<curdat->getDataSize(); ++pos){
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

sys::Connection::Connection(){
    _ClientSocket=new sys::ClientSocket();
    _ServerSocket = nullptr;
    _ReadDataFirst=nullptr;
    _ReadDataLast=nullptr;
    _ReadDataSize=0;
    _SendDataFirst=nullptr;
    _SendDataLast=nullptr;
    _SendDataSize=0;
    _EventApi=nullptr;
}

sys::Connection::~Connection(){
    delete _ClientSocket;
    delete _ReadDataFirst;
    delete _SendDataFirst;
}

