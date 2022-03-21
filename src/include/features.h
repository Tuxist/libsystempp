#undef __USE_GNU
#undef __GNUC__

# define _POSIX_SOURCE 1
#define __GLIBC_PREREQ(maj, min) ((__GLIBC__ << 16) + __GLIBC_MINOR__ >= ((maj) << 16) + (min))
