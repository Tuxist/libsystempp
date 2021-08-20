#include "pthread.h"
#include "../include/thread.h"

int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr){
    mutex = new libsystempp::Mutex();
}

int pthread_mutex_destroy(pthread_mutex_t *mutex){
    delete (libsystempp::Mutex*) mutex;
}

int pthread_mutex_getprioceiling(const pthread_mutex_t *mutex, int *ceiling){
    
}

int pthread_mutex_lock(pthread_mutex_t *mutex){
    try{
        ((libsystempp::Mutex*)mutex)->Lock();
        return 0;
    }catch(...){
        return -1;
    }
}

int pthread_mutex_setprioceiling(pthread_mutex_t *mutex,int prioceiling, int *old_ceiling){
    
}

int pthread_mutex_trylock(pthread_mutex_t *mutex){
    try{
        ((libsystempp::Mutex*)mutex)->tryLock();
        return 0;
    }catch(...){
        return -1;
    }
}

int pthread_mutex_unlock(pthread_mutex_t *mutex){
    try{
        ((libsystempp::Mutex*)mutex)->unLock();
        return 0;
    }catch(...){
        return -1;
    }    
}

int pthread_mutexattr_init(pthread_mutexattr_t *mutexattr){
    
}

int pthread_mutexattr_destroy(pthread_mutexattr_t *mutexattr){
    
}

int pthread_mutexattr_getprioceiling(const pthread_mutexattr_t *mutexattr,int *ceiling){
    
}

int pthread_mutexattr_setprioceiling(pthread_mutexattr_t *mutexattr, int ceiling){
    
}

int pthread_mutexattr_getprotocol(const pthread_mutexattr_t *mutexattr, int *protocol){
    
}

int pthread_mutexattr_getpshared(const pthread_mutexattr_t *mutexattr, int *shared){
    
}

int pthread_mutexattr_gettype(const pthread_mutexattr_t *mutexattr, int *type){
    
}



int pthread_mutexattr_setprotocol(pthread_mutexattr_t *mutexattr, int protocol){
    
}

int pthread_mutexattr_setpshared(pthread_mutexattr_t *mutexattr, int shared){
    
}

int pthread_mutexattr_settype(pthread_mutexattr_t *mutexattr, int type){
    
}
