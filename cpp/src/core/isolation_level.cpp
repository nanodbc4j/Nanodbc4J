#include "core/isolation_level.hpp"

#ifdef _WIN32
// needs to be included above sql.h for windows
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <sqlext.h>
#include <stdexcept>

using namespace std;

// Map our enum to ODBC constants
int32_t IsolationLevel::to_odbc() const {
    switch (value_) {
        case Value::READ_UNCOMMITTED: return SQL_TXN_READ_UNCOMMITTED;
        case Value::READ_COMMITTED:   return SQL_TXN_READ_COMMITTED;
        case Value::REPEATABLE_READ:  return SQL_TXN_REPEATABLE_READ;
        case Value::SERIALIZABLE:     return SQL_TXN_SERIALIZABLE;
        default: throw runtime_error("Invalid isolation level");
    }
}

/// \brief Convert to integer for C API compatibility.
IsolationLevel::operator int() const {
    return to_odbc();
}

// Map ODBC constant back to our enum
IsolationLevel IsolationLevel::from_odbc(int32_t odbc_level) {
    switch (odbc_level) {
        case SQL_TXN_READ_UNCOMMITTED: return IsolationLevel(Value::READ_UNCOMMITTED);
        case SQL_TXN_READ_COMMITTED:   return IsolationLevel(Value::READ_COMMITTED);
        case SQL_TXN_REPEATABLE_READ:  return IsolationLevel(Value::REPEATABLE_READ);
        case SQL_TXN_SERIALIZABLE:     return IsolationLevel(Value::SERIALIZABLE);
        default: throw runtime_error("Unknown transaction isolation level returned by ODBC driver");
    }
}