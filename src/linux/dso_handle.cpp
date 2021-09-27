#include <unwind.h>

#include "sysexception.h"

#ifdef BUILD_SHARED
void *__dso_handle = &__dso_handle;
#else
void *__dso_handle = (void *)0;
#endif

static int errno;

__attribute__((visibility("hidden"))) int* __errno_location(void) { 
    return &errno; 
}
