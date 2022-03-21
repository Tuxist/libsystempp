
#ifndef SYSTEMPP_EXPORT_H
#define SYSTEMPP_EXPORT_H

#ifdef SYSTEMPP_STATIC_DEFINE
#  define SYSTEMPP_EXPORT
#  define SYSTEMPP_NO_EXPORT
#else
#  ifndef SYSTEMPP_EXPORT
#    ifdef systempp_EXPORTS
        /* We are building this library */
#      define SYSTEMPP_EXPORT 
#    else
        /* We are using this library */
#      define SYSTEMPP_EXPORT 
#    endif
#  endif

#  ifndef SYSTEMPP_NO_EXPORT
#    define SYSTEMPP_NO_EXPORT 
#  endif
#endif

#ifndef SYSTEMPP_DEPRECATED
#  define SYSTEMPP_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef SYSTEMPP_DEPRECATED_EXPORT
#  define SYSTEMPP_DEPRECATED_EXPORT SYSTEMPP_EXPORT SYSTEMPP_DEPRECATED
#endif

#ifndef SYSTEMPP_DEPRECATED_NO_EXPORT
#  define SYSTEMPP_DEPRECATED_NO_EXPORT SYSTEMPP_NO_EXPORT SYSTEMPP_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef SYSTEMPP_NO_DEPRECATED
#    define SYSTEMPP_NO_DEPRECATED
#  endif
#endif

#endif /* SYSTEMPP_EXPORT_H */
