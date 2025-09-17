#include "core/isolation_level.hpp"
#include <wtypes.h>
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