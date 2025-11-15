#pragma once
#include <string>
#include <type_traits>

template<typename T>
class NumberProxy {
    inline static const std::string NULL_NUMBER = "0";

    T value;

public:
    template<typename U = T, typename = std::enable_if_t<std::is_same_v<std::decay_t<U>, std::string>>>
    explicit NumberProxy(const std::string &str);

    template<typename U = T, typename = std::enable_if_t<std::is_same_v<std::decay_t<U>, std::string>>>
    explicit NumberProxy(const char *str);

    template<typename U = T, typename = std::enable_if_t<std::is_arithmetic_v<U>>>
    explicit NumberProxy(T num);

    explicit operator int() const;

    explicit operator long() const;

    explicit operator long long() const;

    explicit operator unsigned int() const;

    explicit operator unsigned long() const;

    explicit operator unsigned long long() const;

    explicit operator float() const;

    explicit operator double() const;

    explicit operator long double() const;

    explicit operator bool() const;

    explicit operator std::string() const;
};

extern template class NumberProxy<int>;
extern template class NumberProxy<long>;
extern template class NumberProxy<long long>;
extern template class NumberProxy<unsigned int>;
extern template class NumberProxy<unsigned long>;
extern template class NumberProxy<unsigned long long>;
extern template class NumberProxy<float>;
extern template class NumberProxy<double>;
extern template class NumberProxy<long double>;
extern template class NumberProxy<bool>;
extern template class NumberProxy<std::string>;
