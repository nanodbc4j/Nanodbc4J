#include "utils/path_utils.h"

std::string_view utils::extract_filename(const char* path) {
    std::string_view sv(path);  // constexpr-конструктор
    auto pos = sv.find_last_of("/\\");  // find_last_of тоже constexpr
    return (pos == std::string_view::npos) ? sv : sv.substr(pos + 1);
}
