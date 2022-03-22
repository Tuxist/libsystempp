#include <concepts>

#pragma once

namespace std {
    namespace __detail {
        template<typename T>
        using __with_ref = T&;
        template<typename T>
        concept __can_reference = requires { typename __with_ref<T>; };
        
        template<typename T>
        concept __dereferenceable = requires(T& __t){
            { *__t } -> __can_reference;
        };
        
        template<typename _Iter>
        struct __iter_concept_impl;
        
        // ITER_CONCEPT(I) is ITER_TRAITS(I)::iterator_concept if that is valid.
        template<typename _Iter>
        requires requires { typename __iter_traits<_Iter>::iterator_concept; }
        struct __iter_concept_impl<_Iter>
        { using type = typename __iter_traits<_Iter>::iterator_concept; };
        
        // Otherwise, ITER_TRAITS(I)::iterator_category if that is valid.
        template<typename _Iter>
        requires (!requires { typename __iter_traits<_Iter>::iterator_concept; }
        && requires { typename __iter_traits<_Iter>::iterator_category; })
        struct __iter_concept_impl<_Iter>
        { using type = typename __iter_traits<_Iter>::iterator_category; };
        
        // Otherwise, random_access_tag if iterator_traits<I> is not specialized.
        template<typename _Iter>
        requires (!requires { typename __iter_traits<_Iter>::iterator_concept; }
        && !requires { typename __iter_traits<_Iter>::iterator_category; }
        && __primary_traits_iter<_Iter>)
        struct __iter_concept_impl<_Iter>
        { using type = random_access_iterator_tag; };
        
        // Otherwise, there is no ITER_CONCEPT(I) type.
        template<typename _Iter>
        struct __iter_concept_impl
        { };
        
        template<typename _Iter>
        using __iter_concept = typename __iter_concept_impl<_Iter>::type;
    };
    
     template<typename _Iter>
     concept input_iterator = input_or_output_iterator<_Iter>
       && indirectly_readable<_Iter>
       && requires { typename __detail::__iter_concept<_Iter>; }
       && derived_from<__detail::__iter_concept<_Iter>, input_iterator_tag>;
};
