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

#include "sysfile.h"

#pragma once

#define SYSOUT 0
#define SYSERR 1
#define SYSIN  2

namespace libsystempp {
    static FileDescriptor STDIN;
    static FileDescriptor STDOUT;
    static FileDescriptor STDERR;
    
    class _Console {
    public:
        _Console();
        _Console(FileDescriptor &fd);
        ~_Console();
        static const char *endl;
        _Console &operator<<(const char *out);
        _Console &operator<<(int out);
        _Console &operator<<(unsigned int out);
        _Console &operator<<(unsigned long out);
        _Console &operator<<(char out);
        _Console &operator<<(_Console &console);
        _Console &operator>>(char **in);
        _Console &operator>>(CharArray &in);
    private:
        FileDescriptor _FD;
    }static CONOUT, CONERR, CONNIN;
    
    static _Console Console[3] = {
        CONOUT,
        CONERR,
        CONNIN
    };
    
};
