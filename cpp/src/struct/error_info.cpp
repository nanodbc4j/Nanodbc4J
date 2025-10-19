#include "struct/error_info.h"
#include <cstring>
#include <cstdlib>
#include "utils/string_utils.hpp"
#include "utils/logger.hpp"

void NativeError::clear(NativeError* error) {
    LOG_TRACE("clear_native_error: error={}", (void*)error);
    if (error) {
        error->error_code = 0;
        if (error->error_message) {
            free(error->error_message);
            error->error_message = nullptr;
        }
        if (error->error_type) {
            free(error->error_type);
            error->error_type = nullptr;
        }
        LOG_TRACE("NativeError cleared and destructed");
    }
}

NativeError::NativeError(const NativeError& other) {
    LOG_TRACE("Copying NativeError from {}", (void*)&other);
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
}

// Инициализация структуры ошибки
void init_error(NativeError* error) {
    LOG_TRACE("init_error: error={}", (void*)error);
    NativeError::clear(error);
    LOG_TRACE("NativeError initialized");
}

// Установка ошибки
void set_error(NativeError* error, ErrorCode code, const char* type, const char* message) {
    LOG_TRACE("set_error: error={}, code={}, type='{}', message='{}'",
        (void*)error,
        code.to_int(),
        type ? type : "(null)",
        message ? message : "(null)");

    if (error) {
        NativeError::clear(error);
        error->error_code = code;
        error->error_type = strdup(type);
        error->error_message = strdup(message);
        return;
    }
    LOG_TRACE("set_error: error is null, nothing to set");
}