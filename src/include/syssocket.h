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

#include "sysarray.h"

#pragma once

namespace libsystempp {
    class ClientSocket {
    public:
        ClientSocket();
        ~ClientSocket();
        void         setnonblocking();
        int          getSocket();
    protected:
        int            _Socket;
        void          *_SocketPtr;
        int            _SocketPtrSize;
        friend class ServerSocket;
    };
    
    class ServerSocket {
    public:
        ServerSocket(const char *uxsocket,int maxconnections,
                     int sockettype,int sockopts);
        int           acceptEvent(ClientSocket *clientsocket);
        int           getSocket();
        ServerSocket(const char *addr,int port,int maxconnections,
                     int sockettype,int sockopts);
        ~ServerSocket();
        
        void          setnonblocking();
        int           getMaxconnections();
        void          listenSocket();
        int           sendData(ClientSocket *socket,void *data,unsigned long size);
        int           sendData(ClientSocket *socket,void *data,unsigned long size,int flags);
        int           recvData(ClientSocket *socket,void *data,unsigned long size);
        int           recvData(ClientSocket *socket,void *data,unsigned long size,int flags);
        #ifdef Windows
        /*Needed for iocp in event*/
        ssize_t       recvWSAData(ClientSocket *socket, WSABUF *data, DWORD size, LPDWORD flags,
                                  LPDWORD numberofbytessend, LPWSAOVERLAPPED lpOverlapped,
                                  LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
        ssize_t       sendWSAData(ClientSocket *socket,WSABUF *data, DWORD size,DWORD flags, 
                                  LPDWORD numberofbytessend,LPWSAOVERLAPPED lpOverlapped,
                                  LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
        #endif
        
    private:
        int             _Socket;
        int             _Maxconnections;
        CharArray       _UxPath;
    };
};
