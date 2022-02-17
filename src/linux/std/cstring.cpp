#include <cstring>

namespace std {
    void *memset( void * ptr, int value, size_t num ){
        unsigned char* p=(unsigned char*)ptr;
        while(num--){
            *p++ = (unsigned char)value;
        }
        return p;
    }
    
    void *memcpy(void* dst, const void* src, size_t size){
        for ( size_t i = 0; i < size; i++ )
            ((uint8_t*) dst)[i] = ((const uint8_t*) src)[i];
        return dst;
    }
    
    void *memmove(void* dst, const void* src, size_t size){
        char *pDest = (char *)dst;
        const char *pSrc =( const char*)src;
        char *tmp  = new char[(sizeof(char ) * size)];
        if(nullptr == tmp){
            return nullptr;
        }
        else{
            unsigned int i = 0;
            for(i =0; i < size ; ++i){
                *(tmp + i) = *(pSrc + i);
            }
            for(i =0 ; i < size ; ++i){
                *(pDest + i) = *(tmp + i);
            }
            delete[] tmp;
        }
        return dst;
    }
    
    size_t strlen(const char * str){
        size_t i = 0;
        while(str[i] != '\0')
            ++i;
        return i;
    }
};

extern "C" void *memset( void * ptr, int value, std::size_t num ){
    return std::memset(ptr,value,num);
}

extern "C" void *memcpy(void* dst, const void* src, unsigned int size){
    return std::memcpy(dst,src, size);
}

extern "C" void *memmove(void* dst, const void* src, unsigned int size){
    return std::memmove(dst,src, size);
}

extern "C" unsigned int strlen(const char * str){
    return std::strlen(str);
}
