#include "syscall.h"
#include "sysbits.h"

namespace std {
    void exit(int exit_code){
        syscall1(__NR_exit_group,exit_code);
    };
};

extern "C" void exit(int exit_code){
    std::exit(exit_code);
};
