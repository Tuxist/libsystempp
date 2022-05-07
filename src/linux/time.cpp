/*******************************************************************************
Copyright (c) 2018, Jan Koester jan.koester@gmx.net
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

#include "systempp/sysexception.h"
#include "systempp/systime.h"

#include "time.h"

namespace sys {
    struct timezone {
        int  tz_minuteswest;/* minutes west of Greenwich */
        int  tz_dsttime;/* type of dst correction */
    };

    class TimezoneTemplate {
    private:
        TimezoneTemplate(){};
        TimezoneTemplate(const char *name,int minwest,int dstime){
            
        };
        char    *_Name;
        timezone _Timezone;
        friend class Timezone;
    };
};

void *sys::Timezone::Timezones= (void**)new sys::TimezoneTemplate*[]
{
     new TimezoneTemplate("Europe/Berlin",120,0),
     nullptr
};

sys::Timezone::Timezone(const char* name){
}

void sys::Time::setNanoSeconds(unsigned int nanosec){
    
};

unsigned int sys::Time::getNanoSeconds(){
    return _NSec;
};

void sys::Time::setSeconds(unsigned int sec){
    sys::SystemException excep;
    if(sec>60){
        excep[sys::SystemException::Error] << "Secondes can't greater than a value of 60";
        throw excep;
    }
    _Sec=sec;    
};

unsigned int sys::Time::getSeconds(){
    return _Sec;
};
        
void sys::Time::setMinute(unsigned int min){
    sys::SystemException excep;
    if(min>60){
        excep[sys::SystemException::Error] << "Minutes can't greater than a value of 60";
        throw excep;
    }
    _Min=min;
};

unsigned int sys::Time::getMinute(){
    return _Min;
};
        
void sys::Time::setHour(unsigned int hour){
    sys::SystemException excep;
    if(hour>24){
        excep[sys::SystemException::Error] << "Hour can't greater than a value of 24";
        throw excep;
    }
    _Hour=hour;
};

unsigned int sys::Time::getHour(){
    return _Hour;
};

void sys::Time::setMounth(unsigned int mounth){
    sys::SystemException excep;
    if(mounth>12){
        excep[sys::SystemException::Error] << "Mounth can't greater than a value of 12";
        throw excep;
    }
    _Mounth=mounth;
};

unsigned int sys::Time::getMounth(){
    return _Mounth;
};

void sys::Time::setYear(int year){
    _Year=year;
};

int sys::Time::getYear(){
    return _Year;
};

void sys::Time::localtime(int Timezone){
    
};

void sys::Time::gmtime(){
    
};

void sys::Time::compare(Time comptime,Time &result){
    
};
