/*******************************************************************************
Copyright (c) 2022, Jan Koester jan.koester@gmx.net
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

#include "systempp/systime.h"
#include "systempp/syssleep.h"
#include "systempp/sysconsole.h"

#define SLEEPTIME 999999999

int main(int argv, char *argc[]){
    sys::Console[SYSOUT] << "Testing Sleep(Time *sleeptime)" << sys::Console[SYSOUT].endl;
    sys::Timezone tz("Europe/Berlin");
    sys::Time startt(&tz),endt(&tz),waitime;
    
    waitime.setNanoSeconds(SLEEPTIME);
   
    startt.getHWTime();
    sys::Sleep spwait(&waitime);
    endt.getHWTime();
    
    sys::Time ctime;
    endt.compare(startt,ctime);
    
    if(ctime.getNanoSeconds()!=SLEEPTIME){
        sys::Console[SYSERR] << "sleep time differs: " 
                  << ctime.getNanoSeconds()-SLEEPTIME
                  << sys::Console[SYSOUT].endl;
    }
    sys::Console[SYSOUT] << "starttime: " << startt.getHour() << ":"
                               << startt.getMinute() << ":" 
                               << startt.getSeconds() << sys::Console[SYSOUT].endl;
    
    sys::Console[SYSOUT] << "endtime: " << endt.getHour() << ":"
                             << endt.getMinute() << ":" 
                             << endt.getSeconds() << sys::Console[SYSOUT].endl;
    
    sys::Console[SYSOUT] << "Testing Sleep(int seconds)" << sys::Console[SYSOUT].endl;

    
    startt.getHWTime();                      
    sys::Sleep(1);
    endt.getHWTime();
    
    sys::Console[SYSOUT] << "time: " << startt.getHour() << ":"
                          << startt.getMinute() << ":" 
                          << startt.getSeconds() << sys::Console[SYSOUT].endl; 

    sys::Console[SYSOUT] << "endtime: " << endt.getHour() << ":"
                             << endt.getMinute() << ":" 
                             << endt.getSeconds() << sys::Console[SYSOUT].endl;
    return 0;
} 
