#include <exception>

#include "syscall.h"

void std::terminate() noexcept{
    syscall0(__NR_exit_group);
};
