#pragma once
#include <type_traits>

template <typename T>
struct EnableBitMaskOperators {
    static constexpr bool enable = false;
};

template <typename T>
typename std::enable_if<EnableBitMaskOperators<T>::enable, T>::type
operator|(T lhs, T rhs) {
    using U = typename std::underlying_type<T>::type;
    return static_cast<T>(static_cast<U>(lhs) | static_cast<U>(rhs));
}

template <typename T>
typename std::enable_if<EnableBitMaskOperators<T>::enable, T>::type
operator&(T lhs, T rhs) {
    using U = typename std::underlying_type<T>::type;
    return static_cast<bool>(static_cast<U>(lhs) & static_cast<U>(rhs));
}