#include <unwind.h>
#include <inttypes.h>
#include <typeinfo>
#include <cxxabi.h>

#include "sysexception.h"

#ifdef BUILD_SHARED
void *__dso_handle = &__dso_handle;
#else
void *__dso_handle = (void *)0;
#endif

static int errno;

__attribute__((visibility("hidden"))) int* __errno_location(void) { 
    return &errno; 
}

bool __cxxabiv1::__si_class_type_info::__do_dyncast(
    ptrdiff_t __src2dst, __sub_kind __access_path,
    const __class_type_info* __dst_type, const void* __obj_ptr,
    const __class_type_info* __src_type, const void* __src_ptr,
    __dyncast_result& __result) const{
        
}
    

                                                           
bool __cxxabiv1::__si_class_type_info::__do_upcast(
    const __class_type_info*__dst, const void*__obj,
    __upcast_result& __restrict __result) const{
        
};
                                                                                                         
__cxxabiv1::__si_class_type_info::~__si_class_type_info(){
        
}

__cxxabiv1::__class_type_info::~__class_type_info(){
        
}

__cxxabiv1::__si_class_type_info::__sub_kind 
__cxxabiv1::__class_type_info::__do_find_public_src(long, 
    void const*, 
    __cxxabiv1::__class_type_info const*, 
    void const*) const{
};

__cxxabiv1::__si_class_type_info::__sub_kind
__cxxabiv1::__si_class_type_info::__do_find_public_src(
    long, 
    void const*,
    __cxxabiv1::__class_type_info const*,
    void const*) const{
    
};

bool __cxxabiv1::__class_type_info::__do_catch(type_info const*, 
                                               void**, 
                                               unsigned int) const{
    
};

bool __cxxabiv1::__class_type_info::__do_upcast(__cxxabiv1::__class_type_info const*, 
                                                void**) const{
    
};

bool __cxxabiv1::__class_type_info::__do_upcast(__cxxabiv1::__class_type_info const*, 
                                                void const*, 
                                                __cxxabiv1::__class_type_info::__upcast_result&) const{
    
}

bool __cxxabiv1::__class_type_info::__do_dyncast(long, 
                                                 __cxxabiv1::__class_type_info::__sub_kind, 
                                                 __cxxabiv1::__class_type_info const*, void const*, 
                                                 __cxxabiv1::__class_type_info              const*, 
                                                 void const*, 
                                                 __cxxabiv1::__class_type_info::__dyncast_result&) const{
    
};


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
    
    int __cxa_atexit(void (*f)(void *), void *objptr, void *dso){
        return 0;
    };
    
    int __cxa_finalize(void *f){

    };
    
    void __cxa_call_unexpected(void *u){
        
    };
    
    void *__cxa_begin_catch(void *c) _GLIBCXX_NOTHROW{
    
    };
    
    int __cxa_guard_acquire(__cxxabiv1::__guard *guard_object){
        return 0;
    };
    
    void __cxa_guard_release(__cxxabiv1::__guard *guard_object) _GLIBCXX_NOTHROW{
        
    };
    
    void *__cxa_allocate_exception(unsigned long thrown_size){
    };
    
    void __cxa_throw (void *thrown_exception,std::type_info *tinfo, void (*dest) (void *) ){
    };
    
    void __cxa_free_exception(void *thrown_exception){
        
    };
};
