#pragma once
#include <string>

template<typename T>
class NumberProxy {
    const T value;

public:

    explicit NumberProxy(T num);

    explicit operator short() const;

    explicit operator int() const;

    explicit operator long() const;

    explicit operator long long() const;

    explicit operator unsigned short() const;

    explicit operator unsigned int() const;

    explicit operator unsigned long() const;

    explicit operator unsigned long long() const;

    explicit operator float() const;

    explicit operator double() const;

    explicit operator long double() const;

    explicit operator bool() const;

    explicit operator std::string() const;
};

template<>
class NumberProxy<std::string> {
    static constexpr std::string_view NULL_NUMBER = "0";

    std::string value;
    unsigned long hash;

public:
    explicit NumberProxy(std::string&& str);

    explicit NumberProxy(const std::string& str);

    explicit NumberProxy(const char* str);

    explicit operator short() const;

    explicit operator int() const;

    explicit operator long() const;

    explicit operator long long() const;

    explicit operator unsigned short() const;

    explicit operator unsigned int() const;

    explicit operator unsigned long() const;

    explicit operator unsigned long long() const;

    explicit operator float() const;

    explicit operator double() const;

    explicit operator long double() const;

    explicit operator bool() const;

    explicit operator std::string() const;
};

template<>
class NumberProxy<std::wstring> {
    static constexpr std::wstring_view NULL_NUMBER = L"0";

    std::wstring value;
    unsigned long hash;

public:
    explicit NumberProxy(std::wstring&& str);

    explicit NumberProxy(const std::wstring& str);

    explicit NumberProxy(const wchar_t* str);

    explicit operator short() const;

    explicit operator int() const;

    explicit operator long() const;

    explicit operator long long() const;

    explicit operator unsigned short() const;

    explicit operator unsigned int() const;

    explicit operator unsigned long() const;

    explicit operator unsigned long long() const;

    explicit operator float() const;

    explicit operator double() const;

    explicit operator long double() const;

    explicit operator bool() const;

    explicit operator std::wstring() const;
};

extern template class NumberProxy<short>;
extern template class NumberProxy<int>;
extern template class NumberProxy<long>;
extern template class NumberProxy<long long>;
extern template class NumberProxy<unsigned short>;
extern template class NumberProxy<unsigned int>;
extern template class NumberProxy<unsigned long>;
extern template class NumberProxy<unsigned long long>;
extern template class NumberProxy<float>;
extern template class NumberProxy<double>;
extern template class NumberProxy<long double>;
extern template class NumberProxy<bool>;
