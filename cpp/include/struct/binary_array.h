#pragma once
#include <stdint.h>
#include <vector>

#ifdef __cplusplus
extern "C" {
#endif

    struct BinaryArray {
        int8_t* data = nullptr;
        int32_t length = 0;

        BinaryArray() = default;

        BinaryArray(const uint8_t* data_ptr, int32_t data_length);

        BinaryArray(const std::vector<uint8_t>& vec);

        ~BinaryArray();

        BinaryArray(const BinaryArray&) = delete;

        BinaryArray& operator=(const BinaryArray&) = delete;

        BinaryArray(BinaryArray&& other) noexcept;

        BinaryArray& operator=(BinaryArray&& other) noexcept;

        std::vector<uint8_t> to_vector() const;

        bool is_null_data() const;
    };

#ifdef __cplusplus
} // extern "C"
#endif