#include "core/string_proxy.hpp"
#include "utils/string_utils.hpp"

using namespace std;
using namespace utils;

template <typename CharT, typename Traits, typename Alloc>
StringProxy<CharT, Traits, Alloc>::StringProxy(const CharT* str)
    : data(str ? str : StringT{}) {
}

template <typename CharT, typename Traits, typename Alloc>
StringProxy<CharT, Traits, Alloc>::StringProxy(StringT& str)
    : data(str) {
}

template <typename CharT, typename Traits, typename Alloc>
StringProxy<CharT, Traits, Alloc>::StringProxy(StringT&& str)
    : data(move(str)) {
}

template <typename CharT, typename Traits, typename Alloc>
StringProxy<CharT, Traits, Alloc>::StringProxy(CharT ch)
    : data(1, ch) {
}

template <typename CharT, typename Traits, typename Alloc>
StringProxy<CharT, Traits, Alloc>::operator string() const {
    return to_string(data);
}

template <typename CharT, typename Traits, typename Alloc>
StringProxy<CharT, Traits, Alloc>::operator wstring() const {
    return to_wstring(data);
}

template <typename CharT, typename Traits, typename Alloc>
StringProxy<CharT, Traits, Alloc>::operator u16string() const {
    return to_u16string(data);
}

template <typename CharT, typename Traits, typename Alloc>
const typename StringProxy<CharT, Traits, Alloc>::StringT& StringProxy<CharT, Traits, Alloc>::get() const {
    return data;
}

template <typename CharT, typename Traits, typename Alloc>
const CharT* StringProxy<CharT, Traits, Alloc>::c_str() const {
    return data.c_str();
}

template <typename CharT, typename Traits, typename Alloc>
size_t StringProxy<CharT, Traits, Alloc>::length() const {
    return data.length();
}

template <typename CharT, typename Traits, typename Alloc>
bool StringProxy<CharT, Traits, Alloc>::empty() const {
    return data.empty();
}

template<typename CharT, typename Traits, typename Alloc>
ostream& operator<<(ostream& os, const StringProxy<CharT, Traits, Alloc> & proxy) {
    os <<  static_cast<string>(proxy);
    return os;
}

template<typename CharT, typename Traits, typename Alloc>
wostream& operator<<(wostream& os, const StringProxy<CharT, Traits, Alloc> & proxy) {
    os <<  static_cast<wstring>(proxy);
    return os;
}

template class StringProxy<char>;
template class StringProxy<wchar_t>;
template class StringProxy<char16_t>;