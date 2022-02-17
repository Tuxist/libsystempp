
#pragma once

#include <cstddef>

#include "config.h"

#ifdef __cplusplus

namespace std {
    class type_info; // forward declaration
}

namespace __cxxabiv1 {
    extern "C"  {
        extern __attribute__((__visibility__("default"))) void *__cxa_allocate_exception(std::size_t thrown_size) throw();
        extern __attribute__((__visibility__("default"))) void __cxa_free_exception(void *thrown_exception) throw();
        
        // 2.4.3 Throwing the Exception Object
        extern __attribute__((__visibility__("default"))) __attribute__((noreturn)) void __cxa_throw(void *thrown_exception, std::type_info *tinfo,
                    void (*dest)(void *));
        
        // 2.5.3 Exception Handlers
        extern __attribute__((__visibility__("default"))) void *__cxa_get_exception_ptr(void *exceptionObject) throw();
        extern __attribute__((__visibility__("default"))) void *__cxa_begin_catch(void *exceptionObject) throw();
        extern __attribute__((__visibility__("default"))) void __cxa_end_catch();
        
        extern __attribute__((__visibility__("default"))) std::type_info *__cxa_current_exception_type();
        
        // 2.5.4 Rethrowing Exceptions
        extern __attribute__((__visibility__("default"))) __attribute__((noreturn)) void __cxa_rethrow();
        
        // 2.6 Auxiliary Runtime APIs
        extern __attribute__((__visibility__("default"))) __attribute__((noreturn)) void __cxa_bad_cast(void);
        extern __attribute__((__visibility__("default"))) __attribute__((noreturn)) void __cxa_bad_typeid(void);
        extern __attribute__((__visibility__("default"))) __attribute__((noreturn)) void __cxa_throw_bad_array_new_length(void);
        
        // 3.2.6 Pure Virtual Function API
        extern __attribute__((__visibility__("default"))) __attribute__((noreturn)) void __cxa_pure_virtual(void);
        
        // 3.2.7 Deleted Virtual Function API
        extern __attribute__((__visibility__("default"))) __attribute__((noreturn)) void __cxa_deleted_virtual(void);
        
        
        
        // 3.3.3 Array Construction and Destruction API
        extern __attribute__((__visibility__("default"))) void *
        __cxa_vec_new(std::size_t element_count, std::size_t element_size, std::size_t padding_size,
                      void (*constructor)(void *), void (*destructor)(void *));
        
        extern __attribute__((__visibility__("default"))) void *
        __cxa_vec_new2(std::size_t element_count, std::size_t element_size, std::size_t padding_size,
                       void (*constructor)(void *), void (*destructor)(void *),
                       void *(*alloc)(std::size_t), void (*dealloc)(void *));
        
        extern __attribute__((__visibility__("default"))) void *
        __cxa_vec_new3(std::size_t element_count, std::size_t element_size, std::size_t padding_size,
                       void (*constructor)(void *), void (*destructor)(void *),
                       void *(*alloc)(std::size_t), void (*dealloc)(void *, std::size_t));
        
        extern __attribute__((__visibility__("default"))) void
        __cxa_vec_ctor(void *array_address, std::size_t element_count, std::size_t element_size,
                       void (*constructor)(void *), void (*destructor)(void *));
        
        extern __attribute__((__visibility__("default"))) void __cxa_vec_dtor(void *array_address,
                                                                              std::size_t element_count,
                                                                              std::size_t element_size,
                                                                              void (*destructor)(void *));
        
        extern __attribute__((__visibility__("default"))) void __cxa_vec_cleanup(void *array_address,
                                                                                 std::size_t element_count,
                                                                                 std::size_t element_size,
                                                                                 void (*destructor)(void *));
        
        extern __attribute__((__visibility__("default"))) void __cxa_vec_delete(void *array_address,
                                                                                std::size_t element_size,
                                                                                std::size_t padding_size,
                                                                                void (*destructor)(void *));
        
        extern __attribute__((__visibility__("default"))) void
        __cxa_vec_delete2(void *array_address, std::size_t element_size, std::size_t padding_size,
                          void (*destructor)(void *), void (*dealloc)(void *));
        
        extern __attribute__((__visibility__("default"))) void
        __cxa_vec_delete3(void *__array_address, std::size_t element_size,
                          std::size_t padding_size, void (*destructor)(void *),
                          void (*dealloc)(void *, std::size_t));
        
        extern __attribute__((__visibility__("default"))) void
        __cxa_vec_cctor(void *dest_array, void *src_array, std::size_t element_count,
                        std::size_t element_size, void (*constructor)(void *, void *),
                        void (*destructor)(void *));
        
        // 3.3.5.3 Runtime API
        // These functions are part of the C++ ABI, but they are not defined in libc++abi:
        //    int __cxa_atexit(void (*)(void *), void *, void *);
        //    void __cxa_finalize(void *);
        
        // 3.4 Demangler API
        extern __attribute__((__visibility__("default"))) char *__cxa_demangle(const char *mangled_name,
                                                                               char *output_buffer,
                                                                               std::size_t *length, int *status);
        
        // Apple additions to support C++ 0x exception_ptr class
        // These are primitives to wrap a smart pointer around an exception object
        extern __attribute__((__visibility__("default"))) void *__cxa_current_primary_exception() throw();
        extern __attribute__((__visibility__("default"))) void __cxa_rethrow_primary_exception(void *primary_exception);
        extern __attribute__((__visibility__("default"))) void __cxa_increment_exception_refcount(void *primary_exception) throw();
        extern __attribute__((__visibility__("default"))) void __cxa_decrement_exception_refcount(void *primary_exception) throw();
        
        
        extern __attribute__((__visibility__("default"))) bool __cxa_uncaught_exception() throw();
        extern __attribute__((__visibility__("default"))) unsigned int __cxa_uncaught_exceptions() throw();
        
        extern __attribute__((__visibility__("default"))) int __cxa_thread_atexit(void (*)(void *), void *,void *) throw();
        
    }
}

namespace abi = __cxxabiv1;

#endif
