#include <unwind.h>

#include "sysexception.h"

extern "C" {
#ifdef BUILD_SHARED
    __attribute__((weak)) void *__dso_handle = &__dso_handle;
#else
    __attribute__((weak)) void *__dso_handle = (void *)0;
#endif
}

static int errno;

__attribute__((visibility("hidden"))) int* __errno_location(void) { 
    return &errno; 
}
