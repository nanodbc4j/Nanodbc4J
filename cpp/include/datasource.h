#pragma once

#ifdef __cplusplus
extern "C" {
#endif
    struct datasource
    {
        const char16_t* name;   ///< DSN name.
        const char16_t* driver; ///< Driver description.
    };
#ifdef __cplusplus
}
#endif
