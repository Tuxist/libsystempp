/*******************************************************************************
 * Copyright (c) 2021, Jan Koester jan.koester@gmx.net
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 * Neither the name of the <organization> nor the
 *      names of its contributors may be used to endorse or promote products
 *      derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************/

#pragma once


#ifdef __cplusplus
extern "C" {
#endif
   typedef void* pthread_t;
   typedef void* pthread_attr_t;
   typedef void* pthread_mutex_t;
   typedef void* pthread_mutexattr_t;
   
   /*thread*/
   int pthread_attr_init(pthread_attr_t *);
   int pthread_attr_destroy(pthread_attr_t *);
   int pthread_join(pthread_t thread, void **);
   
   int pthread_create(pthread_t* __restrict__ thread,
                          const pthread_attr_t* __restrict__ attr,
                          void *(*start_routine)(void *),
                          void* __restrict__ arg);

   void  pthread_exit(void *);
   
   /*mutex atrribute*/
   int   pthread_mutexattr_init(pthread_mutexattr_t *matr);
   int   pthread_mutexattr_destroy(pthread_mutexattr_t *matr);
   
   int   pthread_mutex_init(pthread_mutex_t *__restrict__ mutex,
                            const pthread_mutexattr_t *__restrict__ matr);
   int   pthread_mutex_destroy(pthread_mutex_t *mutex);
   
   int   pthread_mutex_lock(pthread_mutex_t *mutex);
   int   pthread_mutex_trylock(pthread_mutex_t *mutex);
   int   pthread_mutex_unlock(pthread_mutex_t *mutex);
#ifdef __cplusplus
}
#endif
