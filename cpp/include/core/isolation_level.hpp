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

    /// \brief Convert to integer for C API compatibility.
    operator int() const;

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

    static const IsolationLevel READ_UNCOMMITTED;
    static const IsolationLevel READ_COMMITTED;
    static const IsolationLevel REPEATABLE_READ;
    static const IsolationLevel SERIALIZABLE;

private:
    Value value_;
};

inline constexpr const IsolationLevel IsolationLevel::READ_UNCOMMITTED{ Value::READ_UNCOMMITTED };
inline constexpr const IsolationLevel IsolationLevel::READ_COMMITTED{ Value::READ_COMMITTED };
inline constexpr const IsolationLevel IsolationLevel::REPEATABLE_READ{ Value::REPEATABLE_READ };
inline constexpr const IsolationLevel IsolationLevel::SERIALIZABLE{ Value::SERIALIZABLE };