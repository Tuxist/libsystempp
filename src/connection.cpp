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

const char* sys::con::condata::getData(){
  return _Data;
}

unsigned long sys::con::condata::getDataSize(){
  return _DataSize;
}

sys::con::condata *sys::con::condata::nextcondata(){
  return _nextConnectionData;
}

sys::con::condata::condata(const char*data,unsigned long datasize)  {
    SystemException excep;
    _Data = new char[datasize];
    sys::scopy(data,data+datasize,_Data);
    _DataSize=datasize;
    _nextConnectionData=nullptr;
}

sys::con::condata::~condata() {
    delete[] _Data;
    delete _nextConnectionData;
}

sys::ClientSocket *sys::con::getClientSocket(){
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

sys::con::condata *sys::con::addSendQueue(const char*data,unsigned long datasize){
    if(datasize<=0){
        SystemException exception;
        exception[SystemException::Error] << "addRecvQueue wrong datasize";
        throw exception;
    }
    if(!_SendDataFirst){
        _SendDataFirst= new con::condata(data,datasize);
        _SendDataLast=_SendDataFirst;
    }else{
        _SendDataLast->_nextConnectionData=new con::condata(data,datasize);
        _SendDataLast=_SendDataLast->_nextConnectionData;
    }
    _SendDataSize+=datasize;
    _eventapi->sendReady(this,true);
    return _SendDataLast;
}

void sys::con::cleanSendData(){
   delete _SendDataFirst;
   _SendDataFirst=nullptr;
   _SendDataLast=nullptr;
   _SendDataSize=0;
}

sys::con::condata *sys::con::resizeSendQueue(unsigned long size){
    try{
        return _resizeQueue(&_SendDataFirst,&_SendDataLast,&_SendDataSize,size);
    }catch(SystemException &e){
        throw e; 
    }
}

sys::con::condata* sys::con::getSendData(){
  return _SendDataFirst;
}

unsigned long sys::con::getSendSize(){
  return _SendDataSize;
}

sys::con::condata *sys::con::addRecvQueue(const char *data,unsigned long datasize){
    if(datasize<=0){
        SystemException exception;
        exception[SystemException::Error] << "addRecvQueue wrong datasize";
        throw exception;
    }
    if(!_ReadDataFirst){
        _ReadDataFirst= new con::condata(data,datasize);
        _ReadDataLast=_ReadDataFirst;
    }else{
        _ReadDataLast->_nextConnectionData=new con::condata(data,datasize);
        _ReadDataLast=_ReadDataLast->_nextConnectionData;
    }
    _ReadDataSize+=datasize;
    return _ReadDataLast;
}

void sys::con::cleanRecvData(){
   delete _ReadDataFirst;
  _ReadDataFirst=nullptr;
  _ReadDataLast=nullptr;
  _ReadDataSize=0;
}


sys::con::condata *sys::con::resizeRecvQueue(unsigned long size){
    try{
        return _resizeQueue(&_ReadDataFirst,&_ReadDataLast,&_ReadDataSize,size);
    }catch(SystemException &e){
        throw e; 
    }
}

sys::con::condata *sys::con::getRecvData(){
  return _ReadDataFirst;
}

unsigned long sys::con::getRecvSize(){
  return _ReadDataSize;
}

sys::con::condata *sys::con::_resizeQueue(condata** firstdata, condata** lastdata,
                                                               unsigned long *qsize,unsigned long size){
    SystemException exception;
    if(!*firstdata || size > *qsize){
        exception[SystemException::Error] << "_resizeQueue wrong datasize or ConnectionData";
        throw exception;
    }
    #ifdef DEBUG
    unsigned long delsize=0,presize=*qsize;
    #endif
    (*qsize)-=size;
HAVEDATA:
    if((*firstdata)->getDataSize() <=size){
        #ifdef DEBUG
        delsize+=(*firstdata)->getDataSize();;
        #endif
        size-=(*firstdata)->getDataSize();
        condata *newdat=(*firstdata)->_nextConnectionData;
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
        for(unsigned long i=0; i<((*firstdata)->getDataSize()-size); ++i){
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
                                                               
int sys::con::copyValue(con::condata* startblock, int startpos, 
                          con::condata* endblock, int endpos, char** buffer){
    unsigned long copysize=0,copypos=0;
    for(con::condata *curdat=startblock; curdat; curdat=curdat->nextcondata()){
        if(curdat==endblock){
        copysize+=endpos;
        break;
        }
        copysize+=curdat->getDataSize();
    }
    copysize-=startpos;
    char *buf;
    buf = new char[(copysize+1)]; //one more for termination
    for(con::condata *curdat=startblock; curdat; curdat=curdat->nextcondata()){
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

int sys::con::searchValue(con::condata* startblock, con::condata** findblock, 
                                       const char* keyword){
    return searchValue(startblock, findblock, keyword,sys::getlen(keyword));
}
                                       
int sys::con::searchValue(con::condata* startblock, con::condata** findblock, 
                                       const char* keyword,unsigned long keylen){
   unsigned long fpos=0,fcurpos=0;
    for(con::condata *curdat=startblock; curdat; curdat=curdat->nextcondata()){
        for(unsigned long pos=0; pos<curdat->getDataSize(); ++pos){
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

sys::con::con(sys::ServerSocket *servsock,eventapi *event){
    _ClientSocket=new sys::ClientSocket();
    _ServerSocket = servsock;
    _ReadDataFirst=nullptr;
    _ReadDataLast=nullptr;
    _ReadDataSize=0;
    _SendDataFirst=nullptr;
    _SendDataLast=nullptr;
    _SendDataSize=0;
    _eventapi=event;
}

sys::con::con(){
    _ClientSocket=new sys::ClientSocket();
    _ServerSocket = nullptr;
    _ReadDataFirst=nullptr;
    _ReadDataLast=nullptr;
    _ReadDataSize=0;
    _SendDataFirst=nullptr;
    _SendDataLast=nullptr;
    _SendDataSize=0;
    _eventapi=nullptr;
}

sys::con::~con(){
    delete _ClientSocket;
    delete _ReadDataFirst;
    delete _SendDataFirst;
}

