#include <unwind.h>

#include "sysexception.h"

#ifdef BUILD_SHARED
extern void *__dso_handle __attribute__ ((__visibility__ ("hidden")));
#else
void *__dso_handle = 0;
#endif



static int errno;

__attribute__((visibility("hidden"))) int* __errno_location(void) { 
    return &errno; 
}
