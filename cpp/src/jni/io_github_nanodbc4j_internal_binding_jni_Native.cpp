#include "jni/io_github_nanodbc4j_internal_binding_jni_Native.h"
#include "api/api.h"
#include "utils/logger.hpp"

JNIEXPORT jstring JNICALL Java_io_github_nanodbc4j_internal_binding_jni_Native_getUtf16String
(JNIEnv *env, jclass, jlong addr) {
    if (addr == 0) {
        return NULL;
    }

    static_assert(sizeof(ApiChar) == sizeof(jchar), "ApiChar must be 2 bytes for UTF-16 compatibility");

    try {
        const auto charPtr = reinterpret_cast<ApiChar *>(addr);
        const ApiString result(charPtr);
        return env->NewString(reinterpret_cast<const jchar *>(result.data()), static_cast<jsize>(result.length()));
    } catch (const std::exception &e) {
        const auto exClass = env->FindClass("java/lang/RuntimeException");
        env->ThrowNew(exClass, e.what());
    } catch (...) {
        const auto exClass = env->FindClass("java/lang/RuntimeException");
        env->ThrowNew(exClass, "Unknown exception");
    }
    return NULL;
}


JNIEXPORT void JNICALL Java_io_github_nanodbc4j_internal_binding_jni_Native_std_1free
(JNIEnv *env, jclass, jlong addr) {
    try {
        if (addr) {
            auto ptr = reinterpret_cast<void *>(addr);
            free(ptr);
            LOG_DEBUG("Memory freed successfully");
        }
    } catch (const std::exception &e) {
        const auto exClass = env->FindClass("java/lang/RuntimeException");
        env->ThrowNew(exClass, e.what());
    } catch (...) {
        const auto exClass = env->FindClass("java/lang/RuntimeException");
        env->ThrowNew(exClass, "Unknown exception");
    }
}
