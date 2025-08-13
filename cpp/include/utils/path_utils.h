#pragma once
#include <string>

namespace utils {
    std::string extract_filename(const char* path);

    std::string extract_filename(const std::string& path);
}