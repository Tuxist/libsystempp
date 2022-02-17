#include <cstdlib>

#include "syscall.h"
#include "sysbits.h"


void std::exit(int exit_code){
        syscall1(__NR_exit_group,exit_code);
};

void exit(int exit_code){
    std::exit(exit_code);
};
