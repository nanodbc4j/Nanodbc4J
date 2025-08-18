#include "utils/io.hpp"
#include "utils/string_utils.hpp"

std::wostream& operator<<(std::wostream& os, const char16_t* str) {
    os << utils::to_wstring(str);
    return os;
}

std::wostream& operator<<(std::wostream& os, const std::string& str) {
    os << utils::to_wstring(str);
    return os;
}