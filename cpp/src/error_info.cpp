#include "error_info.h"
#include <cstring>
#include <cstdlib>


// Инициализация структуры ошибки
void init_error(NativeError* error) {
    if (error) {
        error->error_code = 0;

        if (error->error_message) {
            free(error->error_message);
            error->error_message = NULL;
        }

        if (error->error_type) {
            free(error->error_type);
            error->error_type = NULL;
        }
    }
}

// Установка ошибки
void set_error(NativeError* error, int code, const char* type, const char* message) {
    if (error) {
        error->error_code = code;

        // Освобождаем предыдущую память, если она была
        if (error->error_type) free(error->error_type);
        if (error->error_message) free(error->error_message);

        error->error_type = strdup(type);
        error->error_message = strdup(message);
    }
}