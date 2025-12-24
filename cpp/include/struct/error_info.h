#pragma once
#include <cstdlib>

#ifdef __cplusplus
extern "C" {
#endif

    struct NativeError {
        int status = EXIT_SUCCESS;
        char* error_message = nullptr;

        NativeError() = default;
        NativeError(const NativeError& other);
        ~NativeError();
        static void clear(NativeError* error);
    };

#ifdef __cplusplus
}
#endif


// Initialize error structure
void init_error(NativeError* error);

// Set error
void set_error(NativeError* error, const char* message);