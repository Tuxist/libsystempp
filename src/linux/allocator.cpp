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

char memory[20000];

struct block{
    unsigned long size;
    int free;
    struct block *next; 
};

struct block *freeList=(block*)&memory;


void initialize(){
    freeList->size=20000-sizeof(struct block);
    freeList->free=1;
    freeList->next=nullptr;
}

void split(struct block *fitting_slot,unsigned long size){
    struct block *nb=(block*)(&fitting_slot+size+sizeof(struct block));
    nb->size=(fitting_slot->size)-size-sizeof(struct block);
    nb->free=1;
    nb->next=fitting_slot->next;
    fitting_slot->size=size;
    fitting_slot->free=0;
    fitting_slot->next=nb;
}

void merge(){
    struct block *curr,*prev;
    curr=freeList;
    while((curr->next)!=nullptr){
        if((curr->free) && (curr->next->free)){
            curr->size+=(curr->next->size)+sizeof(struct block);
            curr->next=curr->next->next;
        }
        prev=curr;
        curr=curr->next;
    }
}

void *_alloc(unsigned long size){
    struct block *curr,*prev;
    void *result;
    if(!(freeList->size)){
        initialize();
    }
    curr=freeList;
    while((((curr->size)<size)||((curr->free)==0))&&(curr->next!=nullptr)){
        prev=curr;
        curr=curr->next;
    }
    if((curr->size)==size){
        curr->free=0;
        result=(void*)(++curr);
        return result;
    }
    else if((curr->size)>(size+sizeof(struct block))){
        split(curr,size);
        result=(void*)(++curr);
        return result;
    }else{
        result=nullptr;
        return result;
    }    
}

void *operator new(unsigned long size){
    return _alloc(size);
}

void *operator new[](unsigned long size){
    return _alloc(size);
}

void _delete(void *ptr){
    if(((void*)memory<=ptr)&&(ptr<=(void*)(memory+20000))){
        struct block* curr=(block*)ptr;
        --curr;
        curr->free=1;
        merge();
    }    
}

void operator delete (void* ptr) throw(){
    _delete(ptr);
}

void operator delete(void* ptr,unsigned long amount){
    _delete(ptr);
}

void operator delete[](void* ptr,unsigned long amount){
    _delete(ptr);
}
