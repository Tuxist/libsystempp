#include <systempp/sysinfo.h>
#include <systempp/sysutils.h>

class _cpuIDinfo {
public:
    unsigned long int EAX;
    unsigned long int EBX;
    unsigned long int ECX;
    unsigned long int EDX;
};

static inline void _cpuid(unsigned int func, unsigned int subfunc,_cpuIDinfo* info){
    __asm__ __volatile__ (
        "cpuid"
        : "=a"(info->EAX), "=b"(info->EBX), "=c"(info->ECX), "=d"(info->EDX)
        : "a"(func), "c"(subfunc)
    );
}

sys::CpuInfo::CpuInfo(){
    _cpuinfo=new _cpuIDinfo;
}

sys::CpuInfo::~CpuInfo(){
    delete   (_cpuIDinfo*)_cpuinfo;
}

int sys::CpuInfo::getCores(){
    unsigned short cores=1;
    const char *vendor=getVendor();
    if (ncompare(vendor,getlen(vendor),"GenuineIntel",12)==0) {
        _cpuid(0x0004,0x0000, (_cpuIDinfo*)_cpuinfo);
        cores = ((((_cpuIDinfo*)_cpuinfo)->EAX >> 26) & 0xff)+1;
        
    } else if (ncompare(vendor,getlen(vendor),"AuthenticAMD",12)==0) {
        _cpuid(0x0000,0x80000008, (_cpuIDinfo*)_cpuinfo);
        cores = ((unsigned)((((_cpuIDinfo*)_cpuinfo)->ECX >> 24)  & 0xff)) + 1;
    }
    return cores;
}

int sys::CpuInfo::getThreads(){
    _cpuid(0x0001,0x0000, (_cpuIDinfo*)_cpuinfo);
    return ((unsigned)((((_cpuIDinfo*)_cpuinfo)->EBX >> 16)  & 0xff));
}

int sys::CpuInfo::getActualThread(){
    const char *vendor=getVendor();
    if (ncompare(vendor,getlen(vendor),"GenuineIntel",12)==0) {
        _cpuid(0x0001,0x0000, (_cpuIDinfo*)_cpuinfo);
        return (((_cpuIDinfo*)_cpuinfo)->EBX >> 24 ) & 0xff;
    }
    return 0;
}

const char* sys::CpuInfo::getVendor(){
    _cpuid(0x0000,0x0000,(_cpuIDinfo*)_cpuinfo);
    _Vendor.clear();
    
    _Vendor.push_back((char)((((_cpuIDinfo*)_cpuinfo)->EBX >> 0) & 0xff));
    _Vendor.push_back((char)((((_cpuIDinfo*)_cpuinfo)->EBX >> 8) & 0xff));
    _Vendor.push_back((char)((((_cpuIDinfo*)_cpuinfo)->EBX >> 16) & 0xff));
    _Vendor.push_back((char)((((_cpuIDinfo*)_cpuinfo)->EBX >> 24) & 0xff));
    
    _Vendor.push_back((char)(((_cpuIDinfo*)_cpuinfo)->EDX >> 0) & 0xff);
    _Vendor.push_back((char)(((_cpuIDinfo*)_cpuinfo)->EDX >> 8) & 0xff);
    _Vendor.push_back((char)(((_cpuIDinfo*)_cpuinfo)->EDX >> 16) & 0xff);
    _Vendor.push_back((char)(((_cpuIDinfo*)_cpuinfo)->EDX >> 24) & 0xff);
    
    _Vendor.push_back((char)(((_cpuIDinfo*)_cpuinfo)->ECX >> 0) & 0xff);
    _Vendor.push_back((char)(((_cpuIDinfo*)_cpuinfo)->ECX >> 8) & 0xff);
    _Vendor.push_back((char)(((_cpuIDinfo*)_cpuinfo)->ECX >> 16) & 0xff);
    _Vendor.push_back((char)(((_cpuIDinfo*)_cpuinfo)->ECX >> 24) & 0xff);        
    return _Vendor.c_str();
}
