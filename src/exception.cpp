/*******************************************************************************
 * Copyright (c) 2021, Jan Koester jan.koester@gmx.net
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

#include <cstddef>

#include "systempp/sysutils.h"
#include "systempp/sysexception.h"

sys::SystemException::Message::Message(){
    _Buffer=nullptr;
    _BufferSize=0;
    _nextMessage=nullptr;
}

sys::SystemException::Message::~Message(){
    delete[] _Buffer;
    delete   _nextMessage;
}

sys::SystemException::SystemException(){
    curCType=Note;
    _firstMessage=nullptr;
    _lastMessage=nullptr;
    _printBuffer=nullptr;
};

sys::SystemException::SystemException(const SystemException &exp){
    curCType=exp.curCType;
    _firstMessage=nullptr;
    _lastMessage=nullptr;
    _printBuffer=nullptr;
    for(Message *curmsg=exp._firstMessage; curmsg; curmsg=curmsg->_nextMessage){
        *this << curmsg->_Buffer;
    }
}

sys::SystemException::~SystemException(){
    delete[] _printBuffer;
    delete _firstMessage;
}

int sys::SystemException::getErrorType(){
    return curCType; 
}

const char* sys::SystemException::what(){
    std::size_t bufsize=0,written=0;
    for(Message *curmsg=_firstMessage; curmsg; curmsg=curmsg->_nextMessage){
        bufsize+=curmsg->_BufferSize;
    }
    delete[] _printBuffer;
    _printBuffer = new char[bufsize+1];
    for(Message *curmsg=_firstMessage; curmsg; curmsg=curmsg->_nextMessage){
        scopy(curmsg->_Buffer,curmsg->_Buffer+curmsg->_BufferSize,_printBuffer+written);
        written+=curmsg->_BufferSize;
    }
    _printBuffer[bufsize]='\0';
    
    return _printBuffer;
}

const sys::SystemException & sys::SystemException::Exception() throw(){
    return *this;
}


sys::SystemException& sys::SystemException::asign(const char *src){
    if(!src)
        return *this;
    if(!_firstMessage){
        _firstMessage=new Message();
        _lastMessage=_firstMessage;
    }else{
        _lastMessage->_nextMessage=new Message();
        _lastMessage=_lastMessage->_nextMessage;
    }
    _lastMessage->_CType=curCType;
    _lastMessage->_BufferSize=getlen(src);
    _lastMessage->_Buffer=new char[_lastMessage->_BufferSize+1];
    scopy(src,src+getlen(src)+1,_lastMessage->_Buffer);
    return *this;   
}

sys::SystemException& sys::SystemException::operator[](int errtype){
    curCType=errtype;
    return *this;
}

sys::SystemException& sys::SystemException::operator<<(const char *src){
    return asign(src);   
};

sys::SystemException& sys::SystemException::operator<<(int src){
    char *buf=new char[255];
    itoa(src,buf);
    asign(buf);
    delete[] buf;
    return *this;
}
