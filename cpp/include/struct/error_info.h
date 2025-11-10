#pragma once
#include <core/error_codes.hpp>

#ifdef __cplusplus
extern "C" {
#endif

    struct NativeError {
        int error_code = ErrorCode::Success;
        char* error_message = nullptr;
        char* error_type = nullptr;

        NativeError() = default;
        NativeError(const NativeError& other);
        ~NativeError();
        static void clear(NativeError* error);
    };

#ifdef __cplusplus
}
#endif


// Инициализация структуры ошибки
void init_error(NativeError* error);

// Установка ошибки
void set_error(NativeError* error, ErrorCode code, const char* type, const char* message);