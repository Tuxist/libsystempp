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
    static unsigned long int exp=0;
    static unsigned long int des=1;
    long s;
    if(__atomic_compare_exchange(((unsigned long int*)_mutex),&exp,&des,false,
                                __ATOMIC_SEQ_CST,__ATOMIC_SEQ_CST)){
        do {
            s = syscall6(__NR_futex,(unsigned long)_mutex,FUTEX_WAIT,0,0,0,0);
        }while(s==-1);
    }
};


bool sys::mutex::trylock(){
    static unsigned long int exp=0;
    static unsigned long int des=1;
    if(__atomic_compare_exchange(((unsigned long int*)_mutex),&exp,&des,false,
                                __ATOMIC_SEQ_CST,__ATOMIC_SEQ_CST)){
        long s = syscall6(__NR_futex,(unsigned long)_mutex,FUTEX_WAKE,1,0,0,0);
        if(s==-1){
            return false;
        }
    }
    return true;
}

void sys::mutex::unlock(){
    SystemException excep;
    static unsigned long int exp=1;
    static unsigned long int des=0;
    long s = syscall6(__NR_futex,(unsigned long)_mutex,FUTEX_WAKE,1,0,0,0);
    if(__atomic_compare_exchange(((unsigned long int*)_mutex),&exp,&des,false,
                                __ATOMIC_SEQ_CST,__ATOMIC_SEQ_CST)){
        if(s==-1){
            excep[SystemException::Error] << "can't unlock mutex";
            throw excep;
        }
    }
}
