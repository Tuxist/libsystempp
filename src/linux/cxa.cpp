#include <cxxabi.h>
#include <typeinfo>

namespace __cxxabiv1 {
    
    bool __si_class_type_info::__do_dyncast(
        ptrdiff_t __src2dst, __sub_kind __access_path,
        const __class_type_info* __dst_type, const void* __obj_ptr,
        const __class_type_info* __src_type, const void* __src_ptr,
        __dyncast_result& __result) const{
            
        }
        
        
        
        bool __si_class_type_info::__do_upcast(
            const __class_type_info*__dst, const void*__obj,
            __upcast_result& __restrict __result) const{
                
        };
            
        __si_class_type_info::~__si_class_type_info(){
                
        }
            
        __class_type_info::~__class_type_info(){
                
        }
            
        __si_class_type_info::__sub_kind 
        __class_type_info::__do_find_public_src(long, 
            void const*, 
            __cxxabiv1::__class_type_info const*, 
            void const*) const{
                
        };
                                                                
        __si_class_type_info::__sub_kind
        __si_class_type_info::__do_find_public_src(
            long, 
            void const*,
            __class_type_info const*,
            void const*) const{
                                                                        
        };
                                                                    
        bool __class_type_info::__do_catch(type_info const*, 
            void**, 
            unsigned int) const{
                                                                                                                       
        };
                                                                                                                   
        bool __class_type_info::__do_upcast(
            __class_type_info const*, 
            void**) const{
                                                                                                                                                                       
        };
                                                                                                                                                                   
        bool __class_type_info::__do_upcast(
            __class_type_info const*,
            void const*, 
            __class_type_info::__upcast_result&) const{
                
        }
        
        bool __class_type_info::__do_dyncast(long,                                                                                                                                                                                                                                                                                                                          __class_type_info::__sub_kind,                                                                                                                                                               __class_type_info const*, 
        void const*,                                                                                                                                                                                                                                        __class_type_info const*, 
        void const*, 
        __class_type_info::__dyncast_result&) const{
                                                
        };
};

extern "C" {
    int __cxa_atexit(void (*f)(void *), void *objptr, void *dso){
        return 0;
    };
    
    int __cxa_finalize(void *f){

    };
    
    void __cxa_call_unexpected(void *u){
        
    };
    
    void *__cxa_begin_catch(void *c) _GLIBCXX_NOTHROW{
    
    };
    
    void __cxa_end_catch(){
    
        
    }
    
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

    void __cxa_throw_bad_array_new_length(){
      
    };
    
    void __cxa_pure_virtual() { 
        while (1);
    }
    
    void __cxa_guard_abort(__cxxabiv1::__guard* guard_object){
    };
    
};
