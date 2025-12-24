#include "struct/error_info.h"
#include <cstring>
#include <cstdlib>
#include "utils/string_utils.hpp"
#include "utils/logger.hpp"

void NativeError::clear(NativeError* error) {
    LOG_TRACE("clear_native_error: error={}", reinterpret_cast<uintptr_t>(error));
    if (error) {
        error->status = EXIT_SUCCESS;
        if (error->error_message) {
            free(error->error_message);
            error->error_message = nullptr;
        }
        LOG_TRACE("NativeError cleared and destructed");
    }
}

NativeError::NativeError(const NativeError& other) {
    LOG_TRACE("Copying NativeError from {}", reinterpret_cast<uintptr_t>(&other));
    status = other.status;
    error_message = utils::duplicate_string(other.error_message);
}

NativeError::~NativeError() {
    if (error_message) {
        free(error_message);        
    }
}

// Initialize error structure
void init_error(NativeError* error) {
    LOG_TRACE("init_error: error={}", reinterpret_cast<uintptr_t>(error));
    NativeError::clear(error);
    LOG_TRACE("NativeError initialized");
}

// Set error
void set_error(NativeError* error, const char* message) {
    LOG_TRACE("set_error: error={}, message='{}'",
        reinterpret_cast<uintptr_t>(error),
        message ? message : "(null)");

    try {
        if (error) {
            NativeError::clear(error);
            error->error_message = strdup(message);
            error->status = EXIT_FAILURE;
            return;
        }
    } catch (...) {
    }

    LOG_TRACE("set_error: error is null, nothing to set");
}