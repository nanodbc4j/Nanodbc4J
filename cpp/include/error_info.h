#pragma once

#ifdef __cplusplus
extern "C" {
#endif

    struct NativeError {
        int error_code = 0;
        char error_message[256]{};
        char error_type[64]{};
    };

#ifdef __cplusplus
}
#endif


// Инициализация структуры ошибки
void init_error(NativeError* error);

// Установка ошибки
void set_error(NativeError* error, int code, const char* type, const char* message);