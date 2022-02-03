#include <stddef.h>
#include <stdint.h>

namespace std {
    void *memset( void * ptr, int value, size_t num ){
        unsigned char* p=(unsigned char*)ptr;
        while(num--){
            *p++ = (unsigned char)value;
        }
        return p;
    }
    
    void* memcpy(void* dst, const void* src, size_t size){
        for ( size_t i = 0; i < size; i++ )
            ((uint8_t*) dst)[i] = ((const uint8_t*) src)[i];
        return dst;
    }
};

extern "C" void *memset( void * ptr, int value, size_t num ){
    return std::memset(ptr,value,num);
}

extern "C" void* memcpy(void* dst, const void* src, size_t size){
    return std::memcpy(dst,src, size);
}
