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

#include <type_traits>

#pragma once

namespace std {
    // pair piecewise construction
    struct piecewise_construct_t {
        explicit piecewise_construct_t() = default;
    };
    inline constexpr piecewise_construct_t piecewise_construct{};
    template<class... Types> class tuple; 
    
    // class template pair
    template<class T1, class T2>
        struct pair{
            typedef T1 first_type;
            typedef T2 second_type;
            T1 first;
            T2 second;
            pair(const pair&) = default;
            pair(pair&&) = default;
            explicit constexpr pair();
            explicit pair(const T1& x, const T2& y);                         
            template <class U = T1, class V = T2> explicit pair(U&&, V&&);    
            template <class U, class V> explicit pair(const pair<U, V>& p);   
            template <class U, class V> explicit pair(pair<U, V>&& p);        
            template <class... Args1, class... Args2>
            pair(piecewise_construct_t, tuple<Args1...> first_args,tuple<Args2...> second_args);                                       
            template <class U, class V> pair& operator=(const pair<U, V>& p);            
            pair& operator=(pair&& p) noexcept(is_nothrow_move_assignable<T1>::value &&
                                                is_nothrow_move_assignable<T2>::value);   
            template <class U, class V> pair& operator=(pair<U, V>&& p);                 
            void swap(pair& p) noexcept(is_nothrow_swappable_v<T1> &&
                                        is_nothrow_swappable_v<T2>); 
 
        };  
};
