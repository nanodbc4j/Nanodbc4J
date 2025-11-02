#pragma once
#include <nanodbc/nanodbc.h>
#include "struct/error_info.h"
#include "struct/nanodbc_c.h"
#include "struct/binary_array.h"
#include "api/api.h"

#ifdef __cplusplus
extern "C" {
#endif

    /// \brief Prepares a SQL statement for execution with parameters.
    /// \param stmt Pointer to the statement object.
    /// \param sql The SQL statement to prepare.
    /// \param error Error information structure to populate on failure.
    ODBC_API void prepare_statement(nanodbc::statement* stmt, const ApiChar* sql, NativeError* error) noexcept;

    /// \brief Binds an integer value to a parameter in the prepared statement.
    /// \param stmt Pointer to the statement object.
    /// \param index Zero-based parameter index.
    /// \param value Integer value to bind.
    /// \param error Error information structure to populate on failure.
    ODBC_API void set_int_value(nanodbc::statement* stmt, int index, int value, NativeError* error) noexcept;

    /// \brief Binds a long value to a parameter in the prepared statement.
    /// \param stmt Pointer to the statement object.
    /// \param index Zero-based parameter index.
    /// \param value Long value to bind.
    /// \param error Error information structure to populate on failure.
    ODBC_API void set_long_value(nanodbc::statement* stmt, int index, long value, NativeError* error) noexcept;

    /// \brief Binds a double value to a parameter in the prepared statement.
    /// \param stmt Pointer to the statement object.
    /// \param index Zero-based parameter index.
    /// \param value Double value to bind.
    /// \param error Error information structure to populate on failure.
    ODBC_API void set_double_value(nanodbc::statement* stmt, int index, double value, NativeError* error) noexcept;

    /// \brief Binds a boolean value to a parameter in the prepared statement.
    /// \param stmt Pointer to the statement object.
    /// \param index Zero-based parameter index.
    /// \param value Boolean value to bind.
    /// \param error Error information structure to populate on failure.
    ODBC_API void set_bool_value(nanodbc::statement* stmt, int index, bool value, NativeError* error) noexcept;

    /// \brief Binds a float value to a parameter in the prepared statement.
    /// \param stmt Pointer to the statement object.
    /// \param index Zero-based parameter index.
    /// \param value Float value to bind.
    /// \param error Error information structure to populate on failure.
    ODBC_API void set_float_value(nanodbc::statement* stmt, int index, float value, NativeError* error) noexcept;

    /// \brief Binds a short value to a parameter in the prepared statement.
    /// \param stmt Pointer to the statement object.
    /// \param index Zero-based parameter index.
    /// \param value Short value to bind.
    /// \param error Error information structure to populate on failure.
    ODBC_API void set_short_value(nanodbc::statement* stmt, int index, short value, NativeError* error) noexcept;

    /// \brief Binds a string value to a parameter in the prepared statement.
    /// \param stmt Pointer to the statement object.
    /// \param index Zero-based parameter index.
    /// \param value String value to bind.
    /// \param error Error information structure to populate on failure.
    ODBC_API void set_string_value(nanodbc::statement* stmt, int index, const ApiChar* value, NativeError* error) noexcept;

    /// \brief Binds a date value to a parameter in the prepared statement.
    /// \param stmt Pointer to the statement object.
    /// \param index Zero-based parameter index.
    /// \param value Date value to bind.
    /// \param error Error information structure to populate on failure.
    ODBC_API void set_date_value(nanodbc::statement* stmt, int index, CDate* value, NativeError* error) noexcept;

    /// \brief Binds a time value to a parameter in the prepared statement.
    /// \param stmt Pointer to the statement object.
    /// \param index Zero-based parameter index.
    /// \param value Time value to bind.
    /// \param error Error information structure to populate on failure.
    ODBC_API void set_time_value(nanodbc::statement* stmt, int index, CTime* value, NativeError* error) noexcept;

    /// \brief Binds a timestamp value to a parameter in the prepared statement.
    /// \param stmt Pointer to the statement object.
    /// \param index Zero-based parameter index.
    /// \param value Timestamp value to bind.
    /// \param error Error information structure to populate on failure.
    ODBC_API void set_timestamp_value(nanodbc::statement* stmt, int index, CTimestamp* value, NativeError* error) noexcept;

    /// \brief Binds a binary array value to a parameter in the prepared statement.
    /// \param stmt Pointer to the statement object.
    /// \param index Zero-based parameter index.
    /// \param value Binary array value to bind.
    /// \param error Error information structure to populate on failure.
    ODBC_API void set_binary_array_value(nanodbc::statement* stmt, int index, BinaryArray* value, NativeError* error) noexcept;

    /// \brief Executes the prepared statement with bound parameters.
    /// \param stmt Pointer to the statement object.
    /// \param timeout Seconds before execution timeout.
    /// \param error Error information structure to populate on failure.
    /// \return Pointer to result set object on success, nullptr on failure.
    ODBC_API nanodbc::result* execute(nanodbc::statement* stmt, int timeout, NativeError* error) noexcept;

    /// \brief Cancels the current statement execution.
    /// \param stmt Pointer to the statement object.
    /// \param error Error information structure to populate on failure.
    ODBC_API void cancel_statement(nanodbc::statement* stmt, NativeError* error) noexcept;

    /// \brief Closes and releases statement resources.
    /// \param stmt Pointer to the statement object.
    /// \param error Error information structure to populate on failure.
    ODBC_API void close_statement(nanodbc::statement* stmt, NativeError* error) noexcept;

#ifdef __cplusplus
} // extern "C"
#endif