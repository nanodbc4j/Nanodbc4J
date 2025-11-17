#pragma once
#include <nanodbc/nanodbc.h>
#include "core/number_proxy.hpp"
#include "core/string_proxy.hpp"
#include "utils/string_utils.hpp"

class ResultSet : public nanodbc::result {
public:
    /// \brief Empty result set.
    ResultSet() = default;

    /// \brief Copy constructor.
    explicit ResultSet(const result& rhs);

    /// \brief Move constructor.
    explicit ResultSet(result&& rhs) noexcept;

    ResultSet(nanodbc::statement&& statement, long rowset_size);

    /// \brief Gets data from the given column of the current rowset.
    ///
    /// Columns are numbered from left to right and 0-indexed.
    /// \param column position.
    /// \throws database_error
    /// \throws index_range_error
    /// \throws type_incompatible_error
    /// \throws null_access_error
    template <class T>
    T get(short column) const {
        if constexpr (std::is_arithmetic_v<T>) {
            return getArithmetic<T>(column);
        }
        if constexpr (nanodbc::is_string<T>::value){
            return getString<T>(column);
        }
        return result::get<T>(column);
    }

    /// \brief Gets data from the given column of the current rowset.
    ///
    /// If the data is null, fallback is returned instead.
    ///
    /// Columns are numbered from left to right and 0-indexed.
    /// \param column position.
    /// \param fallback if value is null, return fallback instead.
    /// \throws database_error
    /// \throws index_range_error
    /// \throws type_incompatible_error
    template <class T>
    T get(short column, T const& fallback) const {
        if constexpr (std::is_arithmetic_v<T>) {
            return getArithmetic<T>(column, fallback);
        }
        if constexpr (nanodbc::is_string<T>::value){
            return getString<T>(column, fallback);
        }
        return result::get<T>(column, fallback);
    }

    /// \brief Gets data from the given column by name of the current rowset.
    ///
    /// \param column_name column's name.
    /// \throws database_error
    /// \throws index_range_error
    /// \throws type_incompatible_error
    /// \throws null_access_error
    template <class T>
    T get(nanodbc::string const& column_name) const {
        if constexpr (std::is_arithmetic_v<T>) {
            return getArithmetic<T>(column_name);
        }
        if constexpr (nanodbc::is_string<T>::value){
            return getString<T>(column_name);
        }
        return result::get<T>(column_name);
    }

    /// \brief Gets data from the given column by name of the current rowset.
    ///
    /// If the data is null, fallback is returned instead.
    ///
    /// \param column_name column's name.
    /// \param fallback if value is null, return fallback instead.
    /// \throws database_error
    /// \throws index_range_error
    /// \throws type_incompatible_error
    template <class T>
    T get(nanodbc::string const& column_name, T const& fallback) const {
        if constexpr (std::is_arithmetic_v<T>) {
            return getArithmetic<T>(column_name, fallback);
        }
        if constexpr (nanodbc::is_string<T>::value){
            return getString<T>(column_name, fallback);
        }
        return result::get<T>(column_name, fallback);
    }

private:
    template <typename T>
    T getArithmetic(short column) const {
        if (is_string_or_binary(column)) {
            const auto str_value = result::get<std::string>(column);
            NumberProxy number_proxy(str_value);
            return static_cast<T> (number_proxy);
        }
        return result::get<T>(column);
    }

    template <class T>
    T getArithmetic(short column, T const& fallback) const {
        if (is_string_or_binary(column)) {
            NumberProxy fallback_value(fallback);
            const auto str_value = result::get<std::string>(column, static_cast<std::string> (fallback_value));
            NumberProxy number_proxy(str_value);
            return static_cast<T> (number_proxy);
        }
        return result::get<T>(column, fallback);
    }

    template <class T>
    T getArithmetic(nanodbc::string const& column_name) const {
        if (is_string_or_binary(column_name)) {
            const auto str_value = result::get<std::string>(column_name);
            NumberProxy number_proxy(str_value);
            return static_cast<T> (number_proxy);
        }
        return result::get<T>(column_name);
    }

    template <class T>
    T getArithmetic(nanodbc::string const& column_name, T const& fallback) const {
        if (is_string_or_binary(column_name)) {
            NumberProxy fallback_value(fallback);
            const auto str_value = result::get<std::string>(column_name, static_cast<std::string> (fallback_value));
            NumberProxy number_proxy(str_value);
            return static_cast<T> (number_proxy);
        }
        return result::get<T>(column_name, fallback);
    }

    template <typename T>
    T getString(short column) const {
        if constexpr (std::is_same_v<T, std::string>) {
            return result::get<T>(column);
        }

        if (is_string_or_binary(column)) {
            const auto str_value = result::get<std::string>(column);
#ifdef _WIN32
            // On Windows, ODBC drivers typically return SQL_C_CHAR data in the system's active ANSI code page (e.g., CP1251 for Russian locales)
            StringProxy string_proxy(utils::ansi_to_wstring(str_value));
#else
            StringProxy string_proxy(str_value);
#endif
            return static_cast<T> (string_proxy);
        }
        return result::get<T>(column);
    }

    template <class T>
    T getString(short column, T const& fallback) const {
        if constexpr (std::is_same_v<T, std::string>) {
            return result::get<T>(column, fallback);
        }

        if (is_string_or_binary(column)) {
            StringProxy fallback_value(fallback);
            const auto str_value = result::get<std::string>(column, static_cast<std::string> (fallback_value));
#ifdef _WIN32
            // On Windows, ODBC drivers typically return SQL_C_CHAR data in the system's active ANSI code page (e.g., CP1251 for Russian locales)
            StringProxy string_proxy(utils::ansi_to_wstring(str_value));
#else
            StringProxy string_proxy(str_value);
#endif
            return static_cast<T> (string_proxy);
        }
        return result::get<T>(column, fallback);
    }

    template <class T>
    T getString(nanodbc::string const& column_name) const {
        if constexpr (std::is_same_v<T, std::string>) {
            return result::get<T>(column_name);
        }

        if (is_string_or_binary(column_name)) {
            const auto str_value = result::get<std::string>(column_name);
#ifdef _WIN32
            // On Windows, ODBC drivers typically return SQL_C_CHAR data in the system's active ANSI code page (e.g., CP1251 for Russian locales)
            StringProxy string_proxy(utils::ansi_to_wstring(str_value));
#else
            StringProxy string_proxy(str_value);
#endif
            return static_cast<T> (string_proxy);
        }
        return result::get<T>(column_name);
    }

    template <class T>
    T getString(nanodbc::string const& column_name, T const& fallback) const {
        if constexpr (std::is_same_v<T, std::string>) {
            return result::get<T>(column_name, fallback);
        }

        if (is_string_or_binary(column_name)) {
            StringProxy fallback_value(fallback);
            const auto str_value = result::get<std::string>(column_name, static_cast<std::string> (fallback_value));
#ifdef _WIN32
            // On Windows, ODBC drivers typically return SQL_C_CHAR data in the system's active ANSI code page (e.g., CP1251 for Russian locales)
            StringProxy string_proxy(utils::ansi_to_wstring(str_value));
#else
            StringProxy string_proxy(str_value);
#endif
            return static_cast<T> (string_proxy);
        }
        return result::get<T>(column_name, fallback);
    }

    bool is_string_or_binary(short column) const;

    bool is_string_or_binary(const nanodbc::string& column_name) const;
};