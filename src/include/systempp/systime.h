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

#pragma once

typedef unsigned long time_t; 

struct timespec {
    time_t tv_sec;
    long tv_nsec;
};

struct timeval {
   time_t      tv_sec;
   long int    tv_usec; 
};

namespace sys {
    class Timezone {
    public:
        Timezone(const char* name=nullptr);
        ~Timezone() noexcept;
        static const Timezone Timezones[];
        /*please only use in systempp plattform depended code*/
        void  *_CTimezone;
    private:
        Timezone(const char* name,int minwest,int dsttime);
    };
    
    class Time {
    public:
        Time(Timezone *timezone=nullptr);
        void setNanoSeconds(unsigned int nanosec);
        unsigned int getNanoSeconds();
              
        void setSeconds(unsigned int sec);
        unsigned int  getSeconds();
        
        void setMinute(unsigned int min);
        unsigned int  getMinute();
        
        void setHour(unsigned int hour);
        unsigned int  getHour();

        void setDay(unsigned int day);
        unsigned int  getDay();
        
        void setMounth(unsigned int mounth);
        unsigned int  getMounth();
        
        void setYear(int year);
        int  getYear();
        
        void getHWTime();
        void setHWTime();

        void compare(Time comptime,Time &result);
        
        /*please use only systempp only plattform depend code*/
        void _toUnixtimeSpec(struct timespec &ptimespec);
        void _fromUnixtimeSpec(struct timespec ptimespec);
        
        void _toUnixtimeVal(struct timeval &ptimeval);
        void _fromUnixtimeVal(struct timeval ptimeval);
    private:     
        unsigned int   _NSec;
        unsigned int   _MSec;
        unsigned int   _Sec;
        unsigned int   _Min;
        unsigned int   _Hour;
        unsigned int   _Day;
        unsigned int   _Mounth;
        int            _Year;
        Timezone      *_Zone;
    };
};
