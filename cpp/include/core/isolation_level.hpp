#pragma once
#include <cstdint> 

class IsolationLevel {
public:
    /// \brief Enum representing standard ODBC transaction isolation levels.
    /// Values are mapped internally to SQL_TXN_* constants from ODBC.
    enum class Value {
        READ_UNCOMMITTED,
        READ_COMMITTED,
        REPEATABLE_READ,
        SERIALIZABLE
    };

    constexpr IsolationLevel(Value v) : value_(v) {}

    /// \brief Convert to ODBC constant (32-bit signed integer).
    std::int32_t to_odbc() const;

    /// \brief Convert from ODBC constant.
    static IsolationLevel from_odbc(std::int32_t odbc_level);

    /// \brief Implicit conversion to enum value (for switch, comparisons, etc.)
    constexpr operator Value() const { return value_; }

    /// \brief Equality operators
    friend constexpr bool operator==(IsolationLevel a, IsolationLevel b) {
        return a.value_ == b.value_;
    }
    friend constexpr bool operator!=(IsolationLevel a, IsolationLevel b) {
        return !(a == b);
    }

private:
    Value value_;
};