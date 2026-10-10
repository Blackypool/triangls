#ifndef DO_ANY_TYPE_ZERO_H
#define DO_ANY_TYPE_ZERO_H

#include "Header.h"


template <typename T>
constexpr void do_zero(T& val)
{
    if constexpr (requires { val.clear(); })  // constexpr for compile-time inlining
        val.clear();
    else
        val = T{};
}


template <typename Value>
std::optional<Value> flush_to_zero(Value dx) noexcept  // not constexpr supported
{
    if (std::abs(dx) < EPSILON_OF_ZERO)
        return std::nullopt;
                          
    return dx;
}


#endif // DO_ANY_TYPE_ZERO_H