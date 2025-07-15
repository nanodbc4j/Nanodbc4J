#include "error_info.h"
#include <cstring>


// Инициализация структуры ошибки
void init_error(NativeError* error) {
    if (error) {
        error->error_code = 0;
        memset(error->error_message, 0, sizeof(error->error_message));
        memset(error->error_type, 0, sizeof(error->error_type));
    }
}

// Установка ошибки
void set_error(NativeError* error, int code, const char* type, const char* message) {
    if (error) {
        error->error_code = code;
        strncpy(error->error_type, type, sizeof(error->error_type) - 1);
        strncpy(error->error_message, message, sizeof(error->error_message) - 1);
        error->error_type[sizeof(error->error_type) - 1] = '\0';
        error->error_message[sizeof(error->error_message) - 1] = '\0';
    }
}