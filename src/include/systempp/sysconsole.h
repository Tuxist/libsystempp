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

#include <systempp/sysarray.h>
#include <systempp/sysfile.h>

#pragma once

namespace sys {
    static file STDIN=sys::file(0);
    static file STDOUT=sys::file(1);
    static file STDERR=sys::file(2);

    static const char *endl="\n";
    
    class _consolein {
    public:
        _consolein();
        _consolein(file &fd);
        ~_consolein();
        
        _consolein &operator>>(int &in);
        _consolein &operator>>(unsigned int &in);
        _consolein &operator>>(unsigned long &in);
        _consolein &operator>>(char &in);
        _consolein &operator>>(sys::array<char> &in);
        _consolein &operator>>(_consolein &console);
    private:
        file _FD;
    } static cin;
    
    class _consoleout {
    public:
        _consoleout();
        _consoleout(file &fd);
        ~_consoleout();
        
        _consoleout &operator<<(const char *out);
        _consoleout &operator<<(int out);
        _consoleout &operator<<(unsigned int out);
        _consoleout &operator<<(unsigned long out);
        _consoleout &operator<<(char out);
        _consoleout &operator<<(sys::array<char> out);
        _consoleout &operator<<(_consoleout &console);
    private:
        file _FD;
    } static cout, cerr;
    
};
