/*******************************************************************************
Copyright (c) 2022, Jan Koester jan.koester@gmx.net
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/

#include <concepts>
#include <type_traits>

#pragma once

namespace std {
    
    template<class> struct __cond_value_type {};
    
    template<class _Tp>
    requires is_object_v<_Tp>
    struct __cond_value_type<_Tp> { using value_type = remove_cv_t<_Tp>; };
    
    template<class _Tp>
    concept __has_member_value_type = requires { typename _Tp::value_type; };
    
    template<class _Tp>
    concept __has_member_element_type = requires { typename _Tp::element_type; };
    
    template<class> struct indirectly_readable_traits {};
    
    template<class _Ip>
    requires is_array_v<_Ip>
    struct indirectly_readable_traits<_Ip> {
        using value_type = remove_cv_t<remove_extent_t<_Ip>>;
    };
    
    template<class _Ip>
    struct indirectly_readable_traits<const _Ip> : indirectly_readable_traits<_Ip> {};
    
    template<class _Tp>
    struct indirectly_readable_traits<_Tp*> : __cond_value_type<_Tp> {};
    
    template<__has_member_value_type _Tp>
    struct indirectly_readable_traits<_Tp>
    : __cond_value_type<typename _Tp::value_type> {};
    
    template<__has_member_element_type _Tp>
    struct indirectly_readable_traits<_Tp>
    : __cond_value_type<typename _Tp::element_type> {};
    
    template<__has_member_value_type _Tp>
    requires __has_member_element_type<_Tp>
    struct indirectly_readable_traits<_Tp> {};
    
    template<__has_member_value_type _Tp>
    requires __has_member_element_type<_Tp> &&
    same_as<remove_cv_t<typename _Tp::element_type>,
    remove_cv_t<typename _Tp::value_type>>
    struct indirectly_readable_traits<_Tp>
    : __cond_value_type<typename _Tp::value_type> {};
    
    template <class>
    struct iterator_traits;
    
    // Let `RI` be `remove_cvref_t<I>`. The type `iter_value_t<I>` denotes
    // `indirectly_readable_traits<RI>::value_type` if `iterator_traits<RI>` names a specialization
    // generated from the primary template, and `iterator_traits<RI>::value_type` otherwise.
    template <class _Ip>
    using iter_value_t = typename conditional_t<__is_primary_template<iterator_traits<remove_cvref_t<_Ip> > >::value,
                                                indirectly_readable_traits<remove_cvref_t<_Ip> >,
                                                iterator_traits<remove_cvref_t<_Ip> > >::value_type;    
};

