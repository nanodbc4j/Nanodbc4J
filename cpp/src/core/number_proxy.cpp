#include "core/number_proxy.hpp"
#include <stdexcept>
#include <functional>

template<typename T>
template<typename U, typename>
NumberProxy<T>::NumberProxy(const std::string &str)
    : value(str.empty() ? NULL_NUMBER : str) {
}

template<typename T>
template<typename U, typename>
NumberProxy<T>::NumberProxy(const char *str)
    : value(str && *str ? str : NULL_NUMBER) {
}

template<typename T>
template<typename U, typename>
NumberProxy<T>::NumberProxy(T num)
    : value(std::move(num)) {
}

template<typename T>
NumberProxy<T>::operator int() const {
    if constexpr (std::is_same_v<T, std::string>) {
        try {
            return std::stoi(value);
        } catch (std::out_of_range&) {
            return std::numeric_limits<int>::max();
        }
    } else {
        return static_cast<int>(value);
    }
}

template<typename T>
NumberProxy<T>::operator long() const {
    if constexpr (std::is_same_v<T, std::string>) {
        try {
            return std::stol(value);
        } catch (std::out_of_range&) {
            return std::numeric_limits<long>::max();
        }
    } else {
        return static_cast<long>(value);
    }
}

template<typename T>
NumberProxy<T>::operator long long() const {
    if constexpr (std::is_same_v<T, std::string>) {
        try {
            return std::stoll(value);
        } catch (std::out_of_range&) {
            return std::numeric_limits<long long>::max();
        }
    } else {
        return static_cast<long long>(value);
    }
}

template<typename T>
NumberProxy<T>::operator unsigned int() const {
    if constexpr (std::is_same_v<T, std::string>) {
        try {
            return std::stoul(value);
        } catch (std::out_of_range&) {
            return std::numeric_limits<unsigned int>::max();
        }
    } else {
        return static_cast<unsigned int>(value);
    }
}

template<typename T>
NumberProxy<T>::operator unsigned long() const {
    if constexpr (std::is_same_v<T, std::string>) {
        try {
            return std::stoul(value);
        } catch (std::out_of_range&) {
            return std::numeric_limits<unsigned long>::max();
        }
    } else {
        return static_cast<unsigned long>(value);
    }
}

template<typename T>
NumberProxy<T>::operator unsigned long long() const {
    if constexpr (std::is_same_v<T, std::string>) {
        try {
            return std::stoull(value);
        } catch (std::out_of_range&) {
            return std::numeric_limits<unsigned long long>::max();
        }
    } else {
        return static_cast<unsigned long long>(value);
    }
}

template<typename T>
NumberProxy<T>::operator float() const {
    if constexpr (std::is_same_v<T, std::string>) {
        try {
            return std::stof(value);
        } catch (std::out_of_range&) {
            return std::numeric_limits<float>::max();
        }
    } else {
        return static_cast<float>(value);
    }
}

template<typename T>
NumberProxy<T>::operator double() const {
    if constexpr (std::is_same_v<T, std::string>) {
        try {
            return std::stod(value);
        } catch (std::out_of_range&) {
            return std::numeric_limits<double>::max();
        }
    } else {
        return static_cast<double>(value);
    }
}

template<typename T>
NumberProxy<T>::operator long double() const {
    if constexpr (std::is_same_v<T, std::string>) {
        try {
            return std::stold(value);
        } catch (std::out_of_range&) {
            return std::numeric_limits<long double>::max();
        }
    } else {
        return static_cast<long double>(value);
    }
}

template<typename T>
NumberProxy<T>::operator bool() const {
    if constexpr (std::is_same_v<T, std::string>) {
        return !value.empty() && value != "0" && value != "false";
    } else {
        return static_cast<bool>(value);
    }
}

template<typename T>
NumberProxy<T>::operator std::string() const {
    if constexpr (std::is_same_v<T, std::string>) {
        return value;
    } else {
        if constexpr (std::is_same_v<T, bool>) {
            return value ? "true" : "false";
        } else {
            return std::to_string(value);
        }
    }
}

template class NumberProxy<int>;
template class NumberProxy<long>;
template class NumberProxy<long long>;
template class NumberProxy<unsigned int>;
template class NumberProxy<unsigned long>;
template class NumberProxy<unsigned long long>;
template class NumberProxy<float>;
template class NumberProxy<double>;
template class NumberProxy<long double>;
template class NumberProxy<bool>;
template class NumberProxy<std::string>;