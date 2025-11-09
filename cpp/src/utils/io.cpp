#include "utils/io.hpp"
#include "utils/string_utils.hpp"

std::wostream& operator<<(std::wostream& os, const std::string& str) {
    os << utils::to_wstring(str);
    return os;
}

std::wostream& operator<<(std::wostream& os, const std::u16string& str) {
    os << utils::to_wstring(str);
    return os;
}

std::ostream& operator<<(std::ostream& os, const std::wstring& str) {
    os << utils::to_string(str);
    return os;
}

std::ostream& operator<<(std::ostream& os, const std::u16string& str) {
    os << utils::to_string(str);
    return os;
}

std::ostream& operator<<(std::ostream& os, const std::u32string& str) {
    os << utils::to_string(str);
    return os;
}