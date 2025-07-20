#include "struct/error_info.h"
#include <cstring>
#include <cstdlib>
#include "utils/string_utils.h"

NativeError::NativeError() {
    error_code = 0;
    error_message = nullptr;
    error_type = nullptr;
}

NativeError::NativeError(const NativeError& other) {
    error_code = other.error_code;
    error_message = utils::duplicate_string(other.error_message);
    error_type = utils::duplicate_string(other.error_type);
}

NativeError::~NativeError() {
    if (error_message) {
        free(error_message);        
    }
    if (error_type) {
        free(error_type);        
    }

    error_message = nullptr;
    error_type = nullptr;
}

// очистить структуры ошибки
void clear_native_error(NativeError* error) {
    if (error) {
        error->error_code = 0;
        error->~NativeError();
    }
}

// Инициализация структуры ошибки
void init_error(NativeError* error) {
    clear_native_error(error);
}

// Установка ошибки
void set_error(NativeError* error, int code, const char* type, const char* message) {
    if (error) {
        clear_native_error(error);
        error->error_type = strdup(type);
        error->error_message = strdup(message);
    }
}