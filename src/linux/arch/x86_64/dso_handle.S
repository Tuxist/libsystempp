# The __dso_handle global variable is used by static
# C++ constructors and destructors in the binary.
# See http://www.codesourcery.com/public/cxx-abi/abi.html#dso-dtor
#

#ifdef BUILD_SHARED_LIBS
.data
    .align 4
    .hidden __dso_handle
    .globl __dso_handle
__dso_handle:
    .long __dso_handle
#else
.section .bss
    .align 4
    .hidden __dso_handle
    .globl __dso_handle
__dso_handle:
    .long 0
#endif
