#include <stdlib.h>

template<class T,class A>
T tldiv(A num,A den){
    return (T){ num/den, num%den };
}


extern "C" {
    
    div_t div(int num, int den){
        return tldiv<div_t,int>(num,den);
    }
    
    ldiv_t ldiv(long num, long den){
       return tldiv<ldiv_t,long>(num,den); 
    }
    
     lldiv_t lldiv(long long num, long long den){
       return tldiv<lldiv_t,long long>(num,den); 
    }
    
};
