#include "utils/path_utils.h"
#include <filesystem>

namespace fs = std::filesystem;

std::string utils::extract_filename(const char* path) {
    if (!path) return std::string();
    return fs::path(path).filename().string();
}

std::string utils::extract_filename(const std::string& path) {
    return fs::path(path).filename().string();
}