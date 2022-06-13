//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#if defined(__MVS__)
// As part of monotonic clock support on z/OS we need macro _LARGE_TIME_API
// to be defined before any system header to include definition of struct timespec64.
#define _LARGE_TIME_API
#endif

#include <cerrno>        // errno
#include <chrono>
#include <system_error>  // __throw_system_error

#include <systempp/systime.h>

#if defined(__ELF__) && defined(_LIBCPP_LINK_RT_LIB)
#  pragma comment(lib, "rt")
#endif

_LIBCPP_BEGIN_NAMESPACE_STD

namespace chrono
{

//
// system_clock
//

static system_clock::time_point __libcpp_system_clock_now() {
    struct timeval tv;
    ::sys::Time time;
    time.getHWTime();
    time._toUnixtimeVal(tv);
    return system_clock::time_point(seconds(tv.tv_sec) + microseconds(tv.tv_usec));
}

const bool system_clock::is_steady;

system_clock::time_point
system_clock::now() noexcept
{
    return __libcpp_system_clock_now();
}

time_t
system_clock::to_time_t(const time_point& t) noexcept
{
    return time_t(duration_cast<seconds>(t.time_since_epoch()).count());
}

system_clock::time_point
system_clock::from_time_t(time_t t) noexcept
{
    return system_clock::time_point(seconds(t));
}

//
// steady_clock
//
// Warning:  If this is not truly steady, then it is non-conforming.  It is
//  better for it to not exist and have the rest of libc++ use system_clock
//  instead.
//

static steady_clock::time_point __libcpp_steady_clock_now() {
    ::sys::Time time;
    struct timespec tspec;
    time.getHWTime();
    time._toUnixtimeSpec(tspec);
    return steady_clock::time_point(seconds(tspec.tv_sec) + nanoseconds(tspec.tv_nsec));
}

const bool steady_clock::is_steady;

steady_clock::time_point
steady_clock::now() noexcept
{
    return __libcpp_steady_clock_now();
}

}

_LIBCPP_END_NAMESPACE_STD
