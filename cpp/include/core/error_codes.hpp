#pragma once

class ErrorCode {
public:
    /// \brief Enum representing error categories in the database API.
    enum class Value : int {
        Success = 0,   ///< No error
        Database = 1,   ///< Database-related error (e.g. nanodbc::database_error)
        Standard = 2,   ///< Standard C++ exception (e.g. std::exception, invalid argument, null pointer)
        Unknown = -1   ///< Unknown or unexpected error (e.g. catch (...))
    };

    constexpr ErrorCode(Value v) noexcept : value_(v) {}

    /// \brief Convert to 32-bit signed integer for C API compatibility.
    constexpr int to_int() const noexcept {
        return static_cast<int>(value_);
    }

    /// \brief Convert to 32-bit signed integer for C API compatibility.
    constexpr operator int() const noexcept {
        return static_cast<int>(value_);
    }

    /// \brief Convert from 32-bit signed integer (e.g. from legacy code or C layer).
    static constexpr ErrorCode from_int(int code) noexcept {
        switch (code) {
            case 0:  return ErrorCode(Value::Success);
            case 1:  return ErrorCode(Value::Database);
            case 2:  return ErrorCode(Value::Standard);
            case -1: return ErrorCode(Value::Unknown);
            default: return ErrorCode(Value::Unknown); // or Success? but safer to treat unknown as Unknown
        }
    }

    /// \brief Implicit conversion to enum value (for switch, comparisons, etc.)
    constexpr operator Value() const noexcept { return value_; }

    /// \brief Equality operators
    friend constexpr bool operator==(ErrorCode a, ErrorCode b) noexcept {
        return a.value_ == b.value_;
    }
    friend constexpr bool operator!=(ErrorCode a, ErrorCode b) noexcept {
        return !(a == b);
    }

    static const ErrorCode Success;
    static const ErrorCode Database;
    static const ErrorCode Standard;
    static const ErrorCode Unknown;
private:
    Value value_;
};

inline constexpr const ErrorCode ErrorCode::Success{ Value::Success };
inline constexpr const ErrorCode ErrorCode::Database{ Value::Database };
inline constexpr const ErrorCode ErrorCode::Standard{ Value::Standard };
inline constexpr const ErrorCode ErrorCode::Unknown{ Value::Unknown };