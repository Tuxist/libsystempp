#include "systempp/sysexception.h"

static int errno;

__attribute__((visibility("hidden"))) int* __errno_location(void) { 
    return &errno; 
}
