//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include <__config>

#include <limits>
#include <random>
#include <system_error>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/random.h>
#include <fcntl.h>
#include <unistd.h>
#if __has_include(<sys/ioctl.h>) && __has_include(<linux/random.h>)
#       include <sys/ioctl.h>
#       include <linux/random.h>
#endif


_LIBCPP_BEGIN_NAMESPACE_STD

random_device::random_device(const string& __token)
{
    if (__token != "/dev/urandom")
        __throw_system_error(ENOENT, ("random device not supported " + __token).c_str());
}

random_device::~random_device()
{
}

unsigned
random_device::operator()()
{
    unsigned r;
    size_t n = sizeof(r);
    int err = getentropy(&r, n);
    if (err)
        __throw_system_error(errno, "random_device getentropy failed");
    return r;
}


double
random_device::entropy() const noexcept
{
  int ent;
  if (::ioctl(__f_, RNDGETENTCNT, &ent) < 0)
    return 0;

  if (ent < 0)
    return 0;

  if (ent > std::numeric_limits<result_type>::digits)
    return std::numeric_limits<result_type>::digits;

  return ent;
  return 0;
}

_LIBCPP_END_NAMESPACE_STD
