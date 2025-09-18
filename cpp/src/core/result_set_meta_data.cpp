#include "core/result_set_meta_data.hpp"
#include <string>
#include <algorithm>
#include <locale>
#include <codecvt>
#include <wtypes.h>
#include <sqlext.h>
#include <sql.h>
#include "utils/logger.hpp"

#define BUFFER_SIZE 1024

// Проверка успешности выполнени¤ ODBC операции
inline static bool isOdbcSuccess(const SQLRETURN& ret) {
    bool success = (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO);
    if (!success) {
        LOG_DEBUG("ODBC operation failed with return code: {}", ret);
    }
    return success;
}

// Вспомогательный метод для определения по имени типа
static const std::wstring determineClassNameByTypeName(int column, int sqlType, const std::wstring& typeName) {
    LOG_TRACE_W(L"column={}, sqlType={}, typeName={}", column, sqlType, typeName);

    try {
        std::string lowerTypeName;
        lowerTypeName.reserve(typeName.size());

        // Преобразование wstring в string и в нижний регистр
        for (wchar_t c : typeName) {
            lowerTypeName += static_cast<char>(std::tolower(c));
        }

        LOG_DEBUG("Resolved type name in lowercase: {}", lowerTypeName);

        if (lowerTypeName.find("bool") != std::string::npos ||
            lowerTypeName.find("bit") != std::string::npos) {
            LOG_TRACE("Mapping to java.lang.Boolean");
            return L"java.lang.Boolean";
        }
        else if (lowerTypeName.find("tinyint") != std::string::npos) {
            LOG_TRACE("Mapping to java.lang.Byte");
            return L"java.lang.Byte";
        }
        else if (lowerTypeName.find("smallint") != std::string::npos) {
            LOG_TRACE("Mapping to java.lang.Short");
            return L"java.lang.Short";
        }
        else if (lowerTypeName.find("int") != std::string::npos ||
            lowerTypeName.find("integer") != std::string::npos) {
            LOG_TRACE("Mapping to java.lang.Integer");
            return L"java.lang.Integer";
        }
        else if (lowerTypeName.find("bigint") != std::string::npos) {
            LOG_TRACE("Mapping to java.lang.Long");
            return L"java.lang.Long";
        }
        else if (lowerTypeName.find("float") != std::string::npos) {
            LOG_TRACE("Mapping to java.lang.Float");
            return L"java.lang.Float";
        }
        else if (lowerTypeName.find("real") != std::string::npos) {
            LOG_TRACE("Mapping to java.lang.Float");
            return L"java.lang.Float";
        }
        else if (lowerTypeName.find("double") != std::string::npos) {
            LOG_TRACE("Mapping to java.lang.Double");
            return L"java.lang.Double";
        }
        else if (lowerTypeName.find("decimal") != std::string::npos ||
            lowerTypeName.find("numeric") != std::string::npos) {
            LOG_TRACE("Mapping to java.math.BigDecimal");
            return L"java.math.BigDecimal";
        }
        else if (lowerTypeName.find("date") != std::string::npos) {
            LOG_TRACE("Mapping to java.sql.Date");
            return L"java.sql.Date";
        }
        else if (lowerTypeName.find("time") != std::string::npos) {
            LOG_TRACE("Mapping to java.sql.Time");
            return L"java.sql.Time";
        }
        else if (lowerTypeName.find("timestamp") != std::string::npos ||
            lowerTypeName.find("datetime") != std::string::npos) {
            LOG_TRACE("Mapping to java.sql.Timestamp");
            return L"java.sql.Timestamp";
        }
        else if (lowerTypeName.find("char") != std::string::npos ||
            lowerTypeName.find("varchar") != std::string::npos ||
            lowerTypeName.find("text") != std::string::npos ||
            lowerTypeName.find("string") != std::string::npos) {
            LOG_TRACE("Mapping to java.lang.String");
            return L"java.lang.String";
        }
        else if (lowerTypeName.find("binary") != std::string::npos ||
            lowerTypeName.find("varbinary") != std::string::npos ||
            lowerTypeName.find("blob") != std::string::npos) {
            LOG_TRACE("Mapping to [B (byte array)");
            return L"[B"; // byte array
        }
        else if (lowerTypeName.find("guid") != std::string::npos ||
            lowerTypeName.find("uuid") != std::string::npos) {
            LOG_TRACE("Mapping to java.util.UUID");
            return L"java.util.UUID";
        }

        // Fallback для неизвестных типов
        if (sqlType == SQL_UNKNOWN_TYPE) {
            LOG_DEBUG("SQL type is unknown, falling back to java.lang.Object");
            return L"java.lang.Object";
        }

        // Для известных SQL типов без конкретного case
        LOG_DEBUG_W(L"No match found, falling back to java.lang.String for type: {}", typeName);
        return L"java.lang.String";

    }
    catch (const std::exception& e) {
        LOG_DEBUG("Exception in determineClassNameByTypeName: {}", e.what());
        return L"java.lang.Object";
    }
    catch (...) {
        LOG_DEBUG("Unknown exception in determineClassNameByTypeName");
        return L"java.lang.Object";
    }
}

// Получение строкового атрибута колонки через ODBC
inline static std::wstring getColumnStringAttribute(const SQLHSTMT& hStmt, const SQLUSMALLINT& column, const SQLUSMALLINT& field) {
    LOG_TRACE("hStmt={}, column={}, field={}", hStmt, column, field);

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
        std::wstring result(buffer, numChars);
        LOG_DEBUG_W(L"Got string attribute value: '{}'", result);
        return result;
    }
    LOG_DEBUG("Failed to get string attribute, return empty wstring");
    return std::wstring();
}

// Получение числового атрибута колонки через ODBC
inline static SQLLEN getColumnNumericAttribute(const SQLHSTMT& hStmt, const SQLUSMALLINT& column, const SQLUSMALLINT& field) {
    LOG_TRACE("hStmt={}, column={}, field={}", hStmt, column, field);

    SQLLEN value = 0;
    SQLRETURN ret = SQLColAttributeW(
        hStmt,
        column,
        field,
        nullptr,
        0,
        nullptr,
        &value);

    if (isOdbcSuccess(ret)) {
        LOG_DEBUG("Numeric attribute value: {}", value);
        return value;
    } else {
        LOG_DEBUG("Failed to get numeric attribute, returning 0");
        return 0;
    }
}

ResultSetMetaData::ResultSetMetaData(const nanodbc::result& result) 
    : result_ (result)
{
    LOG_DEBUG("ResultSetMetaData initialized successfully with {} columns", result.columns());
}

int ResultSetMetaData::getColumnCount() const {   
    int count = result_.columns();
    LOG_TRACE("getColumnCount() = {}", count);
    return count;
}

bool ResultSetMetaData::isAutoIncrement(int column) const {
    LOG_TRACE("column={}", column);
    SQLLEN value = getColumnNumericAttribute(result_.native_statement_handle(), column, SQL_DESC_AUTO_UNIQUE_VALUE);
    if (value == SQL_TRUE) {
        LOG_DEBUG("Column {} is auto-increment (SQL_DESC_AUTO_UNIQUE_VALUE)", column);
        return true;
    }

    // Дополнительные проверки через другие атрибуты
    value = getColumnNumericAttribute(result_.native_statement_handle(), column, SQL_DESC_BASE_COLUMN_NAME);
    std::wstring name = getColumnStringAttribute(result_.native_statement_handle(), column, SQL_DESC_BASE_COLUMN_NAME);

    // Ёвристика: если им¤ содержит "id" или "identity", возможно это автоинкремент
    std::wstring lowerName = name;
    std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::towlower);
    bool heuristicMatch = (lowerName.find(L"id") != std::wstring::npos ||
                           lowerName.find(L"identity") != std::wstring::npos);
    if (heuristicMatch) {
        LOG_DEBUG_W(L"Heuristic: column {} name '{}' suggests auto-increment", column, name);
    }
    return heuristicMatch;
}

bool ResultSetMetaData::isCaseSensitive(int column) const {
    LOG_TRACE("column={}", column);
    SQLLEN value = getColumnNumericAttribute(result_.native_statement_handle(), column, SQL_DESC_CASE_SENSITIVE);
    if (value == SQL_TRUE) return true;
    if (value == SQL_FALSE) return false;

    // Fallback: проверка по типу данных
    int type = getColumnType(column);
    bool isStringType = (type == SQL_VARCHAR || type == SQL_CHAR ||
                         type == SQL_WVARCHAR || type == SQL_WCHAR);
    LOG_DEBUG("Fallback case sensitivity check: type={}, isStringType={}", type, isStringType);
    return isStringType;
}

bool ResultSetMetaData::isSearchable(int column) const {
    LOG_TRACE("column={}", column);
    SQLLEN value = getColumnNumericAttribute(result_.native_statement_handle(), column, SQL_DESC_SEARCHABLE);
    if (value == SQL_PRED_NONE) return false;
    if (value == SQL_PRED_BASIC || value == SQL_PRED_CHAR || value == SQL_SEARCHABLE) return true;

    // Fallback: исключаем BLOB-типы
    int type = getColumnType(column);
    bool searchable = !(type == SQL_LONGVARBINARY || type == SQL_LONGVARCHAR);
    LOG_DEBUG("Fallback searchable check: type={}, searchable={}", type, searchable);
    return searchable;
}

bool ResultSetMetaData::isCurrency(int column) const {
    LOG_TRACE("column={}", column);
    SQLLEN value = getColumnNumericAttribute(result_.native_statement_handle(), column, SQL_DESC_FIXED_PREC_SCALE);
    if (value == SQL_TRUE) {
        LOG_DEBUG("Column {} has fixed precision scale -> currency", column);
        return true;
    }

    // проверка по имени типа
    std::wstring typeName = getColumnTypeName(column);
    std::transform(typeName.begin(), typeName.end(), typeName.begin(), ::towlower);
    bool isCurrency = (typeName.find(L"money") != std::wstring::npos ||
                        typeName.find(L"currency") != std::wstring::npos);
    if (isCurrency) {
        LOG_DEBUG_W(L"Column {} type name '{}' suggests currency", column, typeName);
    }
    return isCurrency;
}

int ResultSetMetaData::isNullable(int column) const {
    LOG_TRACE("column={}", column);

    SQLSMALLINT nullable = SQL_NULLABLE_UNKNOWN;
    SQLRETURN ret = SQLDescribeColW(result_.native_statement_handle(), column, nullptr, 0, nullptr, nullptr, nullptr, nullptr, &nullable);

    if (isOdbcSuccess(ret)) {
        LOG_DEBUG("isNullable: column {} -> {}", column, nullable);
        return nullable;
    }

    LOG_DEBUG("isNullable: failed to describe column {}, returning SQL_NULLABLE_UNKNOWN", column);
    return SQL_NULLABLE_UNKNOWN;
}

bool ResultSetMetaData::isSigned(int column) const {
    LOG_TRACE("column={}", column);
    SQLLEN value = getColumnNumericAttribute(result_.native_statement_handle(), column, SQL_DESC_UNSIGNED);
    if (value == SQL_FALSE) return true;
    if (value == SQL_TRUE) return false;

    // Fallback: проверка по типу данных
    int type = getColumnType(column);
    bool isSigned = (type == SQL_INTEGER || type == SQL_BIGINT || type == SQL_SMALLINT ||
        type == SQL_TINYINT || type == SQL_DECIMAL || type == SQL_NUMERIC ||
        type == SQL_REAL || type == SQL_FLOAT || type == SQL_DOUBLE);
    LOG_DEBUG("Fallback signed check: type={}, isSigned={}", type, isSigned);
    return isSigned;
}

int ResultSetMetaData::getColumnDisplaySize(int column) const {
    LOG_TRACE("column={}", column);
    try {
        int size = static_cast<int>(result_.column_size(column - 1));
        LOG_DEBUG("Column {} display size: {}", column, size);
        return size;
    } catch (const std::exception& e) {
        LOG_DEBUG("Exception in getColumnDisplaySize({}): {}", column, e.what());
    } catch (...) {
        LOG_DEBUG("Unknown exception in getColumnDisplaySize({})", column);
    }
    return 0;
}

std::wstring ResultSetMetaData::getColumnLabel(int column) const {
    LOG_TRACE("column={}", column);
    std::wstring label = getColumnStringAttribute(result_.native_statement_handle(), column, SQL_DESC_LABEL);
    if (!label.empty()) {
        LOG_DEBUG_W(L"Column {} label: '{}'", column, label);
        return label;
    }

    // если нет специального label, используем имя
    std::wstring name = getColumnName(column);
    LOG_DEBUG_W(L"Using column name as label: '{}'", name);
    return name;
}

std::wstring ResultSetMetaData::getColumnName(int column) const {
    LOG_TRACE("column={}", column);

    try {
        std::wstring name = result_.column_name(column - 1);
        LOG_DEBUG_W(L"Column {} name: '{}'", column, name);
        return name;
    } catch (const std::exception& e) {
        LOG_DEBUG("Exception in getColumnName({}): {}", column, e.what());
    } catch (...) {
        LOG_DEBUG("Unknown exception in getColumnName({})", column);
    }
    return std::wstring();
}

std::wstring ResultSetMetaData::getSchemaName(int column) const {
    LOG_TRACE("column={}", column);
    std::wstring schema = getColumnStringAttribute(result_.native_statement_handle(), column, SQL_DESC_SCHEMA_NAME);
    if (!schema.empty()) {
        LOG_DEBUG_W(L"Column {} schema: '{}'", column, schema);
    }
    return schema;
}

int ResultSetMetaData::getPrecision(int column) const {
    LOG_TRACE("column={}", column);

    SQLLEN precision = getColumnNumericAttribute(result_.native_statement_handle(), column, SQL_DESC_PRECISION);
    if (precision > 0) {
        LOG_DEBUG("Column {} precision: {}", column, precision);
        return static_cast<int>(precision);
    }

    int type = getColumnType(column);
    if (type == SQL_CHAR || type == SQL_VARCHAR ||
        type == SQL_WCHAR || type == SQL_WVARCHAR) {
        int size = getColumnDisplaySize(column);
        LOG_DEBUG("String type fallback: precision = display size = {}", size);
        return size;
    }

    LOG_DEBUG("Precision not available and no fallback, returning 0");
    return 0;
}

int ResultSetMetaData::getScale(int column) const {
    LOG_TRACE("column={}", column);

    try {
        int scale = result_.column_decimal_digits(column - 1);
        LOG_DEBUG("Column {} scale: {}", column, scale);
        return scale;
    } catch (const std::exception& e) {
        LOG_DEBUG("Exception in getScale({}): {}", column, e.what());
    } catch (...) {
        LOG_DEBUG("Unknown exception in getScale({})", column);
    }
    return 0;
}

std::wstring ResultSetMetaData::getTableName(int column) const {
    LOG_TRACE("column={}", column);
    std::wstring table = getColumnStringAttribute(result_.native_statement_handle(), column, SQL_DESC_TABLE_NAME);
    if (!table.empty()) {
        LOG_DEBUG_W(L"Column {} table: '{}'", column, table);
    }
    return table;
}

std::wstring ResultSetMetaData::getCatalogName(int column) const {
    LOG_TRACE("column={}", column);
    std::wstring catalog = getColumnStringAttribute(result_.native_statement_handle(), column, SQL_DESC_CATALOG_NAME);
    if (!catalog.empty()) {
        LOG_DEBUG_W(L"Column {} catalog: '{}'", column, catalog);
    }
    return catalog;
}

int ResultSetMetaData::getColumnType(int column) const {
    LOG_TRACE("column={}", column);

    try {
        int type = result_.column_datatype(column - 1);
        LOG_DEBUG("Column {} type (via nanodbc): {}", column, type);
        return type;
    } catch (...) {
        LOG_DEBUG("Failed to get column type via nanodbc, falling back to SQLDescribeColW");
    }

    SQLSMALLINT type = 0;
    SQLRETURN ret = SQLDescribeColW(result_.native_statement_handle(), column, nullptr, 0, nullptr, &type, nullptr, nullptr, nullptr);
    if (isOdbcSuccess(ret)) {
        LOG_DEBUG("Column {} type (via SQLDescribeColW): {}", column, type);
        return type;
    }

    LOG_DEBUG("Column type unknown, returning SQL_UNKNOWN_TYPE");
    return SQL_UNKNOWN_TYPE;
}

std::wstring ResultSetMetaData::getColumnTypeName(int column) const {
    LOG_TRACE("column={}", column);
    // Сначала пробуем получить им¤ типа через ODBC
    std::wstring typeName = getColumnStringAttribute(result_.native_statement_handle(), column, SQL_DESC_TYPE_NAME);
    if (!typeName.empty()) {
        LOG_DEBUG_W(L"Column {} type name: '{}'", column, typeName);
        return typeName;
    }

    LOG_DEBUG("No type name from ODBC, using fallback mapping");

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
        default: {
            std::wstring unknown = L"UNKNOWN";
            LOG_DEBUG_W(L"Unknown SQL type {}, returning '{}'", type, unknown);
            return unknown;
        }
    }
}

// 18. boolean isReadOnly(int column)
bool ResultSetMetaData::isReadOnly(int column) const {
    LOG_TRACE("column={}", column);

    SQLLEN updatable = getColumnNumericAttribute(result_.native_statement_handle(), column, SQL_DESC_UPDATABLE);
    bool readOnly = (updatable == SQL_ATTR_READONLY || updatable == SQL_ATTR_READWRITE_UNKNOWN);
    LOG_DEBUG("Column {} updatable status: {}, readOnly={}", column, updatable, readOnly);
    return readOnly;
}

bool ResultSetMetaData::isWritable(int column) const {
    LOG_TRACE("column={}", column);
    bool writable = !isReadOnly(column);
    LOG_DEBUG("Column {} is writable: {}", column, writable);
    return writable;
}

bool ResultSetMetaData::isDefinitelyWritable(int column) const {
    LOG_TRACE("column={}", column);
    bool definitelyWritable = !isReadOnly(column) && !isAutoIncrement(column);
    LOG_DEBUG("Column {} is definitely writable: {}", column, definitelyWritable);
    return definitelyWritable;
}

std::wstring ResultSetMetaData::getColumnClassName(int column) const {
    LOG_TRACE("column={}", column);

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
            LOG_TRACE_W(L"Mapping wide string type to java.lang.String");
            return L"java.lang.String";

        case SQL_DECIMAL:
        case SQL_NUMERIC:
            LOG_TRACE_W(L"Mapping decimal/numeric to java.math.BigDecimal");
            return L"java.math.BigDecimal";

        case SQL_INTEGER:
            LOG_TRACE_W(L"Mapping INTEGER to java.lang.Integer");
            return L"java.lang.Integer";

        case SQL_BIGINT:
            LOG_TRACE_W(L"Mapping BIGINT to java.lang.Long");
            return L"java.lang.Long";

        case SQL_SMALLINT:
            LOG_TRACE_W(L"Mapping SMALLINT to java.lang.Short");
            return L"java.lang.Short";

        case SQL_TINYINT:
            LOG_TRACE_W(L"Mapping TINYINT to java.lang.Byte");
            return L"java.lang.Byte";

        case SQL_REAL:
            LOG_TRACE_W(L"Mapping REAL to java.lang.Float");
            return L"java.lang.Float";

        case SQL_FLOAT:
        case SQL_DOUBLE:
            LOG_TRACE_W(L"Mapping FLOAT/DOUBLE to java.lang.Double");
            return L"java.lang.Double";

        case SQL_BIT:
            LOG_TRACE_W(L"Mapping BIT to java.lang.Boolean");
            return L"java.lang.Boolean";

        case SQL_DATE:
            LOG_TRACE_W(L"Mapping DATE to java.sql.Date");
            return L"java.sql.Date";

        case SQL_TIME:
            LOG_TRACE_W(L"Mapping TIME to java.sql.Time");
            return L"java.sql.Time";

        case SQL_TIMESTAMP:
            LOG_TRACE_W(L"Mapping TIMESTAMP to java.sql.Timestamp");
            return L"java.sql.Timestamp";

        case SQL_BINARY:
        case SQL_VARBINARY:
        case SQL_LONGVARBINARY:
            LOG_TRACE_W(L"Mapping binary type to [B (byte array)");
            return L"[B";

        default: {
            std::wstring typeName = getColumnTypeName(column);
            std::wstring className = determineClassNameByTypeName(column, sqlType, typeName);
            LOG_DEBUG_W(L"Using determineClassNameByTypeName: type={}, typeName='{}' -> className='{}'", sqlType, typeName, className);
            return className;
        }
    }
}
