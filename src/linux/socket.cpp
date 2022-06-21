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

#include <string.h>

#include "sysbits.h"
#include "syscall.h"

#include "systempp/sysutils.h"
#include "systempp/sysexception.h"
#include "systempp/syssocket.h"


#define F_GETFL 3
#define F_SETFL 4

#define SO_REUSEADDR 2
#define SOCK_STREAM  1

#define SOL_SOCKET 1

#define AF_UNIX 1
#define AF_INET 2

#define O_BLOCK     00000000
#define O_NONBLOCK  00004000

typedef unsigned long int in_addr_t;

typedef unsigned short sa_family_t;
typedef unsigned short in_port_t;
typedef unsigned short u_int16m_t; 
typedef unsigned int u_int32m_t;

/* Address to accept any incoming messages.  */
#define INADDR_ANY  ((in_addr_t) 0x00000000)
/* Address to send to all hosts.  */
#define INADDR_BROADCAST ((in_addr_t) 0xffffffff)
/* Address indicating an error return.  */
#define INADDR_NONE ((in_addr_t) 0xffffffff)

#define htons(n) (((((unsigned short)(n) & 0xFF)) << 8) | (((unsigned short)(n) & 0xFF00) >> 8))

#define MAXSERVS 255
#define MAXADDRS 255

struct service {
    int socktype;
    int proto;
    int port;
} ports[MAXSERVS];

struct address {
    int family;
    int num;
}addrs[MAXADDRS];
    

struct sockaddr{
    sa_family_t   sa_family;
    char          sa_data[];
};

struct sockaddr_un {
    sa_family_t sun_family;
    char        sun_path[108];
};

struct in_addr {
    unsigned long int s_addr;  // load with inet_aton()
};

struct in6_addr {
    unsigned char   s6_addr[16];
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

struct sockaddr_in {
    short            sin_family;   // e.g. AF_INET
    unsigned short   sin_port;     // e.g. htons(3490)
    struct in_addr   sin_addr;     // see struct in_addr, below
    char             sin_zero[8];  // zero this if you want to
};

struct sockaddr_in6 {
    unsigned char           sin6_len;      /* length of this structure */
    unsigned char           sin6_family;   /* AF_INET6                 */
    u_int16m_t       sin6_port;     /* Transport layer port #   */
    u_int32m_t       sin6_flowinfo; /* IPv6 flow information    */
    struct in6_addr  sin6_addr;     /* IPv6 address             */
};

struct aibuf {
    struct addrinfo ai;
    union sa {
        struct sockaddr_in sin;
        struct sockaddr_in6 sin6;
    } sa;
    volatile int lock[1];
    short slot, ref;
};

sys::ClientSocket::ClientSocket(){
    _Socket=-1;
    _SocketPtr = new struct sockaddr;
    _SocketPtrSize=sizeof(sockaddr);
    zero(_SocketPtr,_SocketPtrSize);
}

sys::ClientSocket::~ClientSocket(){
    syscall1(__NR_close, _Socket);
    delete (struct sockaddr*)_SocketPtr;
}


void sys::ClientSocket::setnonblocking(){
    int sockopts=(int)syscall3(__NR_fcntl, _Socket, F_GETFL, 0);
    if((int)syscall3(__NR_fcntl, _Socket, F_SETFL,sockopts | O_NONBLOCK)<0){
        SystemException exception;
        exception[SystemException::Error] << "Could not set ClientSocket nonblocking!";
        throw exception; 
    }
}

int sys::ClientSocket::getSocket(){
    return _Socket;
}


sys::ServerSocket::ServerSocket(const char* uxsocket,int maxconnections,
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
    _UxPath=uxsocket;
    scopy(uxsocket,uxsocket+getlen(uxsocket),usock.sun_path);
    
    if ((_Socket=(int)syscall3(__NR_socket,AF_UNIX,sockettype, 0)) < 0){
        exception[SystemException::Critical] << "Can't create Socket UnixSocket";
        throw exception;
    }
    
    syscall5(__NR_setsockopt,_Socket,SOL_SOCKET,sockopts,(long)&optval, sizeof(optval));
    
    if (syscall3(__NR_bind,_Socket,(long)&usock, sizeof(struct sockaddr_un)) < 0){
        exception[SystemException::Error] << "Can't bind Server UnixSocket";
        throw exception;
    }
}

sys::ServerSocket::ServerSocket(const char* addr, int port,int maxconnections,
                                        int sockettype,int sockopts){
    SystemException exception;
    _Maxconnections=maxconnections;
    if(sockopts == -1)
        sockopts=SO_REUSEADDR;
    
    _Socket = (int)syscall3(__NR_socket,AF_INET,SOCK_STREAM,0);
    
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    
    
    int optval = 1;
    syscall5(__NR_setsockopt,_Socket, SOL_SOCKET, sockopts,(unsigned long) &optval,sizeof(optval));   
    
    syscall3(__NR_bind,_Socket,(unsigned long)((struct sockaddr_in *)&address),sizeof(address));
    
    if(_Socket <0){
        exception[SystemException::Critical] << "Could not bind serversocket";
        throw exception;
    }
}
                                        
sys::ServerSocket::~ServerSocket(){
    syscall1(__NR_close, _Socket);
    if(_UxPath.size()>0){
        syscall1(__NR_unlink,(unsigned long)_UxPath.c_str());
    }
}

void sys::ServerSocket::setnonblocking(){
    int sockopts=(int)syscall3(__NR_fcntl, _Socket, F_GETFL, 0);
    if((int)syscall3(__NR_fcntl, _Socket, F_SETFL,sockopts | O_NONBLOCK)<0){
        SystemException exception;
        exception[SystemException::Error] << "Could not set ServerSocket nonblocking!";
        throw exception; 
    }
}

void sys::ServerSocket::listenSocket(){
    SystemException httpexception;
    if(syscall2(__NR_listen,_Socket,_Maxconnections) < 0){
        httpexception[SystemException::Critical] << "Can't listen Server Socket";
        throw httpexception;
    }
}

int sys::ServerSocket::getSocket(){
    return _Socket;
}

int sys::ServerSocket::getMaxconnections(){
    return _Maxconnections;
}

int sys::ServerSocket::acceptEvent(ClientSocket *clientsocket){
    SystemException exception;
    int socket = syscall3(__NR_accept,_Socket,(unsigned long)&clientsocket->_SocketPtr,
                          (unsigned long)&clientsocket->_SocketPtrSize);
    if(socket<0){
        exception[SystemException::Error] << "Can't accept on Socket";
        throw exception;
    }
    clientsocket->_Socket=socket;
    return socket;
}

int sys::ServerSocket::sendData(ClientSocket* socket, void* data, unsigned long size){
    return sendData(socket,data,size,0);
}

int sys::ServerSocket::sendData(ClientSocket* socket, void* data, unsigned long size,int flags){
    SystemException exception;
    int rval=syscall6(__NR_sendto,socket->_Socket,
                        (unsigned long)data,
                        size,
                        flags,
                        (unsigned long)socket->_SocketPtr,
                        (unsigned long)socket->_SocketPtrSize
                     );
    if(rval<0){
        exception[SystemException::Error] << "Socket senddata failed on Socket: " << socket->_Socket;
        throw exception;
    }
    return rval;
}


int sys::ServerSocket::recvData(ClientSocket* socket, void* data, unsigned long size){
    return recvData(socket,data,size,0);
}

int sys::ServerSocket::recvData(ClientSocket* socket, void* data, unsigned long size,int flags){
    SystemException exception;
    int recvsize=syscall6(__NR_recvfrom,socket->_Socket,
                            (unsigned long)data,
                            size,
                            flags,
                            (unsigned long)socket->_SocketPtr,
                            (unsigned long)socket->_SocketPtrSize
                         );
    if(recvsize<0){
        exception[SystemException::Error] << "Socket recvdata failed on Socket: " << socket->_Socket;
        throw exception;
    }
    return recvsize;
}
