#include <unwind.h>
#include <stdint.h>

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

    #define ATEXIT_MAX_FUNCS 128

    typedef unsigned uarch_t;
    
    class atexit_func_entry_t {
    public:
        void (*destructor_func)(void *);
        void *obj_ptr;
        void *dso_handle;
    };
    
    atexit_func_entry_t __atexit_funcs[ATEXIT_MAX_FUNCS];
    uarch_t __atexit_func_count = 0;
    
    int __cxa_atexit(void (*f)(void *), void *objptr, void *dso)
    {
        if (__atexit_func_count >= ATEXIT_MAX_FUNCS) {return -1;};
        __atexit_funcs[__atexit_func_count].destructor_func = f;
        __atexit_funcs[__atexit_func_count].obj_ptr = objptr;
        __atexit_funcs[__atexit_func_count].dso_handle = dso;
        __atexit_func_count++;
        return 0;
    };
    
    void __cxa_finalize(void *f)
    {
        uarch_t i = __atexit_func_count;
        if (!f)
        {
            while (i--)
            {
                if (__atexit_funcs[i].destructor_func)
                {
                    (*__atexit_funcs[i].destructor_func)(__atexit_funcs[i].obj_ptr);
                };
            };
            return;
        };
        
        while (i--)
        {
            if (__atexit_funcs[i].destructor_func == f)
            {
                (*__atexit_funcs[i].destructor_func)(__atexit_funcs[i].obj_ptr);
                __atexit_funcs[i].destructor_func = 0;
                
            };
        };
    };
    
    void __cxa_call_unexpected(void *u){
        
    };
    
    void __cxa_begin_catch(void *c){
    
    };
};
