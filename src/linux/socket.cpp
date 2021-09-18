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

#include "include/utils.h"
#include "include/exception.h"
#include "include/socket.h"
#include "include/file.h"

#include "linux/include/socket.h"

#include "syscall.h"

#define O_NONBLOCK  00004000

libsystempp::ClientSocket::ClientSocket(){
  _Socket = new FileDescriptor();
}

libsystempp::ClientSocket::~ClientSocket(){
    try{
        _Socket->close();
    }catch(...){};
}


void libsystempp::ClientSocket::setnonblocking(){
    _Socket->setFcntl(_Socket->getFcntl() | O_NONBLOCK);
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
    
    int s = getaddrinfo(addr, port_buffer, &sockaddr, &result);
    if (s != 0) {
        exception[SystemException::Critical] << "getaddrinfo failed " << gai_strerror(s);
        throw exception;
    }
    
    /* getaddrinfo() returns a list of address structures.
     *  Try each address until we successfully bind(2).
     *  If socket(2) (or bind(2)) fails, we (close the socket
     *  and) try the next address. */
    
    for (rp = result; rp != nullptr; rp = rp->ai_next) {
        _Socket = (int)syscall3(__NR_socket,rp->ai_family, rp->ai_socktype,rp->ai_protocol);
        if (_Socket == -1)
            continue;
        
        int optval = 1;
        syscall5(__NR_setsockopt,_Socket, SOL_SOCKET, sockopts, &optval, sizeof(optval));
       
        if (bind(_Socket, rp->ai_addr, rp->ai_addrlen) == 0)
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
// 
// libsystempp::ServerSocket::~ServerSocket(){
// }
// 
// void libsystempp::ServerSocket::setnonblocking(){
//     fcntl(Socket, F_SETFL, fcntl(Socket, F_GETFL, 0) | O_NONBLOCK);
// }
// 
// void libsystempp::ServerSocket::listenSocket(){
//     SystemException httpexception;
//     if(listen(Socket, _Maxconnections) < 0){
//         httpexception[SystemException::Critical] << "Can't listen Server Socket"<< errno;
//         throw httpexception;
//     }
// }
// 
// int libsystempp::ServerSocket::getSocket(){
//     return Socket;
// }
// 
// int libsystempp::ServerSocket::getMaxconnections(){
//     return _Maxconnections;
// }
// 
// int libsystempp::ServerSocket::acceptEvent(ClientSocket *clientsocket){
//     SystemException exception;
//     clientsocket->_ClientAddrLen=sizeof(clientsocket);
//     int socket = socketcall_cp(accept,Socket,clientsocket->_ClientAddr, &clientsocket->_ClientAddrLen);
//     if(socket<0){
// #ifdef __GLIBCXX__
//         char errbuf[255];
//         exception[SystemException::Error] << "Can't accept on  Socket" << strerror_r(errno, errbuf, 255);
// #else
//         char errbuf[255];
//         strerror_r(errno, errbuf, 255);
//         exception[SystemException::Error] << "Can't accept on  Socket" << errbuf;
// #endif
//     }
//     clientsocket->Socket=socket;
//     return socket;
// }
// 
// 
// 
// int libsystempp::ServerSocket::sendData(ClientSocket* socket, void* data, size_t size){
//     return sendData(socket,data,size,0);
// }
// 
// int libsystempp::ServerSocket::sendData(ClientSocket* socket, void* data, size_t size,int flags){
//     SystemException exception;
//     int rval=0;
//     rval=write(socket->Socket,data,size,flags);
//     if(rval<0){
//         char errbuf[255];
// #ifdef __GLIBCXX__
//         if(errno==EAGAIN)
//             exception[SystemException::Warning] << "Socket sendata:" << strerror_r(errno,errbuf,255);   
//         else
//             exception[SystemException::Error] << "Socket sendata:" << strerror_r(errno,errbuf,255);
// #else
//         strerror_r(errno,errbuf,255);
//         if(errno == EAGAIN)
//             exception[SystemException::Warning] << "Socket sendata:" << errbuf;
//         else 
//             exception[SystemException::Error] << "Socket sendata:" << errbuf;
// #endif
//         throw exception;
//     }
//     return rval;
// }
// 
// int libsystempp::ServerSocket::recvData(ClientSocket* socket, void* data, size_t size){
//     return recvData(socket,data,size,0);
// }
// 
// int libsystempp::ServerSocket::recvData(ClientSocket* socket, void* data, size_t size,int flags){
//     SystemException exception;
//     int recvsize=read(socket->Socket,data, size,flags);
//     if(recvsize<0){
//         char errbuf[255];
// #ifdef __GLIBCXX__ 
//         if(errno==EAGAIN)
//             exception[SystemException::Warning] << "Socket recvdata "  <<  socket->Socket << ": "<< strerror_r(errno,errbuf,255);
//         else
//             exception[SystemException::Error] << "Socket recvdata " << socket->Socket << ": " << strerror_r(errno,errbuf,255);
// #else
//         strerror_r(errno,errbuf,255);
//         if(errno == EAGAIN)
//             exception[SystemException::Warning] << "Socket recvdata:" << socket->Socket << ": " << errbuf;
//         else
//             exception[SystemException::Critical] << "Socket recvdata:" << socket->Socket << ":" <<  errbuf;
// #endif
//         throw exception;
//     }
//     return recvsize;
// }
