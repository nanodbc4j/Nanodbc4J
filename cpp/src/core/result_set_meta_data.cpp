#include "core/result_set_meta_data.hpp"
#include <string>
#include <algorithm>
#include <locale>
#include <codecvt>

#ifdef _WIN32
// needs to be included above sql.h for windows
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <sqlext.h>
#include <sql.h>
#include "utils/logger.hpp"
#include "utils/string_utils.hpp"
#include "core/nanodbc_defs.h"

#define BUFFER_SIZE 1024

using namespace utils;

// Вспомогательный метод для определения по имени типа
static const nanodbc::string determineClassNameByTypeName(int column, int sqlType, const nanodbc::string& typeName) {
    LOG_TRACE("column={}, sqlType={}, typeName={}", column, sqlType, to_string(typeName));

    try {
        std::string lowerTypeName = to_string(typeName);
        std::for_each(lowerTypeName.begin(), lowerTypeName.end(), [](char& c) {
            c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        });

        LOG_DEBUG("Resolved type name in lowercase: {}", lowerTypeName);

        if (lowerTypeName.find("bool") != std::string::npos ||
            lowerTypeName.find("bit") != std::string::npos) {
            LOG_TRACE("Mapping to java.lang.Boolean");
            return NANODBC_TEXT("java.lang.Boolean");
        } else if (lowerTypeName.find("tinyint") != std::string::npos) {
            LOG_TRACE("Mapping to java.lang.Byte");
            return NANODBC_TEXT("java.lang.Byte");
        } else if (lowerTypeName.find("smallint") != std::string::npos) {
            LOG_TRACE("Mapping to java.lang.Short");
            return NANODBC_TEXT("java.lang.Short");
        } else if (lowerTypeName.find("int") != std::string::npos ||
            lowerTypeName.find("integer") != std::string::npos) {
            LOG_TRACE("Mapping to java.lang.Integer");
            return NANODBC_TEXT("java.lang.Integer");
        } else if (lowerTypeName.find("bigint") != std::string::npos) {
            LOG_TRACE("Mapping to java.lang.Long");
            return NANODBC_TEXT("java.lang.Long");
        } else if (lowerTypeName.find("float") != std::string::npos) {
            LOG_TRACE("Mapping to java.lang.Float");
            return NANODBC_TEXT("java.lang.Float");
        } else if (lowerTypeName.find("real") != std::string::npos) {
            LOG_TRACE("Mapping to java.lang.Float");
            return NANODBC_TEXT("java.lang.Float");
        } else if (lowerTypeName.find("double") != std::string::npos) {
            LOG_TRACE("Mapping to java.lang.Double");
            return NANODBC_TEXT("java.lang.Double");
        } else if (lowerTypeName.find("decimal") != std::string::npos ||
            lowerTypeName.find("numeric") != std::string::npos) {
            LOG_TRACE("Mapping to java.math.BigDecimal");
            return NANODBC_TEXT("java.math.BigDecimal");
        } else if (lowerTypeName.find("date") != std::string::npos) {
            LOG_TRACE("Mapping to java.sql.Date");
            return NANODBC_TEXT("java.sql.Date");
        } else if (lowerTypeName.find("timestamp") != std::string::npos ||
            lowerTypeName.find("datetime") != std::string::npos) {
            LOG_TRACE("Mapping to java.sql.Timestamp");
            return NANODBC_TEXT("java.sql.Timestamp");
        } else if (lowerTypeName.find("time") != std::string::npos) {
            LOG_TRACE("Mapping to java.sql.Time");
            return NANODBC_TEXT("java.sql.Time");
        } else if (lowerTypeName.find("char") != std::string::npos ||
            lowerTypeName.find("varchar") != std::string::npos ||
            lowerTypeName.find("text") != std::string::npos ||
            lowerTypeName.find("string") != std::string::npos) {
            LOG_TRACE("Mapping to java.lang.String");
            return NANODBC_TEXT("java.lang.String");
        } else if (lowerTypeName.find("binary") != std::string::npos ||
            lowerTypeName.find("varbinary") != std::string::npos ||
            lowerTypeName.find("blob") != std::string::npos) {
            LOG_TRACE("Mapping to [B (byte array)");
            return NANODBC_TEXT("[B"); // byte array
        } else if (lowerTypeName.find("guid") != std::string::npos ||
            lowerTypeName.find("uuid") != std::string::npos) {
            LOG_TRACE("Mapping to java.util.UUID");
            return NANODBC_TEXT("java.util.UUID");
        }

        // Fallback для неизвестных типов
        if (sqlType == SQL_UNKNOWN_TYPE) {
            LOG_DEBUG("SQL type is unknown, falling back to java.lang.Object");
            return NANODBC_TEXT("java.lang.Object");
        }

        // Для известных SQL типов без конкретного case
        LOG_DEBUG("No match found, falling back to java.lang.String for type: {}", to_string(typeName));
        return NANODBC_TEXT("java.lang.String");

    } catch (const std::exception& e) {
        LOG_DEBUG("Exception in determineClassNameByTypeName: {}", e.what());
        return NANODBC_TEXT("java.lang.Object");
    } catch (...) {
        LOG_DEBUG("Unknown exception in determineClassNameByTypeName");
        return NANODBC_TEXT("java.lang.Object");
    }
}

template <std::size_t N>
inline std::size_t size(NANODBC_SQLCHAR const (&array)[N]) noexcept {
    auto const n = std::char_traits<NANODBC_SQLCHAR>::length(array);
    assert(n < N);
    return n < N ? n : N - 1;
}

// Получение строкового атрибута колонки через ODBC
inline static nanodbc::string getColumnStringAttribute(const SQLHSTMT& hStmt, const SQLUSMALLINT& column, const SQLUSMALLINT& field) {
    LOG_TRACE("hStmt={}, column={}, field={}", hStmt, column, field);

    NANODBC_SQLCHAR buffer[BUFFER_SIZE] = { 0 };
    SQLSMALLINT byteLength = 0;
    RETCODE rc = NANODBC_FUNC(SQLColAttribute)(
        hStmt,
        column,
        field,
        buffer,
        sizeof(buffer) / sizeof(NANODBC_SQLCHAR),
        &byteLength,
        nullptr);

    if (SQL_SUCCEEDED(rc)) {
        return nanodbc::string(&buffer[0], &buffer[size(buffer)]);
    }
    LOG_DEBUG("Failed to get string attribute, return empty wstring");
    return nanodbc::string();
}

// Получение числового атрибута колонки через ODBC
inline static SQLLEN getColumnNumericAttribute(const SQLHSTMT& hStmt, const SQLUSMALLINT& column, const SQLUSMALLINT& field) {
    LOG_TRACE("hStmt={}, column={}, field={}", hStmt, column, field);

    SQLLEN value = 0;
    RETCODE rc = NANODBC_FUNC(SQLColAttribute)(
        hStmt,
        column,
        field,
        nullptr,
        0,
        nullptr,
        &value);

    if (SQL_SUCCEEDED(rc)) {
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
    nanodbc::string name = getColumnStringAttribute(result_.native_statement_handle(), column, SQL_DESC_BASE_COLUMN_NAME);

    // Ёвристика: если им¤ содержит "id" или "identity", возможно это автоинкремент
    std::string lowerName = to_string(name);

    std::for_each(lowerName.begin(), lowerName.end(), [](char& c) {
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));    
    });

    bool heuristicMatch = (lowerName.find("id") != std::string::npos ||
                           lowerName.find("identity") != std::string::npos);
    if (heuristicMatch) {
        LOG_DEBUG("Heuristic: column {} name '{}' suggests auto-increment", column, lowerName);
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
    std::string typeName = to_string(getColumnTypeName(column));

    std::for_each(typeName.begin(), typeName.end(), [](char& c) {
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    });

    bool isCurrency = (typeName.find("money") != std::string::npos ||
                        typeName.find("currency") != std::string::npos);
    if (isCurrency) {
        LOG_DEBUG("Column {} type name '{}' suggests currency", column, typeName);
    }
    return isCurrency;
}

int ResultSetMetaData::isNullable(int column) const {
    LOG_TRACE("column={}", column);

    SQLSMALLINT nullable = SQL_NULLABLE_UNKNOWN;
    RETCODE rc = NANODBC_FUNC(SQLDescribeCol)(
        result_.native_statement_handle(), 
        column, 
        nullptr, 
        0, 
        nullptr, 
        nullptr, 
        nullptr, 
        nullptr, 
        &nullable
    );

    if (SQL_SUCCEEDED(rc)) {
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

nanodbc::string ResultSetMetaData::getColumnLabel(int column) const {
    LOG_TRACE("column={}", column);
    nanodbc::string label = getColumnStringAttribute(result_.native_statement_handle(), column, SQL_DESC_LABEL);
    if (!label.empty()) {
        LOG_DEBUG("Column {} label: '{}'", column, to_string(label));
        return label;
    }

    // если нет специального label, используем имя
    nanodbc::string name = getColumnName(column);
    LOG_DEBUG("Using column name as label: '{}'", to_string(name));
    return name;
}

nanodbc::string ResultSetMetaData::getColumnName(int column) const {
    LOG_TRACE("column={}", column);

    try {
        nanodbc::string name = result_.column_name(column - 1);
        LOG_DEBUG("Column {} name: '{}'", column, to_string(name));
        return name;
    } catch (const std::exception& e) {
        LOG_DEBUG("Exception in getColumnName({}): {}", column, e.what());
    } catch (...) {
        LOG_DEBUG("Unknown exception in getColumnName({})", column);
    }
    return nanodbc::string();
}

nanodbc::string ResultSetMetaData::getSchemaName(int column) const {
    LOG_TRACE("column={}", column);
    nanodbc::string schema = getColumnStringAttribute(result_.native_statement_handle(), column, SQL_DESC_SCHEMA_NAME);
    if (!schema.empty()) {
        LOG_DEBUG("Column {} schema: '{}'", column, to_string(schema));
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

nanodbc::string ResultSetMetaData::getTableName(int column) const {
    LOG_TRACE("column={}", column);
    nanodbc::string table = getColumnStringAttribute(result_.native_statement_handle(), column, SQL_DESC_TABLE_NAME);
    if (!table.empty()) {
        LOG_DEBUG("Column {} table: '{}'", column, to_string(table));
    }
    return table;
}

nanodbc::string ResultSetMetaData::getCatalogName(int column) const {
    LOG_TRACE("column={}", column);
    nanodbc::string catalog = getColumnStringAttribute(result_.native_statement_handle(), column, SQL_DESC_CATALOG_NAME);
    if (!catalog.empty()) {
        LOG_DEBUG("Column {} catalog: '{}'", column, to_string(catalog));
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
    SQLRETURN rc = NANODBC_FUNC(SQLDescribeCol)(
        result_.native_statement_handle(), 
        column, 
        nullptr, 
        0, 
        nullptr, 
        &type, 
        nullptr, 
        nullptr, 
        nullptr
    );
    if (SQL_SUCCEEDED(rc)) {
        LOG_DEBUG("Column {} type (via SQLDescribeColW): {}", column, type);
        return type;
    }

    LOG_DEBUG("Column type unknown, returning SQL_UNKNOWN_TYPE");
    return SQL_UNKNOWN_TYPE;
}

nanodbc::string ResultSetMetaData::getColumnTypeName(int column) const {
    LOG_TRACE("column={}", column);
    // Сначала пробуем получить им¤ типа через ODBC
    nanodbc::string typeName = getColumnStringAttribute(result_.native_statement_handle(), column, SQL_DESC_TYPE_NAME);
    if (!typeName.empty()) {
        LOG_DEBUG("Column {} type name: '{}'", column, to_string(typeName));
        return typeName;
    }

    LOG_DEBUG("No type name from ODBC, using fallback mapping");

    // Fallback: сопоставление по типу данных
    int type = getColumnType(column);
    switch (type) {
        case SQL_INTEGER: return NANODBC_TEXT("INTEGER");
        case SQL_VARCHAR: return NANODBC_TEXT("VARCHAR");
        case SQL_CHAR: return NANODBC_TEXT("CHAR");
        case SQL_DECIMAL: return NANODBC_TEXT("DECIMAL");
        case SQL_NUMERIC: return NANODBC_TEXT("NUMERIC");
        case SQL_TIMESTAMP: return NANODBC_TEXT("TIMESTAMP");
        case SQL_BIGINT: return NANODBC_TEXT("BIGINT");
        case SQL_SMALLINT: return NANODBC_TEXT("SMALLINT");
        case SQL_TINYINT: return NANODBC_TEXT("TINYINT");
        case SQL_REAL: return NANODBC_TEXT("REAL");
        case SQL_FLOAT: return NANODBC_TEXT("FLOAT");
        case SQL_DOUBLE: return NANODBC_TEXT("DOUBLE");
        case SQL_DATE: return NANODBC_TEXT("DATE");
        case SQL_TIME: return NANODBC_TEXT("TIME");
        case SQL_WVARCHAR: return NANODBC_TEXT("NVARCHAR");
        case SQL_WCHAR: return NANODBC_TEXT("NCHAR");
        case SQL_BIT: return NANODBC_TEXT("BIT");
        case SQL_BINARY: return NANODBC_TEXT("BINARY");
        case SQL_VARBINARY: return NANODBC_TEXT("VARBINARY");
        case SQL_LONGVARCHAR: return NANODBC_TEXT("LONGVARCHAR");
        case SQL_LONGVARBINARY: return NANODBC_TEXT("LONGVARBINARY");
        default: {
            LOG_DEBUG("Unknown SQL type {}, returning UNKNOWN", type);
            return NANODBC_TEXT("UNKNOWN");
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

nanodbc::string ResultSetMetaData::getColumnClassName(int column) const {
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
            LOG_TRACE("Mapping wide string type to java.lang.String");
            return NANODBC_TEXT("java.lang.String");

        case SQL_DECIMAL:
        case SQL_NUMERIC:
            LOG_TRACE("Mapping decimal/numeric to java.math.BigDecimal");
            return NANODBC_TEXT("java.math.BigDecimal");

        case SQL_INTEGER:
            LOG_TRACE("Mapping INTEGER to java.lang.Integer");
            return NANODBC_TEXT("java.lang.Integer");

        case SQL_BIGINT:
            LOG_TRACE("Mapping BIGINT to java.lang.Long");
            return NANODBC_TEXT("java.lang.Long");

        case SQL_SMALLINT:
            LOG_TRACE("Mapping SMALLINT to java.lang.Short");
            return NANODBC_TEXT("java.lang.Short");

        case SQL_TINYINT:
            LOG_TRACE("Mapping TINYINT to java.lang.Byte");
            return NANODBC_TEXT("java.lang.Byte");

        case SQL_REAL:
            LOG_TRACE("Mapping REAL to java.lang.Float");
            return NANODBC_TEXT("java.lang.Float");

        case SQL_FLOAT:
        case SQL_DOUBLE:
            LOG_TRACE("Mapping FLOAT/DOUBLE to java.lang.Double");
            return NANODBC_TEXT("java.lang.Double");

        case SQL_BIT:
            LOG_TRACE("Mapping BIT to java.lang.Boolean");
            return NANODBC_TEXT("java.lang.Boolean");

        case SQL_DATE:
            LOG_TRACE("Mapping DATE to java.sql.Date");
            return NANODBC_TEXT("java.sql.Date");

        case SQL_TIME:
            LOG_TRACE("Mapping TIME to java.sql.Time");
            return NANODBC_TEXT("java.sql.Time");

        case SQL_TIMESTAMP:
            LOG_TRACE("Mapping TIMESTAMP to java.sql.Timestamp");
            return NANODBC_TEXT("java.sql.Timestamp");

        case SQL_BINARY:
        case SQL_VARBINARY:
        case SQL_LONGVARBINARY:
            LOG_TRACE("Mapping binary type to [B (byte array)");
            return NANODBC_TEXT("[B");

        default: {
            nanodbc::string typeName = getColumnTypeName(column);
            nanodbc::string className = determineClassNameByTypeName(column, sqlType, typeName);
            LOG_DEBUG("Using determineClassNameByTypeName: type={}, typeName='{}' -> className='{}'", sqlType, to_string(typeName), to_string(className));
            return className;
        }
    }
}
