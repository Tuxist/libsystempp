/*******************************************************************************
 * Copyright (c) 2018, Jan Koester jan.koester@gmx.net
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * Redistributions of source code must retain the above copyright
 *	  notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
 *	  notice, this list of conditions and the following disclaimer in the
 *	  documentation and/or other materials provided with the distribution.
 * Neither the name of the <organization> nor the
 *	  names of its contributors may be used to endorse or promote products
 *	  derived from this software without specific prior written permission.
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

#include <sys/sysinfo.h>
#include <sys/mount.h>

#include "sysarray.h"

#define FSINFOMAXLEN 255

#ifndef SYSINFO_H
#define SYSINFO_H

namespace libsystempp {   
    class CpuInfo {
    public:
        CpuInfo();
        ~CpuInfo();
        const char *getVendor();
        int         getCores();
        int         getThreads();
        int         getActualThread();
    private:
        CharArray   _Vendor;
        void       *_cpuinfo;
    };
    
    class SysInfo {
    public:
        SysInfo();
        unsigned int getFreeRam();
        unsigned int getTotalRam();
        unsigned int getBufferRam();
    private:
        #ifdef Windows
        MEMORYSTATUSEX statex;
        #else
        struct sysinfo _Sysinfo;
        #endif
        
    };
};

#endif
