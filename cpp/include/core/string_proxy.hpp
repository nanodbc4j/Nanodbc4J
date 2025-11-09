#pragma once
#include <string>

template <typename CharT,
          typename Traits = std::char_traits<CharT>,
          typename Alloc = std::allocator<CharT>>
class StringProxy {
    std::basic_string<CharT, Traits, Alloc> data;

    public:
    using StringT = std::basic_string<CharT, Traits, Alloc>;

    explicit StringProxy(const CharT* str);

    explicit StringProxy(StringT& str);

    explicit StringProxy(StringT&& str);

    explicit StringProxy(CharT ch);

    explicit operator std::string() const;

    explicit operator std::wstring() const;

    explicit operator std::u16string() const;

    const StringT& get() const;

    const CharT* c_str() const;

    size_t length() const;

    bool empty() const;
};

template<typename CharT, typename Traits, typename Alloc>
std::ostream& operator<<(std::ostream& os, const StringProxy<CharT, Traits, Alloc> & proxy);

template<typename CharT, typename Traits, typename Alloc>
std::wostream& operator<<(std::wostream& os, const StringProxy<CharT, Traits, Alloc> & proxy);

extern template class StringProxy<char>;
extern template class StringProxy<wchar_t>;
extern template class StringProxy<char16_t>;