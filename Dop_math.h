#ifndef DO_ANY_TYPE_ZERO_H
#define DO_ANY_TYPE_ZERO_H

#include "Header.h"


template <typename T>
constexpr void do_zero(T& val) noexcept  // maybe cant be noexcept because .clear
{
    if constexpr (requires { val.clear(); })  // constexpr for compile-time inlining
        val.clear();
    else
        val = T{};
}

template <typename Value>
constexpr bool is_nearly_zero(const Value& dx, const Value& eps = EPSILON_OF_ZERO) noexcept
{
    return (dx > -eps) && (dx < eps);
}


#endif // DO_ANY_TYPE_ZERO_H