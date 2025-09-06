#pragma once

#ifdef __cplusplus
extern "C" {
#endif

    struct NativeError {
        int error_code;
        char* error_message;
        char* error_type;

        NativeError();
        explicit NativeError(const NativeError& other);
        ~NativeError();
    };

    // очистить структуры ошибки
    void clear_native_error(NativeError* error);

#ifdef __cplusplus
}
#endif


// Инициализация структуры ошибки
void init_error(NativeError* error);

// Установка ошибки
void set_error(NativeError* error, int code, const char* type, const char* message);