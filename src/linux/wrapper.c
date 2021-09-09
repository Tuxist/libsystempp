/*******************************************************************************
Copyright (c) 2020, Jan Koester jan.koester@gmx.net
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

//fixes the bullshit bingo names in linux 

#include "linux/mutex.h"
#include "linux/clone.h"

void wrapper_mutex_init(wrapper_mutex **mymutex){
    mutex_init((struct **mutex)mymutex);
}

void wrapper_mutex_destroy(wrapper_mutex *mymutex){
    mutex_destroy((struct *mutex)mymutex);
}

void wrapper_mutex_lock(wrapper_mutex *mymutex){
    mutex_lock((struct *mutex)mymutex);
}

int wrapper_mutex_trylock(wrapper_mutex *mymutex){
    mutex_trylock((struct *mutex)mymutex);
}

int wrapper_mutex_unlock(wrapper_mutex *mymutex){
    mutex_unlock((struct *mutex)mymutex);
}

int wrapper_mutex_is_locked(wrapper_mutex *mymutex){
    mutex_is_locked((struct *mutex)mymutex);
}

int wrapper_clone_get_clone_thread_flag(){
    return CLONE_THREAD;
}

int wrapper_clone2(void *fn,void *child_stack,size_t stack_size,int flags,void *args){
    __clone2(fn,child_stack,stack_size,flags,args);
}
