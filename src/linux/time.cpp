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

#include "systempp/sysutils.h"
#include "systempp/sysexception.h"
#include "systempp/systime.h"

#include "syscall.h"
#include "sysbits.h"

#include "time.h"

#define SECONDS_PER_DAY       86400ul /*  24* 60 * 60 */
#define DAYS_PER_YEAR         365ul /* no leap year*/
#define DAYS_PER_LEAP_YEAR    1461ul /*   4*365 +   1 */
#define DAYS_IN_100_YEARS     36524ul /* 100*365 +  25 - 1 */
#define DAYS_IN_400_YEARS     146097ul /* 400*365 + 100 - 4 + 1 */
#define DAYS_SINCE_1970_01_01 719468ul  

namespace sys {
    struct timezone {
        int  tz_minuteswest;/* minutes west of Greenwich */
        int  tz_dsttime;/* type of dst correction */
    };

    class TimezoneTemplate {
    private:
        TimezoneTemplate(const char *name,int minwest,int dstime){
            int nlen=sys::getlen(name);
            _Name=new char[nlen+1];
            sys::scopy(name,name+nlen,_Name);
            _Name[nlen]='\0';
            
            _Timezone.tz_minuteswest=minwest;
            _Timezone.tz_dsttime=dstime;
            
        };
        char    *_Name;
        timezone _Timezone;
        friend class Timezone;
    };
};

const void *sys::Timezone::Timezones[]= {
     new sys::TimezoneTemplate("Europe/Berlin",60,120),
     nullptr
};

sys::Timezone::Timezone(const char* name){
    _CTimezone=nullptr;
    int i=0;
    TimezoneTemplate *tz=((TimezoneTemplate*)Timezone::Timezones[i]);
    while(tz){
        if(ncompare(tz->_Name,getlen(tz->_Name),name,getlen(name))==0){
            _CTimezone=&tz->_Timezone;
            return;
        }
        ++i;
        tz=((TimezoneTemplate*)Timezone::Timezones[i]);
    };
}

sys::Time::Time(sys::Timezone* timezone){
    _Zone=timezone;
    _NSec=0;
    _Sec=0;
    _Min=0;
    _Hour=0;
    _Day=0;
    _Year=0;
}

void sys::Time::setNanoSeconds(unsigned int nanosec){
    sys::SystemException excep;
    if(nanosec>999999999){
        excep[sys::SystemException::Error] << "Nano Secondes can't greater than a value of 999999999";
        throw excep;
    } 
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

void sys::Time::setDay(unsigned int day){
    sys::SystemException excep;
    if(day>31){
        excep[sys::SystemException::Error] << "Day can't greater than a value of 31";
        throw excep;
    }
    _Day=day;
}

unsigned int sys::Time::getDay(){
    return _Day;
}

unsigned int sys::Time::getMounth(){
    return _Mounth;
};

void sys::Time::setYear(int year){
    _Year=year;
};

int sys::Time::getYear(){
    return _Year;
};

void sys::Time::_fromUnixtime(void* ptimespec){
    unsigned long long uxtime = ((struct timespec*)ptimespec)->tv_sec;
    unsigned long int dayn = DAYS_SINCE_1970_01_01 + uxtime/SECONDS_PER_DAY;
    unsigned long int sec_since_night = uxtime%SECONDS_PER_DAY;
    unsigned long int temp;

    temp = 4 * (dayn + DAYS_IN_100_YEARS + 1) / DAYS_IN_400_YEARS - 1;
    _Year = 100 * temp;
    dayn -= DAYS_IN_100_YEARS * temp + temp / 4;

    temp = 4 * (dayn + DAYS_PER_YEAR + 1) / DAYS_PER_LEAP_YEAR - 1;
    _Year += temp;
    dayn -= DAYS_PER_YEAR * temp + temp / 4;

    _Mounth = (5 * dayn + 2) / 153;
    _Day = dayn - (_Mounth * 153 + 2) / 5 + 1;

    _Mounth += 3;
    if (_Mounth > 12){
        _Mounth -= 12;
        ++_Year;
    }

    _Hour = sec_since_night / 3600;
    _Min = sec_since_night % 3600 / 60;
    _Sec = sec_since_night % 60;
    _NSec = ((struct timespec*)ptimespec)->tv_nsec;
}

void sys::Time::_toUnixtime(void **ptimespec){
   struct timespec *ts=new struct timespec;
   const short start_days[12] =
    {0,31,59,90,120,151,181,212,243,273,304,334};

  int leapyears = ((_Year-1)-1968)/4
                  - ((_Year-1)-1900)/100
                  + ((_Year-1)-1600)/400;

  long long since1970 = (_Year-1970)*365 + leapyears
                           + start_days[_Mounth-1] + _Day-1;

  if ( (_Mounth>2) && (_Year%4==0 && (_Year%100!=0 || _Year%400==0)) )
    since1970 += 1;

  ts->tv_sec=_Sec + 60 * ( _Min + 60 * (_Hour + 24*since1970) );
  ts->tv_nsec=_NSec;
  *ptimespec=ts;
}

void sys::Time::compare(Time comptime,Time &result){
    struct timespec ts,cts,rts;
    _toUnixtime((void**)&ts);
    comptime._toUnixtime((void**)&cts);
    
    rts.tv_nsec=ts.tv_nsec-cts.tv_nsec;
    rts.tv_sec=ts.tv_sec-cts.tv_sec;
    
    result._fromUnixtime((void*)&rts);
};

void sys::Time::getHWTime(){
    struct timespec ts;
    syscall2(__NR_clock_gettime,0,(long)&ts);
    if(_Zone)
        ts.tv_sec+= (((struct timezone*)_Zone->_CTimezone)->tz_dsttime * 60);
    _fromUnixtime(&ts);
}

void sys::Time::setHWTime(){
    struct timespec *ts;
    _toUnixtime((void**)&ts);
     if(_Zone)
        ts->tv_sec-= (((struct timezone*)_Zone->_CTimezone)->tz_dsttime * 60);   
    syscall2(__NR_clock_settime,0,(long)(long)&ts);
    delete ts;
}
