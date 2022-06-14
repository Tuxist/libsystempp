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

#pragma once

namespace sys {
    class EventApi;
    
    class ConnectionData {
    public:
        const char*      getData();
        unsigned long    getDataSize();
        ConnectionData  *nextConnectionData();
    protected:
        ConnectionData(const char*data, unsigned long datasize);
        ~ConnectionData();
    private:
        char             *_Data;
        unsigned long     _DataSize;
        ConnectionData   *_nextConnectionData;
        friend class      Connection;
    };
    
    class Connection {
    public:
        Connection(ServerSocket *servsock,EventApi *event);
        ~Connection();
        
        /*get client Socket from Connection*/
        ClientSocket   *getClientSocket();
        
        /*Cache helper functions*/
        
        int             copyValue(ConnectionData* startblock, int startpos,
                                  ConnectionData* endblock, int endpos, char** buffer);
        int             searchValue(ConnectionData* startblock, ConnectionData** findblock,
                                    const char* keyword);
        int             searchValue(ConnectionData* startblock, ConnectionData** findblock,
                                    const char* keyword,unsigned long keylen);
        
        ConnectionData *addSendQueue(const char *data,unsigned long datasize);
        ConnectionData *resizeSendQueue(unsigned long size);
        void            cleanSendData();
        ConnectionData *getSendData();
        unsigned long   getSendSize();
        
        /*Get Data funtions Recv Queue*/
        ConnectionData *addRecvQueue(const char *data,unsigned long datasize);
        ConnectionData *resizeRecvQueue(unsigned long size);
        void            cleanRecvData();
        ConnectionData *getRecvData();
        unsigned long   getRecvSize();
    protected:
        Connection();
        /*Incomming Data*/
        unsigned long     _ReadDataSize;
        /*Outgoing Data*/
        unsigned long     _SendDataSize;
    private:
        ConnectionData *_resizeQueue(ConnectionData **firstdata, ConnectionData **lastdata,
                                     unsigned long *qsize,unsigned long size);
        ClientSocket   *_ClientSocket;
        ServerSocket   *_ServerSocket;
        
        /*Incomming Data*/
        ConnectionData *_ReadDataFirst;
        ConnectionData *_ReadDataLast;
        
        /*Outgoing Data*/
        ConnectionData *_SendDataFirst;
        ConnectionData *_SendDataLast;
        EventApi       *_EventApi;

        friend class ConnectionPool;
        friend class Event;
    };
};
