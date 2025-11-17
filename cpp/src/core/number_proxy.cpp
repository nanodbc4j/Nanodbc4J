#include "core/number_proxy.hpp"
#include <stdexcept>
#include <limits>
#include "utils/strhash.hpp"

using namespace std;

template<typename T>
NumberProxy<T>::NumberProxy(T num)
    : value(std::move(num)) {
}

template<typename T>
NumberProxy<T>::operator short() const {
    return static_cast<short>(value);
}

template<typename T>
NumberProxy<T>::operator int() const {
    return static_cast<int>(value);
}

template<typename T>
NumberProxy<T>::operator long() const {
    return static_cast<long>(value);
}

template<typename T>
NumberProxy<T>::operator long long() const {
    return static_cast<long long>(value);
}

template<typename T>
NumberProxy<T>::operator unsigned short() const {
    return static_cast<unsigned short>(value);
}

template<typename T>
NumberProxy<T>::operator unsigned int() const {
    return static_cast<unsigned int>(value);
}

template<typename T>
NumberProxy<T>::operator unsigned long() const {
    return static_cast<unsigned long>(value);
}

template<typename T>
NumberProxy<T>::operator unsigned long long() const {
    return static_cast<unsigned long long>(value);
}

template<typename T>
NumberProxy<T>::operator float() const {
    return static_cast<float>(value);
}

template<typename T>
NumberProxy<T>::operator double() const {
    return static_cast<double>(value);
}

template<typename T>
NumberProxy<T>::operator long double() const {
    return static_cast<long double>(value);
}

template<typename T>
NumberProxy<T>::operator bool() const {
    return static_cast<bool>(value);
}

template<typename T>
NumberProxy<T>::operator string() const {
    return to_string(value);
}

NumberProxy<string>::NumberProxy(const string& str)
    : value(str.empty() ? NULL_NUMBER : str) {
    hash = hash_djb2a(value);
    switch (hash) {
        case "false"_sh:
            value = "0";
            hash = "0"_sh;
            break;
        case "true"_sh:
            value = "1";
            hash = "1"_sh;
            break;
        default:
            break;
    }
}


NumberProxy<string>::NumberProxy(const char* str)
    : value(str && *str ? str : NULL_NUMBER) {
    hash = hash_djb2a(value);
    switch (hash) {
        case "false"_sh:
            value = "0";
            hash = "0"_sh;
            break;
        case "true"_sh:
            value = "1";
            hash = "1"_sh;
            break;
        default:
            break;
    }
}

NumberProxy<string>::operator short() const {
    try {
        return stoi(value);
    } catch (out_of_range&) {
        return numeric_limits<short>::max();
    }
}

NumberProxy<string>::operator int() const {
    try {
        return stoi(value);
    } catch (out_of_range&) {
        return numeric_limits<int>::max();
    }
}

NumberProxy<string>::operator long() const {
    try {
        return stol(value);
    } catch (out_of_range&) {
        return numeric_limits<long>::max();
    }
}

NumberProxy<string>::operator long long() const {
    try {
        return stoll(value);
    } catch (out_of_range&) {
        return numeric_limits<long long>::max();
    }
}

NumberProxy<string>::operator unsigned short() const {
    try {
        return stoul(value);
    } catch (out_of_range&) {
        return numeric_limits<unsigned short>::max();
    }
}

NumberProxy<string>::operator unsigned int() const {
    try {
        return stoul(value);
    } catch (out_of_range&) {
        return numeric_limits<unsigned int>::max();
    }
}

NumberProxy<string>::operator unsigned long() const {
    try {
        return stoul(value);
    } catch (out_of_range&) {
        return numeric_limits<unsigned long>::max();
    }
}

NumberProxy<string>::operator unsigned long long() const {
    try {
        return stoull(value);
    } catch (out_of_range&) {
        return numeric_limits<unsigned long long>::max();
    }
}

NumberProxy<string>::operator float() const {
    try {
        return stof(value);
    } catch (out_of_range&) {
        return numeric_limits<float>::max();
    }
}

NumberProxy<string>::operator double() const {
    try {
        return stod(value);
    } catch (out_of_range&) {
        return numeric_limits<double>::max();
    }
}

NumberProxy<string>::operator long double() const {
    try {
        return stold(value);
    } catch (out_of_range&) {
        return numeric_limits<long double>::max();
    }
}

NumberProxy<string>::operator bool() const {
    return !value.empty() && hash != "0"_sh && hash != "false"_sh;
}

NumberProxy<string>::operator string() const {
    return value;
}

template class NumberProxy<short>;
template class NumberProxy<int>;
template class NumberProxy<long>;
template class NumberProxy<long long>;
template class NumberProxy<unsigned short>;
template class NumberProxy<unsigned int>;
template class NumberProxy<unsigned long>;
template class NumberProxy<unsigned long long>;
template class NumberProxy<float>;
template class NumberProxy<double>;
template class NumberProxy<long double>;
template class NumberProxy<bool>;