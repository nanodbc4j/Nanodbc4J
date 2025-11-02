#pragma once
#include <nanodbc/nanodbc.h>
#include "struct/error_info.h"
#include "struct/nanodbc_c.h"
#include "api/api.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    /// \brief Retrieves list of available ODBC drivers.
    /// \param count Output parameter for number of drivers returned.
    /// \return Array of Driver pointers, nullptr on failure.
    ODBC_API Driver** drivers_list(int* count) noexcept;

    /// \brief Retrieves list of available ODBC data sources.
    /// \param count Output parameter for number of data sources returned.
    /// \return Array of Datasource pointers, nullptr on failure.
    ODBC_API Datasource** datasources_list(int* count) noexcept;

    /// \brief Releases memory allocated by standard library functions.
    /// \param ptr Pointer to memory block to free.
    ODBC_API void std_free(void* ptr) noexcept;

    /// \brief Clears and resets the native error structure.
    /// \param error Pointer to NativeError structure to clear.
    ODBC_API void clear_native_error(NativeError* error) noexcept;

    /// \brief Sets the logging level for ODBC operations.
    /// \param level The logging level to set.
    /// \return Previous logging level.
    ODBC_API int set_log_level(int level) noexcept;

    /// \brief Releases a single datasource object.
    /// \param datasource Pointer to Datasource object to delete.
    ODBC_API void delete_datasource(Datasource* datasource) noexcept;

    /// \brief Releases an array of datasource objects.
    /// \param datasource_array Array of Datasource pointers to delete.
    /// \param size Number of elements in the array.
    ODBC_API void delete_datasource_array(Datasource** datasource_array, int size) noexcept;

    /// \brief Releases a single driver object.
    /// \param driver Pointer to Driver object to delete.
    ODBC_API void delete_driver(Driver* driver) noexcept;

    /// \brief Releases an array of driver objects.
    /// \param driver_array Array of Driver pointers to delete.
    /// \param size Number of elements in the array.
    ODBC_API void delete_driver_array(Driver** driver_array, int size) noexcept;

#ifdef __cplusplus
} // extern "C"
#endif
