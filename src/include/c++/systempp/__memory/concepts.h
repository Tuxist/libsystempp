
namespace std {
    namespace ranges {
        template <class _Ip>
        concept __nothrow_input_iterator = input_iterator<_Ip> &&
                                           is_lvalue_reference_v<iter_reference_t<_Ip>> &&
                                           same_as<remove_cvref_t<iter_reference_t<_Ip>>, iter_value_t<_Ip>>;

template <class _Sp, class _Ip>
concept __nothrow_sentinel_for = sentinel_for<_Sp, _Ip>;

template <class _Rp>
concept __nothrow_input_range =
    range<_Rp> &&
    __nothrow_input_iterator<iterator_t<_Rp>> &&
    __nothrow_sentinel_for<sentinel_t<_Rp>, iterator_t<_Rp>>;

template <class _Ip>
concept __nothrow_forward_iterator =
    __nothrow_input_iterator<_Ip> &&
    forward_iterator<_Ip> &&
    __nothrow_sentinel_for<_Ip, _Ip>;

template <class _Rp>
concept __nothrow_forward_range =
    __nothrow_input_range<_Rp> &&
    __nothrow_forward_iterator<iterator_t<_Rp>>;

} 
};
