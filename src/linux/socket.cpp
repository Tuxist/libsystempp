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

#include <linux/fcntl.h>
#include <linux/net.h>

#include "include/utils.h"
#include "include/exception.h"
#include "include/socket.h"

#include "include/linux_socket.h"

#include "syscall.h"

#define O_NONBLOCK  00004000

typedef unsigned short sa_family_t;

struct sockaddr{
    sa_family_t   sa_family;
    char          sa_data[];
};

struct sockaddr_un {
    sa_family_t sun_family;
    char        sun_path[108];
};

struct addrinfo {
    int               ai_flags;     /* AI_PASSIVE, AI_CANONNAME, AI_NUMERICHOST */
    int               ai_family;    /* PF_xxx */
    int               ai_socktype;  /* SOCK_xxx */
    int               ai_protocol;  /* 0 or IPPROTO_xxx for IPv4 and IPv6 */
    unsigned long     ai_addrlen;   /* length of ai_addr */
    char             *ai_canonname; /* canonical name for nodename */
    struct sockaddr  *ai_addr;      /* binary address */
    struct addrinfo  *ai_next;      /* next structure in linked list */
};

libsystempp::ClientSocket::ClientSocket(){
}

libsystempp::ClientSocket::~ClientSocket(){
    syscall1(__NR_close, _Socket);
}


void libsystempp::ClientSocket::setnonblocking(){
    int sockopts=(int)syscall3(__NR_fcntl, _Socket, F_GETFL, 0);
    if((int)syscall3(__NR_fcntl, _Socket, F_SETFL,sockopts | O_NONBLOCK)==-1){
        SystemException exception;
        exception[SystemException::Error] << "Could not set ClientSocket nonblocking!";
        throw exception; 
    }
}

libsystempp::ServerSocket::ServerSocket(const char* uxsocket,int maxconnections,
                                        int sockettype,int sockopts){
    SystemException exception;
    int optval = 1;
    if(sockopts == -1)
        sockopts=SO_REUSEADDR;
    _Maxconnections=maxconnections;
    struct sockaddr_un usock{0};
    usock.sun_family = AF_UNIX;

    if(!uxsocket){
        exception[SystemException::Critical] << "Can't copy Server UnixSocket";
        throw exception;
    }
    
    scopy(uxsocket,uxsocket+getlen(uxsocket),usock.sun_path);
    
    if ((_Socket=(int)syscall3(__NR_socket,AF_UNIX,sockettype, 0)) < 0){
        exception[SystemException::Critical] << "Can't create Socket UnixSocket";
        throw exception;
    }
    
    syscall5(__NR_setsockopt,_Socket,SOL_SOCKET,sockopts,(long)&optval, sizeof(optval));
    
    if (syscall3(__NR_bind,_Socket,(long)&usock, sizeof(struct sockaddr_un)) < 0){
        char errbuf[255];
        exception[SystemException::Error] << "Can't bind Server UnixSocket";
        throw exception;
    }
}

libsystempp::ServerSocket::ServerSocket(const char* addr, int port,int maxconnections,
                                        int sockettype,int sockopts){
    SystemException exception;
    _Maxconnections=maxconnections;
    if(sockopts == -1)
        sockopts=SO_REUSEADDR;
    
    char port_buffer[6];
    itoa(port,port_buffer);
    struct addrinfo *result, *rp,sockaddr;
    zero(&sockaddr,sizeof(struct addrinfo));
    sockaddr.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    sockaddr.ai_socktype = SOCK_STREAM; /* Datagram socket */
    sockaddr.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
    sockaddr.ai_protocol = 0;          /* Any protocol */
    sockaddr.ai_canonname = nullptr;
    sockaddr.ai_addr = nullptr;
    sockaddr.ai_next = nullptr;
    
//     int s = getaddrinfo(addr, port_buffer, &sockaddr, &result);
//     if (s != 0) {
//         exception[SystemException::Critical] << "getaddrinfo failed ";
//         throw exception;
//     }
       
    for (rp = result; rp != nullptr; rp = rp->ai_next) {
        _Socket = (int)syscall3(__NR_socket,rp->ai_family, rp->ai_socktype,rp->ai_protocol);
        if (_Socket == -1)
            continue;
        
        int optval = 1;
        syscall5(__NR_setsockopt,_Socket, SOL_SOCKET, sockopts,(unsigned long) &optval,sizeof(optval));
       
        if (syscall3(__NR_bind,_Socket, (unsigned long)rp->ai_addr, rp->ai_addrlen) == 0)
            break;                  /* Success */
            
        syscall1(__NR_close,_Socket);
    }
    
    if (rp == nullptr) {               /* No address succeeded */
        exception[SystemException::Critical] << "Could not bind serversocket";
        throw exception;
    }
    
    zero(&result,sizeof(result));
    
    if(_Socket <0){               /* No address succeeded */
        exception[SystemException::Critical] << "Could not bind serversocket";
        throw exception;
    }
        
}

libsystempp::ServerSocket::~ServerSocket(){
    syscall1(__NR_close, _Socket);
}

void libsystempp::ServerSocket::setnonblocking(){
    int sockopts=(int)syscall3(__NR_fcntl, _Socket, F_GETFL, 0);
    if((int)syscall3(__NR_fcntl, _Socket, F_SETFL,sockopts | O_NONBLOCK)==-1){
        SystemException exception;
        exception[SystemException::Error] << "Could not set ServerSocket nonblocking!";
        throw exception; 
    }
}

void libsystempp::ServerSocket::listenSocket(){
    SystemException httpexception;
    if(syscall2(SYS_LISTEN,_Socket,_Maxconnections) < 0){
        httpexception[SystemException::Critical] << "Can't listen Server Socket";
        throw httpexception;
    }
}

int libsystempp::ServerSocket::getSocket(){
    return _Socket;
}

int libsystempp::ServerSocket::getMaxconnections(){
    return _Maxconnections;
}

int libsystempp::ServerSocket::acceptEvent(ClientSocket *clientsocket){
    SystemException exception;
    clientsocket->_SocketPtrSize=sizeof(sockaddr);
    clientsocket->_SocketPtr = new sockaddr();
    int socket = syscall3(SYS_ACCEPT,_Socket,(unsigned long)clientsocket->_SocketPtr,
                          (unsigned long)&clientsocket->_SocketPtrSize);
    if(socket<0){
        char errbuf[255];
        exception[SystemException::Error] << "Can't accept on  Socket";
        throw exception;
    }
    clientsocket->_Socket=socket;
    return socket;
}

int libsystempp::ServerSocket::sendData(ClientSocket* socket, void* data, unsigned long size){
    return sendData(socket,data,size,0);
}

int libsystempp::ServerSocket::sendData(ClientSocket* socket, void* data, unsigned long size,int flags){
    SystemException exception;
    int rval=0;
    rval=syscall6(SYS_SENDTO,socket->_Socket,(unsigned long)data,size,flags,
                  (unsigned long)socket->_SocketPtr,socket->_SocketPtrSize);
    if(rval<0){
        exception[SystemException::Error] << "Socket senddata failed on Socket: " << socket->_Socket;
        throw exception;
    }
    return rval;
}


int libsystempp::ServerSocket::recvData(ClientSocket* socket, void* data, unsigned long size){
    return recvData(socket,data,size,0);
}

int libsystempp::ServerSocket::recvData(ClientSocket* socket, void* data, unsigned long size,int flags){
    SystemException exception;
    int recvsize=syscall6(SYS_RECVFROM,socket->_Socket,(unsigned long)data,size,flags,
                  (unsigned long)socket->_SocketPtr,socket->_SocketPtrSize);
    if(recvsize<0){
        exception[SystemException::Error] << "Socket recvdata failed on Socket: " << socket->_Socket;
        throw exception;
    }
    return recvsize;
}
