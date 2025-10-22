#include "struct/binary_array.h"
#include <cstring>

BinaryArray::BinaryArray(const uint8_t* data_ptr, int32_t data_length) {
    if (data_ptr != nullptr && data_length > 0) {
        length = data_length;
        data = new int8_t[length];
        std::memcpy(data, data_ptr, length);
    }
}

BinaryArray::BinaryArray(const std::vector<uint8_t>& vec) {
    if (!vec.empty()) {
        length = static_cast<int32_t>(vec.size());
        data = new int8_t[length];
        std::memcpy(data, vec.data(), length);
    }
}

BinaryArray::~BinaryArray() {
    if (data != nullptr) {
        delete[] data;
        data = nullptr;
    }
    length = 0;
}



BinaryArray::BinaryArray(BinaryArray&& other) noexcept
    : data(other.data), length(other.length) {
    other.data = nullptr;
    other.length = 0;
}

BinaryArray& BinaryArray::operator=(BinaryArray&& other) noexcept {
    if (this != &other) {
        if (data != nullptr) {
            delete[] data;
        }
        data = other.data;
        length = other.length;
        other.data = nullptr;
        other.length = 0;
    }
    return *this;
}

std::vector<uint8_t> BinaryArray::to_vector() const {
    if (data == nullptr || length == 0) {
        return {};
    }
    std::vector<uint8_t> result(length);
    std::memcpy(result.data(), data, length);
    return result;
}

bool BinaryArray::is_null_data() const {
    return data == nullptr;
}