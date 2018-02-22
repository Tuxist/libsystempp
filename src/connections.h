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
 

#include <config.h>
#include "socket.h"
#include "exception.h"

#ifdef Windows
  #include <windows.h>
  #include <process.h>
#else
  #include "pthread.h"
#endif

#ifndef CONNECTIONS_H
#define CONNECTIONS_H

namespace std {
   class mutex;
};

namespace libhttppp {
  class HTTPException;
  
  class ConnectionData {
  public:
    const char*      getData();
    size_t           getDataSize();
    ConnectionData  *nextConnectionData();    
  private:
    ConnectionData(const char*data,size_t datasize);
    ~ConnectionData();
    char              _Data[BLOCKSIZE];
    size_t            _DataSize;
    ConnectionData   *_nextConnectionData;
    friend class      Connection;
  };

  class Connection {
  public:
    ClientSocket   *getClientSocket();
    Connection     *nextConnection();
    
    /*Cache helper functions*/
    
    int             copyValue(ConnectionData* startblock, int startpos, 
                              ConnectionData* endblock, int endpos, char** buffer);
    int             searchValue(ConnectionData* startblock, ConnectionData** findblock, 
 			        const char* keyword);
    int             searchValue(ConnectionData* startblock, ConnectionData** findblock, 
			        const char* keyword,size_t keylen);
    
    /*Get Data funtions Send Queue*/
    ConnectionData *addSendQueue(const char *data,size_t datasize);
    ConnectionData *resizeSendQueue(size_t size);
    void            cleanSendData();
    ConnectionData *getSendData();
    size_t          getSendSize();
    
    /*Get Data funtions Recv Queue*/
    ConnectionData *addRecvQueue(const char data[BLOCKSIZE],size_t datasize);
    ConnectionData *resizeRecvQueue(size_t size);
    void            cleanRecvData();
    ConnectionData *getRecvData();
    size_t          getRecvSize(); 

    int             pollState;
    
    /*Mutex functions for worker*/
    bool            trylock();
    bool            unlock();
  private:
    ConnectionData *_resizeQueue(ConnectionData **firstdata, ConnectionData **lastdata,
				 size_t *qsize,size_t size);
    ClientSocket   *_ClientSocket;
    Connection     *_nextConnection;
    /*Outgoing Data*/
    ConnectionData *_SendDataFirst;
    ConnectionData *_SendDataLast;
    size_t          _SendDataSize;
    /*Incomming Data*/
    ConnectionData *_ReadDataFirst;
    ConnectionData *_ReadDataLast;
    size_t          _ReadDataSize;
#ifdef Windows
    HANDLE           _CMutex;
#else
    pthread_mutex_t  _CMutex; 
#endif    
    /*Helper functions*/
    Connection();
    ~Connection();
    HTTPException   _httpexception;
    friend class ConnectionPool;
  };
  

  class ConnectionPool {
  public:
    ConnectionPool(ServerSocket *socket);
    ~ConnectionPool();
    Connection *addConnection();
    
    Connection *delConnection(ClientSocket *clientsocket);
#ifndef Windows
    Connection *delConnection(int socket);
#else
    Connection *delConnection(SOCKET socket);
#endif
    Connection *delConnection(Connection *delcon);
    
    Connection *getConnection(ClientSocket *clientsocket);
#ifndef Windows
    Connection *getConnection(int socket);
#else
    Connection *getConnection(SOCKET socket);
#endif
  protected:
    HTTPException _httpexception;
    ServerSocket *_ServerSocket;
    Connection   *_firstConnection;
    Connection   *_lastConnection; 
  };
  
  class ClientConnection {
    
  };
}
#endif
