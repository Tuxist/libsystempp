/*******************************************************************************
Copyright (c) 2022, Jan Koester jan.koester@gmx.net
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

extern "C" {    
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
    
    extern void *__dso_handle __attribute__ ((__visibility__ ("hidden")));
    
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
    
    void __cxa_free_exception(void *e){
        
    };
    
    void __cxa_pure_virtual(){
        
    };
    
    void __cxa_throw (void *thrown_exception, void *tinfo, void (*dest) (void *) ){
    
    }
    
    void *__cxa_allocate_exception(unsigned long thrown_size){
        return nullptr;
    };
    
    __extension__ typedef int __guard __attribute__((mode(__DI__)));
 
    int __cxa_guard_acquire (__guard *);
    
    void __cxa_guard_release (__guard *);
    
    extern "C" void __cxa_guard_abort (__guard *);
 
    extern "C" int __cxa_guard_acquire (__guard *g) {
        return !*(char *)(g);
    }
 
    extern "C" void __cxa_guard_release (__guard *g){
        *(char *)g = 1;
    }
 
    extern "C" void __cxa_guard_abort (__guard *){
 
    }
    
};

