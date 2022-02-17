#include <cstdlib>
#include <cxxabi.h>
#include <typeinfo>
#include <mutex>

static std::mutex __guard_mutex;

extern "C" {
    int __cxa_atexit(void (*f)(void *), void *objptr, void *dso){
        return 0;
    };
    
    int __cxa_finalize(void *f){
        return 0;
    };
    
    void __cxa_call_unexpected(void *u){
        
    };
    
    void *__cxa_begin_catch(void *c) throw(){
        return c;
    };
    
    void __cxa_end_catch(){    
        
    }
    
    void *__cxa_allocate_exception(unsigned long thrown_size) throw(){
        return new char[thrown_size];
    };
    
    void __cxa_throw (void *thrown_exception,std::type_info *tinfo, void (*dest) (void *) ){
        std::exit(1);
    };
    
    void __cxa_free_exception(void *thrown_exception) throw(){
        delete (char*) thrown_exception;
    };

    void __cxa_throw_bad_array_new_length(){
        std::exit(1);
    };
    
    void __cxa_pure_virtual() {
        while(1);
    };
    
};
