#include "core/result_set_meta_data.hpp"
#include <string>
#include <algorithm>
#include <locale>
#include <codecvt>
#include <sql.h>

#define BUFFER_SIZE 1024

// Проверка успешности выполнени¤ ODBC операции
inline static bool isOdbcSuccess(const SQLRETURN& ret) {
    return (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO);
}

// Вспомогательный метод для определения по имени типа
static const std::wstring determineClassNameByTypeName(int column, int sqlType, const std::wstring& typeName) {
    try {
        std::string lowerTypeName;
        lowerTypeName.reserve(typeName.size());

        // Преобразование wstring в string и в нижний регистр
        for (wchar_t c : typeName) {
            lowerTypeName += static_cast<char>(std::tolower(c));
        }

        if (lowerTypeName.find("bool") != std::string::npos ||
            lowerTypeName.find("bit") != std::string::npos) {
            return L"java.lang.Boolean";
        }
        else if (lowerTypeName.find("tinyint") != std::string::npos) {
            return L"java.lang.Byte";
        }
        else if (lowerTypeName.find("smallint") != std::string::npos) {
            return L"java.lang.Short";
        }
        else if (lowerTypeName.find("int") != std::string::npos ||
            lowerTypeName.find("integer") != std::string::npos) {
            return L"java.lang.Integer";
        }
        else if (lowerTypeName.find("bigint") != std::string::npos) {
            return L"java.lang.Long";
        }
        else if (lowerTypeName.find("float") != std::string::npos) {
            return L"java.lang.Float";
        }
        else if (lowerTypeName.find("real") != std::string::npos) {
            return L"java.lang.Float";
        }
        else if (lowerTypeName.find("double") != std::string::npos) {
            return L"java.lang.Double";
        }
        else if (lowerTypeName.find("decimal") != std::string::npos ||
            lowerTypeName.find("numeric") != std::string::npos) {
            return L"java.math.BigDecimal";
        }
        else if (lowerTypeName.find("date") != std::string::npos) {
            return L"java.sql.Date";
        }
        else if (lowerTypeName.find("time") != std::string::npos) {
            return L"java.sql.Time";
        }
        else if (lowerTypeName.find("timestamp") != std::string::npos ||
            lowerTypeName.find("datetime") != std::string::npos) {
            return L"java.sql.Timestamp";
        }
        else if (lowerTypeName.find("char") != std::string::npos ||
            lowerTypeName.find("varchar") != std::string::npos ||
            lowerTypeName.find("text") != std::string::npos ||
            lowerTypeName.find("string") != std::string::npos) {
            return L"java.lang.String";
        }
        else if (lowerTypeName.find("binary") != std::string::npos ||
            lowerTypeName.find("varbinary") != std::string::npos ||
            lowerTypeName.find("blob") != std::string::npos) {
            return L"[B"; // byte array
        }
        else if (lowerTypeName.find("guid") != std::string::npos ||
            lowerTypeName.find("uuid") != std::string::npos) {
            return L"java.util.UUID";
        }

        // Fallback для неизвестных типов
        if (sqlType == SQL_UNKNOWN_TYPE) {
            return L"java.lang.Object";
        }

        // Для известных SQL типов без конкретного case
        return L"java.lang.String";

    }
    catch (...) {
        return L"java.lang.Object";
    }
}

// Получение строкового атрибута колонки через ODBC
inline static std::wstring getColumnStringAttribute(const SQLHSTMT& hStmt, const SQLUSMALLINT& column, const SQLUSMALLINT& field) {
    SQLWCHAR buffer[BUFFER_SIZE] = { 0 };
    SQLSMALLINT byteLength = 0;
    SQLRETURN ret = SQLColAttributeW(
        hStmt,
        column,
        field,
        buffer,
        sizeof(buffer) / sizeof(SQLWCHAR),
        &byteLength,
        nullptr);

    if (isOdbcSuccess(ret) && byteLength > 0) {
        // Преобразуем байты в количество символов
        SQLSMALLINT numChars = byteLength / sizeof(SQLWCHAR);

        // Защита от переполнения
        numChars = std::min(numChars, static_cast<SQLSMALLINT>(BUFFER_SIZE - 1));
        return std::wstring(buffer, numChars);
    }
    return std::wstring();
}

// Получение числового атрибута колонки через ODBC
inline static SQLLEN getColumnNumericAttribute(const SQLHSTMT& hStmt, const SQLUSMALLINT& column, const SQLUSMALLINT& field) {
    SQLLEN value = 0;
    SQLRETURN ret = SQLColAttributeW(
        hStmt,
        column,
        field,
        nullptr,
        0,
        nullptr,
        &value);

    return isOdbcSuccess(ret) ? value : 0;
}

ResultSetMetaData::ResultSetMetaData(const nanodbc::result& result){
    result_ = result;
    hStmt_ = static_cast<SQLHSTMT>(result.native_statement_handle());

    if (!hStmt_) {
        throw std::runtime_error("Invalid statement handle");
    }
}

int ResultSetMetaData::getColumnCount() const {
    SQLSMALLINT count = 0;
    if (SQLNumResultCols(hStmt_, &count) == SQL_SUCCESS) {
        return count;
    }
    return result_.columns(); // fallback to nanodbc
}

bool ResultSetMetaData::isAutoIncrement(int column) const {
    SQLLEN value = getColumnNumericAttribute(hStmt_, column, SQL_DESC_AUTO_UNIQUE_VALUE);
    if (value == SQL_TRUE) return true;

    // Дополнительные проверки через другие атрибуты
    value = getColumnNumericAttribute(hStmt_, column, SQL_DESC_BASE_COLUMN_NAME);
    std::wstring name = getColumnStringAttribute(hStmt_, column, SQL_DESC_BASE_COLUMN_NAME);

    // Ёвристика: если им¤ содержит "id" или "identity", возможно это автоинкремент
    std::wstring lowerName = name;
    std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::towlower);
    return (lowerName.find(L"id") != std::wstring::npos ||
        lowerName.find(L"identity") != std::wstring::npos);
}

bool ResultSetMetaData::isCaseSensitive(int column) const {
    SQLLEN value = getColumnNumericAttribute(hStmt_, column, SQL_DESC_CASE_SENSITIVE);
    if (value == SQL_TRUE) return true;
    if (value == SQL_FALSE) return false;

    // Fallback: проверка по типу данных
    int type = getColumnType(column);
    return (type == SQL_VARCHAR || type == SQL_CHAR ||
        type == SQL_WVARCHAR || type == SQL_WCHAR);
}

bool ResultSetMetaData::isSearchable(int column) const {
    SQLLEN value = getColumnNumericAttribute(hStmt_, column, SQL_DESC_SEARCHABLE);
    if (value == SQL_PRED_NONE) return false;
    if (value == SQL_PRED_BASIC || value == SQL_PRED_CHAR || value == SQL_SEARCHABLE) return true;

    // Fallback: исключаем BLOB-типы
    int type = getColumnType(column);
    return !(type == SQL_LONGVARBINARY || type == SQL_LONGVARCHAR);
}

bool ResultSetMetaData::isCurrency(int column) const {
    SQLLEN value = getColumnNumericAttribute(hStmt_, column, SQL_DESC_FIXED_PREC_SCALE);
    if (value == SQL_TRUE) return true;

    // проверка по имени типа
    std::wstring typeName = getColumnTypeName(column);
    std::transform(typeName.begin(), typeName.end(), typeName.begin(), ::towlower);
    return (typeName.find(L"money") != std::wstring::npos ||
        typeName.find(L"currency") != std::wstring::npos);
}

int ResultSetMetaData::isNullable(int column) const {
    SQLSMALLINT nullable = SQL_NULLABLE_UNKNOWN;
    SQLDescribeColW(hStmt_, column, nullptr, 0, nullptr, nullptr, nullptr, nullptr, &nullable);

    if (nullable == SQL_NULLABLE || nullable == SQL_NO_NULLS) {
        return nullable;
    }

    return SQL_NULLABLE_UNKNOWN;
}

bool ResultSetMetaData::isSigned(int column) const {
    SQLLEN value = getColumnNumericAttribute(hStmt_, column, SQL_DESC_UNSIGNED);
    if (value == SQL_FALSE) return true;
    if (value == SQL_TRUE) return false;

    // Fallback: проверка по типу данных
    int type = getColumnType(column);
    return (type == SQL_INTEGER || type == SQL_BIGINT || type == SQL_SMALLINT ||
        type == SQL_TINYINT || type == SQL_DECIMAL || type == SQL_NUMERIC ||
        type == SQL_REAL || type == SQL_FLOAT || type == SQL_DOUBLE);
}

int ResultSetMetaData::getColumnDisplaySize(int column) const {
    SQLLEN size = getColumnNumericAttribute(hStmt_, column, SQL_DESC_DISPLAY_SIZE);
    if (size > 0) return static_cast<int>(size);

    // Fallback через nanodbc
    try {
        return static_cast<int>(result_.column_size(column - 1));
    }
    catch (...) {
        return 0;
    }
}

std::wstring ResultSetMetaData::getColumnLabel(int column) const {
    std::wstring label = getColumnStringAttribute(hStmt_, column, SQL_DESC_LABEL);
    if (!label.empty()) return label;

    return getColumnName(column); // ≈сли нет специального label, используем им¤
}

std::wstring ResultSetMetaData::getColumnName(int column) const {
    std::wstring name = getColumnStringAttribute(hStmt_, column, SQL_DESC_NAME);
    if (!name.empty()) return name;

    // Fallback через nanodbc
    try {
        return result_.column_name(column - 1);
    }
    catch (...) {
        return std::wstring();
    }
}

std::wstring ResultSetMetaData::getSchemaName(int column) const {
    return getColumnStringAttribute(hStmt_, column, SQL_DESC_SCHEMA_NAME);
}

int ResultSetMetaData::getPrecision(int column) const {
    SQLLEN precision = getColumnNumericAttribute(hStmt_, column, SQL_DESC_PRECISION);
    if (precision > 0) return static_cast<int>(precision);

    // Fallback: дл¤ строковых типов используем размер
    int type = getColumnType(column);
    if (type == SQL_CHAR || type == SQL_VARCHAR ||
        type == SQL_WCHAR || type == SQL_WVARCHAR) {
        return getColumnDisplaySize(column);
    }

    return 0;
}

int ResultSetMetaData::getScale(int column) const {
    SQLLEN scale = getColumnNumericAttribute(hStmt_, column, SQL_DESC_SCALE);
    if (scale >= 0) return static_cast<int>(scale);

    // Fallback через nanodbc
    try {
        return result_.column_decimal_digits(column - 1);
    }
    catch (...) {
        return 0;
    }
}

std::wstring ResultSetMetaData::getTableName(int column) const {
    return getColumnStringAttribute(hStmt_, column, SQL_DESC_TABLE_NAME);
}

std::wstring ResultSetMetaData::getCatalogName(int column) const {
    return getColumnStringAttribute(hStmt_, column, SQL_DESC_CATALOG_NAME);
}

int ResultSetMetaData::getColumnType(int column) const {
    SQLSMALLINT type = 0;
    SQLSMALLINT unused;
    if (SQLDescribeColW(hStmt_, column, nullptr, 0, &unused, &type, nullptr, nullptr, nullptr) == SQL_SUCCESS) {
        return type;
    }

    // Fallback через nanodbc
    try {
        return result_.column_datatype(column - 1);
    }
    catch (...) {
        return SQL_UNKNOWN_TYPE;
    }
}

std::wstring ResultSetMetaData::getColumnTypeName(int column) const {
    // Сначала пробуем получить им¤ типа через ODBC
    std::wstring typeName = getColumnStringAttribute(hStmt_, column, SQL_DESC_TYPE_NAME);
    if (!typeName.empty()) return typeName;

    // Fallback: сопоставление по типу данных
    int type = getColumnType(column);
    switch (type) {
        case SQL_INTEGER: return L"INTEGER";
        case SQL_VARCHAR: return L"VARCHAR";
        case SQL_CHAR: return L"CHAR";
        case SQL_DECIMAL: return L"DECIMAL";
        case SQL_NUMERIC: return L"NUMERIC";
        case SQL_TIMESTAMP: return L"TIMESTAMP";
        case SQL_BIGINT: return L"BIGINT";
        case SQL_SMALLINT: return L"SMALLINT";
        case SQL_TINYINT: return L"TINYINT";
        case SQL_REAL: return L"REAL";
        case SQL_FLOAT: return L"FLOAT";
        case SQL_DOUBLE: return L"DOUBLE";
        case SQL_DATE: return L"DATE";
        case SQL_TIME: return L"TIME";
        case SQL_WVARCHAR: return L"NVARCHAR";
        case SQL_WCHAR: return L"NCHAR";
        case SQL_BIT: return L"BIT";
        case SQL_BINARY: return L"BINARY";
        case SQL_VARBINARY: return L"VARBINARY";
        case SQL_LONGVARCHAR: return L"LONGVARCHAR";
        case SQL_LONGVARBINARY: return L"LONGVARBINARY";
        default: return L"UNKNOWN";
    }
}

// 18. boolean isReadOnly(int column)
bool ResultSetMetaData::isReadOnly(int column) const {
    SQLLEN updatable = getColumnNumericAttribute(hStmt_, column, SQL_DESC_UPDATABLE);
    return (updatable == SQL_ATTR_READONLY ||
        updatable == SQL_ATTR_READWRITE_UNKNOWN);
}

bool ResultSetMetaData::isWritable(int column) const {
    return !isReadOnly(column);
}

bool ResultSetMetaData::isDefinitelyWritable(int column) const {
    return !isReadOnly(column) && !isAutoIncrement(column);
}

std::wstring ResultSetMetaData::getColumnClassName(int column) const {
    int sqlType = getColumnType(column);

    switch (sqlType) {
#ifdef SQL_WCHAR
        case SQL_WCHAR:
#endif
#ifdef SQL_WVARCHAR
        case SQL_WVARCHAR:
#endif
#ifdef SQL_WLONGVARCHAR
        case SQL_WLONGVARCHAR:
#endif
            return L"java.lang.String";

        case SQL_DECIMAL:
        case SQL_NUMERIC:
            return L"java.math.BigDecimal";

        case SQL_INTEGER:
            return L"java.lang.Integer";

        case SQL_BIGINT:
            return L"java.lang.Long";

        case SQL_SMALLINT:
            return L"java.lang.Short";

        case SQL_TINYINT:
            return L"java.lang.Byte";

        case SQL_REAL:
            return L"java.lang.Float";

        case SQL_FLOAT:
        case SQL_DOUBLE:
            return L"java.lang.Double";

        case SQL_BIT:
            return L"java.lang.Boolean";

        case SQL_DATE:
            return L"java.sql.Date";

        case SQL_TIME:
            return L"java.sql.Time";

        case SQL_TIMESTAMP:
            return L"java.sql.Timestamp";

        case SQL_BINARY:
        case SQL_VARBINARY:
        case SQL_LONGVARBINARY:
            return L"[B";    // byte array

        default:
            return determineClassNameByTypeName(column, sqlType, getColumnTypeName(column));
    }
}

