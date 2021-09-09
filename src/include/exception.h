/*******************************************************************************
 * Copyright (c) 2014, Jan Koester jan.koester@gmx.net
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

#include "config.h"

#ifndef EXCEPTION_H
#define EXCEPTION_H

#ifdef DEBUG

#endif

namespace libsystempp {
    
    class SystemException {
    public:
        
        SystemException();
        SystemException(const SystemException &exp);
        virtual ~SystemException();
        
        int getErrorType();
        
        const char* what();
        
        const SystemException& Exception() throw();
        
        enum Type {Note,Warning,Error,Critical};
        
        SystemException& asign(const char *src);
        SystemException& operator[](int errtype);
        SystemException& operator<<(const char *src);
        SystemException& operator<<(int src);
    private:
        struct Message {
            Message();
            ~Message();
            char    *_Buffer;
            int      _BufferSize;        
            int      _CType;
            Message *_nextMessage;
        };
        
        Message *_firstMessage;
        Message *_lastMessage;
        
        char    *_printBuffer;
        char    *_errnobuf;
        int      _curCType;
    };
}
#endif

