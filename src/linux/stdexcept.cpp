//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include <new>
#include <stdexcept>
#include <string>
#include <system_error>

static_assert(sizeof(std::__libcpp_refstring) == sizeof(const char*), "");

namespace std {

//     logic_error::logic_error(const string& msg) : __imp_(msg.c_str()) {}
// 
//     logic_error::logic_error(const char* msg) : __imp_(msg) {}
// 
//     logic_error::logic_error(const logic_error& le) noexcept : __imp_(le.__imp_) {}
// 
//     logic_error& logic_error::operator=(const logic_error& le) noexcept {
//         __imp_ = le.__imp_;
//         return *this;
//     }
// 
//     runtime_error::runtime_error(const string& msg) : __imp_(msg.c_str()) {}
// 
//     runtime_error::runtime_error(const char* msg) : __imp_(msg) {}
// 
//     runtime_error::runtime_error(const runtime_error& re) noexcept
//         : __imp_(re.__imp_) {}
// 
//     runtime_error& runtime_error::operator=(const runtime_error& re) noexcept {
//         __imp_ = re.__imp_;
//         return *this;
//     }
};
