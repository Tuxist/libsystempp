# The __dso_handle global variable is used by static
# C++ constructors and destructors in the binary.
# See http://www.codesourcery.com/public/cxx-abi/abi.html#dso-dtor
#
.section .bss
.align 4
#ifndef CRT_LEGACY_WORKAROUND
.hidden __dso_handle
#endif
.globl __dso_handle

__dso_handle:
    .long 0

