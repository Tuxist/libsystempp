#include <systempp/sysmutex.h>
#include <systempp/sysexception.h>

#include "sysbits.h"
#include "syscall.h"

#define FUTEX_WAIT    0
#define FUTEX_WAKE    1

#define PROT_READ     0x1
#define PROT_WRITE    0x2

#define MAP_SHARED    0x01
#define MAP_ANONYMOUS 0x20

sys::mutex::mutex(){
    _mutex = (unsigned long int *)syscall6(__NR_mmap,0,sizeof(unsigned long int),
                                                            PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    *((unsigned long int*)_mutex)=1;
}

sys::mutex::~mutex(){
    syscall2(__NR_munmap,(unsigned long)((unsigned long int*)_mutex),sizeof(unsigned long int));
}

void sys::mutex::lock(){
    SystemException excep;
    long s;
    do {
        s = syscall6(__NR_futex,(unsigned long)((unsigned long int*)_mutex),FUTEX_WAIT,0,0,0,0);
    }while(s<0);
};


bool sys::mutex::trylock(){
    long s = syscall6(__NR_futex,(unsigned long)((unsigned long int*)_mutex),FUTEX_WAKE,1,0,0,0);
    if(s==-1){
        return false;
    }
    return true;
}

void sys::mutex::unlock(){
    SystemException excep;
    long s = syscall6(__NR_futex,(unsigned long)((unsigned long int*)_mutex),FUTEX_WAKE,1,0,0,0);
    if(s==-1){
        excep[SystemException::Error] << "can't unlock mutex";
        throw excep;
    }    
}

bool sys::mutex::islocked(){
    return true;
}

