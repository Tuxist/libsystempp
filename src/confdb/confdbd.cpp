/*******************************************************************************
Copyright (c) 2021, Jan Koester jan.koester@gmx.net
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

#include "config.h"

#include "sysconsole.h"
#include "syssocket.h"
#include "bits_socket.h"

#include "confdb_internal.h"
#include "json_min.h"

libsystempp::ConfDbD::ConfDbD(const char *conffile,const char *socket){
    ServerSocket srvsock(socket,1024,SOCK_STREAM,-1);
    srvsock.listenSocket();
    for(;;){
        try{
            ClientSocket sock;
            srvsock.acceptEvent(&sock);
            char buf[255];
            int recvived=0;
            CharArray data;
RECVIVED:
            recvived=srvsock.recvData(&sock,buf,255);
            if(recvived>0){
                data.assign(buf,recvived);
                goto RECVIVED;
            }
            if(data.size()>0){
                JSON json;
                json.parse(&data);
                int send=srvsock.sendData(&sock,(void*)data.c_str(),data.size());
            }
        }catch(...){
            Console[SYSOUT] << "Something goes wrong!" << _Console::endl;
        }
    }
}

int main(int argc,char *argv[]){
    libsystempp::ConfDbD(CONFDBPATH,CONFSOCKET);
}
