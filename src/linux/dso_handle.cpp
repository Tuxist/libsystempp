#include <unwind.h>
#include <inttypes.h>

#ifdef BUILD_SHARED
void *__dso_handle = &__dso_handle;
#else
void *__dso_handle = (void *)0;
#endif

static int errno;

__attribute__((visibility("hidden"))) int* __errno_location(void) { 
    return &errno; 
}

extern "C" {
    
    _Unwind_Reason_Code __gxx_personality_v0 (
                     int version, _Unwind_Action actions, uint64_t exceptionClass,
                     _Unwind_Exception* unwind_exception, _Unwind_Context* context){
        if (unwind_exception == 0 || context == 0)
            return _URC_FATAL_PHASE1_ERROR;
        
        
//         _URC_NO_REASON;
//   _URC_FOREIGN_EXCEPTION_CAUGHT;
// 
//   _URC_FATAL_PHASE2_ERROR ;
//   _URC_NORMAL_STOP;
// 
//   _URC_END_OF_STACK;
//   _URC_HANDLER_FOUND;
//   _URC_INSTALL_CONTEXT;
//   _URC_CONTINUE_UNWIND;
        return _URC_FATAL_PHASE1_ERROR;
    }

    
    void _Unwind_Resume(struct _Unwind_Exception * object){
        
    }
    
    int __cxa_atexit(void (*f)(void *), void *objptr, void *dso)
    {
        return 0;
    };
    
    void __cxa_finalize(void *f)
    {

    };
    
    void __cxa_call_unexpected(void *u){
        
    };
    
    void __cxa_begin_catch(void *c){
    
    };
    
    int __cxa_guard_acquire(uint64_t* guard_object){
        return 0;
    };
    
    void __cxa_guard_release(uint64_t* guard_object){
        
    };
    
};
