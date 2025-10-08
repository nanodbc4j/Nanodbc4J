#include "core/database_metadata.hpp"
#include <string>
#include <algorithm>
#include <locale>
#include <sstream>

#ifdef _WIN32
// needs to be included above sql.h for windows
#include <windows.h>
#endif

#include <sqlext.h>
#include <cwctype>
#include <sql.h>
#include "utils/string_utils.hpp"
#include "utils/logger.hpp"
#include "core/nanodbc_defs.h"

using namespace utils;

namespace {
    class DatabaseCatalog : public nanodbc::catalog {
    public:
        explicit DatabaseCatalog(nanodbc::connection& conn) : nanodbc::catalog(conn) {
        }

        nanodbc::result getCatalogsResult() {
            return list_catalogs_impl();
        }

        nanodbc::result getSchemasResult() {
            return list_schemas_impl();
        }

        nanodbc::result getTableTypesResult() {
            return list_table_types_impl();
        }
    };

    class DatabaseTables : public nanodbc::catalog::tables {
    public:
        nanodbc::result& getResult() {
            return get_result();
        }
    };

    class DatabaseColumns : public nanodbc::catalog::columns {
    public:
        nanodbc::result& getResult() {
            return get_result();
        }
    };

    class DatabasePrimaryKeys : public nanodbc::catalog::primary_keys {
    public:
        nanodbc::result& getResult() {
            return get_result();
        }
    };

    class DatabaseProcedures : public nanodbc::catalog::procedures {
    public:
        nanodbc::result& getResult() {
            return get_result();
        }
    };

    class DatabaseProcedureColumns : public nanodbc::catalog::procedure_columns {
    public:
        nanodbc::result& getResult() {
            return get_result();
        }
    };

    class ResultSet : public nanodbc::result {
    public:
        ResultSet(nanodbc::statement&& statement, long rowset_size) : nanodbc::result(std::move(statement), rowset_size) {};
    };
}

// === Для строк: использует nanodbc::connection::get_info<T> ===
template <class T>
inline static T getInfoSafely(const nanodbc::connection& conn, SQLUSMALLINT attr, T defaultValue = T{}) {
    // Статическая проверка: T должен быть строковым типом
    static_assert(
        std::is_same_v<T, std::string> ||
        std::is_same_v<T, std::wstring> ||
        std::is_same_v<T, nanodbc::string>,
        "T must be a string type (std::string, std::wstring, nanodbc::string)"
        );

    try {
        return conn.get_info<T>(attr);
    } catch (...) {
        return defaultValue;
    }
}

template<typename T>
inline static T getInfoSafely(SQLHDBC hdbc, SQLUSMALLINT attr, T defaultValue = T{}) {
    // Статическая проверка: T должен быть числовым типом
    static_assert(
        std::is_arithmetic_v<T>,
        "T must be a numeric type (int, short, SQLUSMALLINT, etc.)"
        );

    T value = 0;
    SQLRETURN ret = SQLGetInfo(hdbc, attr, &value, sizeof(T), nullptr);

    if (!SQL_SUCCEEDED(ret)) {
        LOG_ERROR("SQLGetInfo(attr={}) failed with return code {}", attr, ret);
        return defaultValue;
    }

    return value;
}

template<class T>
inline static T joinString(const std::vector<T>& vec, const T& delimiter) {
    T result{};
    for (size_t i = 0; i < vec.size(); ++i) {
        if (i > 0) result += delimiter;
        result += vec[i];
    }
    return result;
}

inline static std::pair<int, int> processingVersionString(std::string ver) {
    // Пропускаем не-цифры и не-знаки в начале строки
    auto it = ver.begin();
    while (it != ver.end() && !std::iswdigit(*it) && *it != '+' && *it != '-' && *it != '.') {
        ++it;
    }

    if (it == ver.end()) {
        LOG_TRACE("No version info found; returning {{0, 0}}");
        return { 0, 0 };
    }
    // Если строка начинается с точки — значит, major = 0
    bool leadingDot = (*it == '.');
    std::string cleaned = leadingDot ? ("0" + std::string(it, ver.end())) : std::string(it, ver.end());
    std::stringstream ss(cleaned);

    int major = 0;
    int minor = 0;

    // Пробуем прочитать major
    if (!(ss >> major)) {
        LOG_TRACE("Failed to parse major version; returning {{0, 0}}");
        return { 0, 0 };
    }

    // Читаем точку и minor, если есть
    if (ss.peek() == '.') {
        char dot;
        ss >> dot;  // Пропускаем точку

        if (!(ss >> minor)) {
            LOG_TRACE("Failed to parse minor version; minor set to 0");
            minor = 0;  // не ошибка — minor может отсутствовать или быть некорректным
        }
    } else {
        LOG_TRACE("No dot found after major; minor version is 0");
        // minor уже 0 — ничего делать не нужно
    }

    return { major, minor };
}

// === Конструктор ===
DatabaseMetaData::DatabaseMetaData(nanodbc::connection& connection)
    : connection_(connection)
{    
    LOG_TRACE("Initialized successfully");
}

// === Строковые методы ===

nanodbc::string DatabaseMetaData::getDatabaseProductName() const {
    LOG_TRACE("Called");
    auto result = connection_.dbms_name();
    LOG_TRACE("Returning: {}", to_string(result));
    return result;
}

nanodbc::string DatabaseMetaData::getDatabaseProductVersion() const {
    LOG_TRACE("Called");
    auto result = connection_.dbms_version();
    LOG_TRACE("Returning: {}", to_string(result));
    return result;
}

nanodbc::string DatabaseMetaData::getDriverName() const {
    LOG_TRACE("Called");
    auto result = connection_.driver_name();
    LOG_TRACE("Returning: {}", to_string(result));
    return result;
}

nanodbc::string DatabaseMetaData::getDriverVersion() const {
    LOG_TRACE("Called");
    auto result = connection_.driver_version();
    LOG_TRACE("Returning: {}", to_string(result));
    return result;
}

nanodbc::string DatabaseMetaData::getIdentifierQuoteString() const {
    LOG_TRACE("Called");
    auto result = getInfoSafely<nanodbc::string>(connection_, SQL_IDENTIFIER_QUOTE_CHAR);
    LOG_TRACE("Returning: {}", to_string(result));
    return result;
}

nanodbc::string DatabaseMetaData::getSchemaTerm() const {
    LOG_TRACE("Called");
    auto result = getInfoSafely<nanodbc::string>(connection_, SQL_SCHEMA_TERM);
    LOG_TRACE("Returning: {}", to_string(result));
    return result;
}

nanodbc::string DatabaseMetaData::getProcedureTerm() const {
    LOG_TRACE("Called");
    auto result = getInfoSafely<nanodbc::string>(connection_, SQL_PROCEDURE_TERM);
    LOG_TRACE("Returning: {}", to_string(result));
    return result;
}

nanodbc::string DatabaseMetaData::getCatalogTerm() const {
    LOG_TRACE("Called");
    auto result = getInfoSafely<nanodbc::string>(connection_, SQL_CATALOG_TERM);
    LOG_TRACE("Returning: {}", to_string(result));
    return result;
}

nanodbc::string DatabaseMetaData::getCatalogSeparator() const {
    LOG_TRACE("Called");
    auto result = getInfoSafely<nanodbc::string>(connection_, SQL_CATALOG_NAME_SEPARATOR);
    LOG_TRACE("Returning: {}", to_string(result));
    return result;
}

nanodbc::string DatabaseMetaData::getSQLKeywords() const {
    LOG_TRACE("Called");
    auto result = getInfoSafely<nanodbc::string>(connection_, SQL_KEYWORDS);
    LOG_TRACE("Returning: {}", to_string(result));
    return result;
}

nanodbc::string DatabaseMetaData::getNumericFunctions() const {
    LOG_TRACE("Called");
    SQLUINTEGER mask = 0;
    SQLRETURN ret = SQLGetInfo(connection_.native_dbc_handle(), SQL_NUMERIC_FUNCTIONS, &mask, sizeof(mask), nullptr);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
        LOG_ERROR("SQLGetInfo(SQL_NUMERIC_FUNCTIONS) failed: {}", ret);
        LOG_TRACE("Returning: L\"\"");
        return NANODBC_TEXT("");
    }

    std::vector<nanodbc::string> funcs;

#define ADD_IF_SET(bit, name) if (mask & (bit)) funcs.push_back(NANODBC_TEXT(name))

    ADD_IF_SET(SQL_FN_NUM_ABS, "ABS");
    ADD_IF_SET(SQL_FN_NUM_ACOS, "ACOS");
    ADD_IF_SET(SQL_FN_NUM_ASIN, "ASIN");
    ADD_IF_SET(SQL_FN_NUM_ATAN, "ATAN");
    ADD_IF_SET(SQL_FN_NUM_ATAN2, "ATAN2");
    ADD_IF_SET(SQL_FN_NUM_CEILING, "CEILING");
    ADD_IF_SET(SQL_FN_NUM_COS, "COS");
    ADD_IF_SET(SQL_FN_NUM_COT, "COT");
    ADD_IF_SET(SQL_FN_NUM_EXP, "EXP");
    ADD_IF_SET(SQL_FN_NUM_FLOOR, "FLOOR");
    ADD_IF_SET(SQL_FN_NUM_LOG, "LOG");
    ADD_IF_SET(SQL_FN_NUM_MOD, "MOD");
    ADD_IF_SET(SQL_FN_NUM_PI, "PI");
    ADD_IF_SET(SQL_FN_NUM_RAND, "RAND");
    ADD_IF_SET(SQL_FN_NUM_ROUND, "ROUND");
    ADD_IF_SET(SQL_FN_NUM_SIGN, "SIGN");
    ADD_IF_SET(SQL_FN_NUM_SIN, "SIN");
    ADD_IF_SET(SQL_FN_NUM_SQRT, "SQRT");
    ADD_IF_SET(SQL_FN_NUM_TAN, "TAN");
    ADD_IF_SET(SQL_FN_NUM_TRUNCATE, "TRUNCATE");

#undef ADD_IF_SET

    nanodbc::string result = joinString<nanodbc::string>(funcs, NANODBC_TEXT(","));

    LOG_TRACE("Returning: {}", to_string(result));
    return result;
}

nanodbc::string DatabaseMetaData::getStringFunctions() const {
    LOG_TRACE("Called");
    SQLUINTEGER mask = 0;
    SQLRETURN ret = SQLGetInfo(connection_.native_dbc_handle(), SQL_STRING_FUNCTIONS, &mask, sizeof(mask), nullptr);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
        LOG_ERROR("SQLGetInfo(SQL_STRING_FUNCTIONS) failed: {}", ret);
        LOG_TRACE("Returning: L\"\"");
        return NANODBC_TEXT("");
    }

    std::vector<nanodbc::string> funcs;

#define ADD_IF_SET(bit, name) if (mask & (bit)) funcs.push_back(NANODBC_TEXT(name))

    ADD_IF_SET(SQL_FN_STR_ASCII, "ASCII");
    ADD_IF_SET(SQL_FN_STR_CHAR, "CHAR");
    ADD_IF_SET(SQL_FN_STR_CONCAT, "CONCAT");
    ADD_IF_SET(SQL_FN_STR_LCASE, "LCASE");
    ADD_IF_SET(SQL_FN_STR_LEFT, "LEFT");
    ADD_IF_SET(SQL_FN_STR_LENGTH, "LENGTH");
    ADD_IF_SET(SQL_FN_STR_LTRIM, "LTRIM");
    ADD_IF_SET(SQL_FN_STR_REPEAT, "REPEAT");
    ADD_IF_SET(SQL_FN_STR_RIGHT, "RIGHT");
    ADD_IF_SET(SQL_FN_STR_RTRIM, "RTRIM");
    ADD_IF_SET(SQL_FN_STR_SPACE, "SPACE");
    ADD_IF_SET(SQL_FN_STR_SUBSTRING, "SUBSTRING");
    ADD_IF_SET(SQL_FN_STR_UCASE, "UCASE");
    ADD_IF_SET(SQL_FN_STR_LOCATE, "LOCATE");
    ADD_IF_SET(SQL_FN_STR_LTRIM, "LTRIM");
    ADD_IF_SET(SQL_FN_STR_RTRIM, "RTRIM");

#undef ADD_IF_SET

    nanodbc::string result = joinString<nanodbc::string>(funcs, NANODBC_TEXT(""));

    LOG_TRACE("Returning: {}", to_string(result));
    return result;
}

nanodbc::string DatabaseMetaData::getSystemFunctions() const {
    LOG_TRACE("Called");
    SQLUINTEGER mask = 0;
    SQLRETURN ret = SQLGetInfo(connection_.native_dbc_handle(), SQL_SYSTEM_FUNCTIONS, &mask, sizeof(mask), nullptr);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
        LOG_ERROR("SQLGetInfo(SQL_SYSTEM_FUNCTIONS) failed: {}", ret);
        LOG_TRACE("Returning: \"\"");
        return NANODBC_TEXT("");
    }

    std::vector<nanodbc::string> funcs;

#define ADD_IF_SET(bit, name) if (mask & (bit)) funcs.push_back(NANODBC_TEXT(name))

    ADD_IF_SET(SQL_FN_SYS_DBNAME, "DBNAME");
    ADD_IF_SET(SQL_FN_SYS_IFNULL, "IFNULL");
    ADD_IF_SET(SQL_FN_SYS_USERNAME, "USERNAME");

#undef ADD_IF_SET

    nanodbc::string result = joinString<nanodbc::string>(funcs, NANODBC_TEXT(","));

    LOG_TRACE("Returning: {}", to_string(result));
    return result;
}

nanodbc::string DatabaseMetaData::getTimeDateFunctions() const {
    LOG_TRACE("Called");
    SQLUINTEGER mask = 0;
    SQLRETURN ret = SQLGetInfo(connection_.native_dbc_handle(), SQL_TIMEDATE_FUNCTIONS, &mask, sizeof(mask), nullptr);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
        LOG_ERROR("SQLGetInfo(SQL_TIMEDATE_FUNCTIONS) failed: {}", ret);
        LOG_TRACE("Returning: \"\"");
        return NANODBC_TEXT("");
    }

    std::vector<nanodbc::string> funcs;

#define ADD_IF_SET(bit, name) if (mask & (bit)) funcs.push_back(NANODBC_TEXT(name))

    ADD_IF_SET(SQL_FN_TD_CURDATE, "CURDATE");
    ADD_IF_SET(SQL_FN_TD_CURTIME, "CURTIME");
    ADD_IF_SET(SQL_FN_TD_DAYNAME, "DAYNAME");
    ADD_IF_SET(SQL_FN_TD_DAYOFMONTH, "DAYOFMONTH");
    ADD_IF_SET(SQL_FN_TD_DAYOFWEEK, "DAYOFWEEK");
    ADD_IF_SET(SQL_FN_TD_DAYOFYEAR, "DAYOFYEAR");
    ADD_IF_SET(SQL_FN_TD_EXTRACT, "EXTRACT");
    ADD_IF_SET(SQL_FN_TD_HOUR, "HOUR");
    ADD_IF_SET(SQL_FN_TD_MINUTE, "MINUTE");
    ADD_IF_SET(SQL_FN_TD_MONTH, "MONTH");
    ADD_IF_SET(SQL_FN_TD_MONTHNAME, "MONTHNAME");
    ADD_IF_SET(SQL_FN_TD_NOW, "NOW");
    ADD_IF_SET(SQL_FN_TD_QUARTER, "QUARTER");
    ADD_IF_SET(SQL_FN_TD_SECOND, "SECOND");
    ADD_IF_SET(SQL_FN_TD_TIMESTAMPADD, "TIMESTAMPADD");
    ADD_IF_SET(SQL_FN_TD_TIMESTAMPDIFF, "TIMESTAMPDIFF");
    ADD_IF_SET(SQL_FN_TD_WEEK, "WEEK");
    ADD_IF_SET(SQL_FN_TD_YEAR, "YEAR");  

#undef ADD_IF_SET

    nanodbc::string result = joinString<nanodbc::string>(funcs, NANODBC_TEXT(","));

    LOG_TRACE("Returning: {}", to_string(result));
    return result;
}

nanodbc::string DatabaseMetaData::getSearchStringEscape() const {
    LOG_TRACE("Called");
    auto result = getInfoSafely<nanodbc::string>(connection_, SQL_SEARCH_PATTERN_ESCAPE);
    LOG_TRACE("Returning: {}", to_string(result));
    return result;
}

nanodbc::string DatabaseMetaData::getExtraNameCharacters() const {
    LOG_TRACE("Called");
    auto result = getInfoSafely<nanodbc::string>(connection_, SQL_SPECIAL_CHARACTERS);
    LOG_TRACE("Returning: {}", to_string(result));
    return result;
}

nanodbc::string DatabaseMetaData::getUserName() const {
    LOG_TRACE("Called");
    auto result = getInfoSafely<nanodbc::string>(connection_, SQL_USER_NAME);
    LOG_TRACE("Returning: {}", to_string(result));
    return result;
}

// === Булевы методы ===

bool DatabaseMetaData::isReadOnly() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(connection_.native_dbc_handle(), SQL_DATA_SOURCE_READ_ONLY, SQL_FALSE);
    bool result = (val == SQL_TRUE);
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsTransactions() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(connection_.native_dbc_handle(), SQL_TXN_CAPABLE, SQL_TC_NONE);
    bool result = (val != SQL_TC_NONE);
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsSavepoints() const {
    LOG_TRACE("Called");
    LOG_TRACE("Returning: false (Savepoints not supported in ODBC)");
    return false;
}

bool DatabaseMetaData::supportsNamedParameters() const {
    LOG_TRACE("Called");
    LOG_TRACE("Returning: false (ODBC does not support named parameters)");
    return false;
}

bool DatabaseMetaData::supportsBatchUpdates() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(connection_.native_dbc_handle(), SQL_BATCH_SUPPORT, 0);
    bool result = (val != 0); // Любое ненулевое значение — поддержка есть
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsUnion() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(connection_.native_dbc_handle(), SQL_UNION, SQL_FALSE);
    bool result = (val == SQL_TRUE);
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsUnionAll() const {
    LOG_TRACE("Called");
    // SQL_UNION_ALL не определён в некоторых SDK, но
    // если SQL_UNION == SQL_TRUE, то UNION ALL тоже поддерживается
    bool result = supportsUnion(); // UNION implies UNION ALL
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsLikeEscapeClause() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(connection_.native_dbc_handle(), SQL_LIKE_ESCAPE_CLAUSE, SQL_FALSE);
    bool result = (val == SQL_TRUE);
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsGroupBy() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(connection_.native_dbc_handle(), SQL_GROUP_BY, SQL_GB_NOT_SUPPORTED);
    bool result = (val != SQL_GB_NOT_SUPPORTED);
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsGroupByUnrelated() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(connection_.native_dbc_handle(), SQL_GROUP_BY, SQL_GB_NOT_SUPPORTED);
    bool result = (val == SQL_GB_COLLATE);
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsGroupByBeyondSelect() const {
    LOG_TRACE("Called");
    bool result = supportsGroupByUnrelated();
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsOrderByUnrelated() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(connection_.native_dbc_handle(), SQL_ORDER_BY_COLUMNS_IN_SELECT, SQL_TRUE);
    bool result = (val == SQL_FALSE);
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsAlterTableWithAddColumn() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(connection_.native_dbc_handle(), SQL_ALTER_TABLE, 0);
    bool result = (val & SQL_AT_ADD_COLUMN) != 0;
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsColumnAliasing() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(connection_.native_dbc_handle(), SQL_COLUMN_ALIAS, SQL_FALSE);
    bool result = (val == SQL_TRUE);
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::nullPlusNonNullIsNull() const {
    LOG_TRACE("Called");
    LOG_TRACE("Returning: false (default assumption)");
    return false;
}

bool DatabaseMetaData::supportsExpressionsInOrderBy() const {
    LOG_TRACE("Called");
    LOG_TRACE("Returning: true (assumed for modern databases)");
    return true;
}

bool DatabaseMetaData::supportsSelectForUpdate() const {
    LOG_TRACE("Called");
    LOG_TRACE("Returning: false (ODBC does not standardly support SELECT FOR UPDATE)");
    return false;
}

bool DatabaseMetaData::supportsStoredProcedures() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(connection_.native_dbc_handle(), SQL_PROCEDURES, SQL_FALSE);
    bool result = (val == SQL_TRUE);
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsSubqueriesInComparisons() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(connection_.native_dbc_handle(), SQL_SUBQUERIES, 0);
    bool result = (val & SQL_SQ_COMPARISON) != 0;
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsSubqueriesInExists() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(connection_.native_dbc_handle(), SQL_SUBQUERIES, 0);
    bool result = (val & SQL_SQ_EXISTS) != 0;
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsSubqueriesInIns() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(connection_.native_dbc_handle(), SQL_SUBQUERIES, 0);
    bool result = (val & SQL_SQ_IN) != 0;
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsSubqueriesInQuantifieds() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(connection_.native_dbc_handle(), SQL_SUBQUERIES, 0);
    bool result = (val & SQL_SQ_QUANTIFIED) != 0;
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsCorrelatedSubqueries() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(connection_.native_dbc_handle(), SQL_SUBQUERIES, 0);
    bool result = (val & SQL_SQ_CORRELATED_SUBQUERIES) != 0;
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsIntegrityEnhancementFacility() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(connection_.native_dbc_handle(), SQL_INTEGRITY, SQL_FALSE);
    bool result = (val == SQL_TRUE);
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsOuterJoins() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(connection_.native_dbc_handle(), SQL_OUTER_JOINS, SQL_FALSE);
    bool result = (val == SQL_TRUE);
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsFullOuterJoins() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(connection_.native_dbc_handle(), SQL_OUTER_JOINS, 0);
    bool result = (val & SQL_SRJO_FULL_OUTER_JOIN) != 0;
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsLimitedOuterJoins() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(connection_.native_dbc_handle(), SQL_OUTER_JOINS, 0);
    bool result = (val == SQL_TRUE);
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsSchemasInDataManipulation() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(connection_.native_dbc_handle(), SQL_SCHEMA_USAGE, 0);
    bool result = (val & SQL_SU_DML_STATEMENTS) != 0;
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsSchemasInProcedureCalls() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(connection_.native_dbc_handle(), SQL_SCHEMA_USAGE, 0);
    bool result = (val & SQL_SU_PROCEDURE_INVOCATION) != 0;
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsSchemasInTableDefinitions() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(connection_.native_dbc_handle(), SQL_SCHEMA_USAGE, 0);
    bool result = (val & SQL_SU_TABLE_DEFINITION) != 0;
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsSchemasInIndexDefinitions() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(connection_.native_dbc_handle(), SQL_SCHEMA_USAGE);
    bool result = (val & SQL_SU_INDEX_DEFINITION) != 0;
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsSchemasInPrivilegeDefinitions() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(connection_.native_dbc_handle(), SQL_SCHEMA_USAGE);
    bool result = (val & SQL_SU_PRIVILEGE_DEFINITION) != 0;
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsCatalogsInDataManipulation() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(connection_.native_dbc_handle(), SQL_CATALOG_USAGE, 0);
    bool result = (val & SQL_CU_DML_STATEMENTS) != 0;
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsCatalogsInProcedureCalls() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(connection_.native_dbc_handle(), SQL_CATALOG_USAGE, 0);
    bool result = (val & SQL_CU_PROCEDURE_INVOCATION) != 0;
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsCatalogsInTableDefinitions() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(connection_.native_dbc_handle(), SQL_CATALOG_USAGE, 0);
    bool result = (val & SQL_CU_TABLE_DEFINITION) != 0;
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsCatalogsInIndexDefinitions() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(connection_.native_dbc_handle(), SQL_CATALOG_USAGE, 0);
    bool result = (val & SQL_CU_INDEX_DEFINITION) != 0;
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsCatalogsInPrivilegeDefinitions() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(connection_.native_dbc_handle(), SQL_CATALOG_USAGE, 0);
    bool result = (val & SQL_CU_PRIVILEGE_DEFINITION) != 0;
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsPositionedDelete() const {
    LOG_TRACE("Called");
    LOG_TRACE("Returning: false");
    return false;
}

bool DatabaseMetaData::supportsPositionedUpdate() const {
    LOG_TRACE("Called");
    LOG_TRACE("Returning: false");
    return false;
}

bool DatabaseMetaData::supportsOpenCursorsAcrossCommit() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(connection_.native_dbc_handle(), SQL_CURSOR_COMMIT_BEHAVIOR, SQL_CB_CLOSE);
    bool result = (val == SQL_CB_PRESERVE);
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsOpenCursorsAcrossRollback() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(connection_.native_dbc_handle(), SQL_CURSOR_ROLLBACK_BEHAVIOR, SQL_CB_CLOSE);
    bool result = (val == SQL_CB_PRESERVE);
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsOpenStatementsAcrossCommit() const {
    LOG_TRACE("Called");
    LOG_TRACE("Returning: false");
    return false;
}

bool DatabaseMetaData::supportsOpenStatementsAcrossRollback() const {
    LOG_TRACE("Called");
    LOG_TRACE("Returning: false");
    return false;
}

bool DatabaseMetaData::locatorsUpdateCopy() const {
    LOG_TRACE("Called");
    LOG_TRACE("Returning: false");
    return false;
}

bool DatabaseMetaData::supportsStatementPooling() const {
    LOG_TRACE("Called");
    LOG_TRACE("Returning: false");
    return false;
}

bool DatabaseMetaData::allProceduresAreCallable() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(connection_.native_dbc_handle(), SQL_ACCESSIBLE_PROCEDURES, SQL_FALSE);
    bool result = (val == SQL_TRUE);
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::allTablesAreSelectable() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(connection_.native_dbc_handle(), SQL_ACCESSIBLE_TABLES, SQL_FALSE);
    bool result = (val == SQL_TRUE);
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsMultipleResultSets() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(connection_.native_dbc_handle(), SQL_MULT_RESULT_SETS, SQL_FALSE);
    bool result = (val == SQL_TRUE);
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsMultipleTransactions() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(connection_.native_dbc_handle(), SQL_MULTIPLE_ACTIVE_TXN, SQL_FALSE);
    bool result = (val == SQL_TRUE);
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::autoCommitFailureClosesAllResultSets() const {
    LOG_TRACE("Called");
    // SQL_MULT_RESULT_SETS: если поддерживает множественные результаты
    auto val = getInfoSafely<SQLUINTEGER>(connection_.native_dbc_handle(), SQL_MULT_RESULT_SETS, SQL_FALSE);
    bool result = (val == SQL_TRUE);
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsStoredFunctionsUsingCallSyntax() const {
    LOG_TRACE("Called");
    LOG_TRACE("Returning: false");
    return false;
}

bool DatabaseMetaData::generatedKeyAlwaysReturned() const {
    LOG_TRACE("Called");
    // SQL_GETDATA_EXTENSIONS: если поддерживает SQL_GD_OUTPUT_PARAMS
    auto val = getInfoSafely<SQLUINTEGER>(connection_.native_dbc_handle(), SQL_GETDATA_EXTENSIONS, 0);
    bool result = (val & SQL_GD_ANY_COLUMN) != 0;
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::nullsAreSortedHigh() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUSMALLINT>(connection_.native_dbc_handle(), SQL_NULL_COLLATION, 0);
    bool result = (val == SQL_NC_HIGH);
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::nullsAreSortedLow() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUSMALLINT>(connection_.native_dbc_handle(), SQL_NULL_COLLATION, 0);
    bool result = (val == SQL_NC_LOW);
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::nullsAreSortedAtStart() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUSMALLINT>(connection_.native_dbc_handle(), SQL_NULL_COLLATION, 0);
    bool result = (val == SQL_NC_START);
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::nullsAreSortedAtEnd() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUSMALLINT>(connection_.native_dbc_handle(), SQL_NULL_COLLATION, 0);
    bool result = (val == SQL_NC_END);
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::usesLocalFiles() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUSMALLINT>(connection_.native_dbc_handle(), SQL_FILE_USAGE, 0);
    bool result = (val == SQL_FILE_QUALIFIER);
    LOG_TRACE("Returning: {}", result);
    return result;
    return false;
}

bool DatabaseMetaData::usesLocalFilePerTable() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUSMALLINT>(connection_.native_dbc_handle(), SQL_FILE_USAGE, 0);
    bool result = (val == SQL_FILE_TABLE);
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsMixedCaseIdentifiers() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUSMALLINT>(connection_.native_dbc_handle(), SQL_IDENTIFIER_CASE, 0);
    bool result = (val == SQL_IC_MIXED);
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::storesUpperCaseIdentifiers() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUSMALLINT>(connection_.native_dbc_handle(), SQL_IDENTIFIER_CASE, 0);
    bool result = (val == SQL_IC_UPPER);
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::storesLowerCaseIdentifiers() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUSMALLINT>(connection_.native_dbc_handle(), SQL_IDENTIFIER_CASE, 0);
    bool result = (val == SQL_IC_LOWER);
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::storesMixedCaseIdentifiers() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUSMALLINT>(connection_.native_dbc_handle(), SQL_IDENTIFIER_CASE, 0);
    bool result = (val == SQL_IC_MIXED);
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsMixedCaseQuotedIdentifiers() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUSMALLINT>(connection_.native_dbc_handle(), SQL_QUOTED_IDENTIFIER_CASE, 0);
    bool result = (val == SQL_IC_SENSITIVE);
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::storesUpperCaseQuotedIdentifiers() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUSMALLINT>(connection_.native_dbc_handle(), SQL_QUOTED_IDENTIFIER_CASE, 0);
    bool result = (val == SQL_IC_UPPER);
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::storesLowerCaseQuotedIdentifiers() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUSMALLINT>(connection_.native_dbc_handle(), SQL_QUOTED_IDENTIFIER_CASE, 0);
    bool result = (val == SQL_IC_LOWER);
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::storesMixedCaseQuotedIdentifiers() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUSMALLINT>(connection_.native_dbc_handle(), SQL_QUOTED_IDENTIFIER_CASE, 0);
    bool result = (val == SQL_IC_MIXED);
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsAlterTableWithDropColumn() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(connection_.native_dbc_handle(), SQL_ALTER_TABLE, 0);
    bool result = (val & SQL_AT_DROP_COLUMN) != 0;
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsConvert() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(connection_.native_dbc_handle(), SQL_CONVERT_FUNCTIONS, 0);
    bool result = (val == SQL_FN_CVT_CONVERT);
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsConvert(int fromType, int toType) const {
    LOG_TRACE("Called with fromType: {}, toType: {}", fromType, toType);

    SQLUSMALLINT odbcInfoType = 0;

    // Mapping JDBC type to ODBC information type
    switch (fromType) {
        case SQL_BIT:
            odbcInfoType = SQL_CONVERT_BIT;
            break;
        case SQL_TINYINT:
            odbcInfoType = SQL_CONVERT_TINYINT;
            break;
        case SQL_BIGINT:
            odbcInfoType = SQL_CONVERT_BIGINT;
            break;
        case SQL_LONGVARBINARY:
            odbcInfoType = SQL_CONVERT_LONGVARBINARY;
            break;
        case SQL_VARBINARY:
            odbcInfoType = SQL_CONVERT_VARBINARY;
            break;
        case SQL_BINARY:
            odbcInfoType = SQL_CONVERT_BINARY;
            break;
        case SQL_LONGVARCHAR:
            odbcInfoType = SQL_CONVERT_LONGVARCHAR;
            break;
        case SQL_CHAR:
            odbcInfoType = SQL_CONVERT_CHAR;
            break;
        case SQL_NUMERIC:
            odbcInfoType = SQL_CONVERT_NUMERIC;
            break;
        case SQL_DECIMAL:
            odbcInfoType = SQL_CONVERT_DECIMAL;
            break;
        case SQL_INTEGER:
            odbcInfoType = SQL_CONVERT_INTEGER;
            break;
        case SQL_SMALLINT:
            odbcInfoType = SQL_CONVERT_SMALLINT;
            break;
        case SQL_FLOAT:
            odbcInfoType = SQL_CONVERT_FLOAT;
            break;
        case SQL_REAL:
            odbcInfoType = SQL_CONVERT_REAL;
            break;
        case SQL_DOUBLE:
            odbcInfoType = SQL_CONVERT_DOUBLE;
            break;
        case SQL_VARCHAR:
            odbcInfoType = SQL_CONVERT_VARCHAR;
            break;
        case SQL_TYPE_DATE:
            odbcInfoType = SQL_CONVERT_DATE;
            break;
        case SQL_TYPE_TIME:
            odbcInfoType = SQL_CONVERT_TIME;
            break;
        case SQL_TYPE_TIMESTAMP:
            odbcInfoType = SQL_CONVERT_TIMESTAMP;
            break;
        default:
            LOG_TRACE("Unsupported fromType: {}, returning false", fromType);
            return false;
    }

    // Mapping target JDBC type to bitmask
    SQLUINTEGER bitmask = 0;
    switch (toType) {
        case SQL_BIT:
            bitmask = SQL_CVT_BIT;
            break;
        case SQL_TINYINT:
            bitmask = SQL_CVT_TINYINT;
            break;
        case SQL_BIGINT:
            bitmask = SQL_CVT_BIGINT;
            break;
        case SQL_LONGVARBINARY:
            bitmask = SQL_CVT_LONGVARBINARY;
            break;
        case SQL_VARBINARY:
            bitmask = SQL_CVT_VARBINARY;
            break;
        case SQL_BINARY:
            bitmask = SQL_CVT_BINARY;
            break;
        case SQL_LONGVARCHAR:
            bitmask = SQL_CVT_LONGVARCHAR;
            break;
        case SQL_CHAR:
            bitmask = SQL_CVT_CHAR;
            break;
        case SQL_NUMERIC:
            bitmask = SQL_CVT_NUMERIC;
            break;
        case SQL_DECIMAL:
            bitmask = SQL_CVT_DECIMAL;
            break;
        case SQL_INTEGER:
            bitmask = SQL_CVT_INTEGER;
            break;
        case SQL_SMALLINT:
            bitmask = SQL_CVT_SMALLINT;
            break;
        case SQL_FLOAT:
            bitmask = SQL_CVT_FLOAT;
            break;
        case SQL_REAL:
            bitmask = SQL_CVT_REAL;
            break;
        case SQL_DOUBLE:
            bitmask = SQL_CVT_DOUBLE;
            break;
        case SQL_VARCHAR:
            bitmask = SQL_CVT_VARCHAR;
            break;
        case SQL_TYPE_DATE:
            bitmask = SQL_CVT_DATE;
            break;
        case SQL_TYPE_TIME:
            bitmask = SQL_CVT_TIME;
            break;
        case SQL_TYPE_TIMESTAMP:
            bitmask = SQL_CVT_TIMESTAMP;
            break;
        default:
            LOG_TRACE("Unsupported toType: {}, returning false", toType);
            return false;
    }
    // Get information from ODBC
    auto conversionInfo = getInfoSafely<SQLUINTEGER>(connection_.native_dbc_handle(), odbcInfoType, 0);
    bool result = (conversionInfo & bitmask) != 0;

    LOG_TRACE("Returning: {} (conversionInfo: 0x{:X}, bitmask: 0x{:X})",  result, conversionInfo, bitmask);
    return result;
}

bool DatabaseMetaData::supportsTableCorrelationNames() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUSMALLINT>(connection_.native_dbc_handle(), SQL_CORRELATION_NAME, 0);
    bool result = (val == SQL_CN_ANY || val == SQL_CN_DIFFERENT);
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsDifferentTableCorrelationNames() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUSMALLINT>(connection_.native_dbc_handle(), SQL_CORRELATION_NAME, 0);
    bool result = (val == SQL_CN_DIFFERENT);
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsNonNullableColumns() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUSMALLINT>(connection_.native_dbc_handle(), SQL_NON_NULLABLE_COLUMNS, 0);
    bool result = (val == SQL_NNC_NON_NULL);
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsMinimumSQLGrammar() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUSMALLINT>(connection_.native_dbc_handle(), SQL_ODBC_SQL_CONFORMANCE, 0);
    bool result = (val == SQL_OSC_MINIMUM || val == SQL_OSC_CORE || val == SQL_OSC_EXTENDED);
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsCoreSQLGrammar() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUSMALLINT>(connection_.native_dbc_handle(), SQL_ODBC_SQL_CONFORMANCE, 0);
    bool result = (val == SQL_OSC_CORE || val == SQL_OSC_EXTENDED);
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsExtendedSQLGrammar() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUSMALLINT>(connection_.native_dbc_handle(), SQL_ODBC_SQL_CONFORMANCE, 0);
    bool result = (val == SQL_OSC_EXTENDED);
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsANSI92EntryLevelSQL() const {
    LOG_TRACE("Called");
    LOG_TRACE("Returning: {}", true);
    return false;
}

bool DatabaseMetaData::supportsANSI92IntermediateSQL() const {
    LOG_TRACE("Called");
    LOG_TRACE("Returning: {}", false);
    return false;
}

bool DatabaseMetaData::supportsANSI92FullSQL() const {
    LOG_TRACE("Called");
    LOG_TRACE("Returning: {}", false);
    return false;
}

bool DatabaseMetaData::supportsDataDefinitionAndDataManipulationTransactions() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUSMALLINT>(connection_.native_dbc_handle(), SQL_TXN_CAPABLE, 0);
    bool result = (val & SQL_TC_ALL) != 0;
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsDataManipulationTransactionsOnly() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUSMALLINT>(connection_.native_dbc_handle(), SQL_TXN_CAPABLE, 0);
    bool result = (val & SQL_TC_DML) != 0;
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::dataDefinitionCausesTransactionCommit() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUSMALLINT>(connection_.native_dbc_handle(), SQL_TXN_CAPABLE, 0);
    bool result = (val & SQL_TC_DDL_COMMIT) != 0;
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::dataDefinitionIgnoredInTransactions() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUSMALLINT>(connection_.native_dbc_handle(), SQL_TXN_CAPABLE, 0);
    bool result = (val & SQL_TC_DDL_IGNORE) != 0;
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsMultipleOpenResults() const {
    LOG_TRACE("Called");
    LOG_TRACE("Returning: {}", false);
    return false;
}

bool DatabaseMetaData::supportsGetGeneratedKeys() const {
    LOG_TRACE("Called");
    LOG_TRACE("Returning: {}", false);
    return false;
}

bool DatabaseMetaData::doesMaxRowSizeIncludeBlobs() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<nanodbc::string>(connection_, SQL_MAX_ROW_SIZE_INCLUDES_LONG, NANODBC_TEXT(""));
    bool result = (val == NANODBC_TEXT("Y") || val == NANODBC_TEXT("y"));
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::isCatalogAtStart() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUSMALLINT>(connection_.native_dbc_handle(), SQL_CATALOG_LOCATION, 0);
    bool result = (val & SQL_CL_START) != 0;
    LOG_TRACE("Returning: {}", result);
    return result;
}

// === Целочисленные методы ===
int DatabaseMetaData::supportsTransactionIsolationLevel() const {
    LOG_TRACE("Called");
    SQLUINTEGER supported = 0;
    SQLRETURN ret = SQLGetInfo(connection_.native_dbc_handle(), SQL_TXN_ISOLATION_OPTION, &supported, 0, nullptr);
    int result = (ret == SQL_SUCCESS) ? static_cast<int>(supported) : 0x00000000;
    LOG_TRACE("Returning: 0x{:08X}", result);
    return result;
}

int DatabaseMetaData::getNullCollation() const {
    LOG_TRACE("Called");
    auto result = getInfoSafely<SQLUINTEGER>(connection_.native_dbc_handle(), SQL_NULL_COLLATION, 0);
    LOG_TRACE("Returning: {}", result);
    return result;
}

int DatabaseMetaData::getSQLStateType() const {
    LOG_TRACE("Called");
    auto result = getInfoSafely<SQLUINTEGER>(connection_.native_dbc_handle(), SQL_SQL_CONFORMANCE, 0);
    LOG_TRACE("Returning: {}", result);
    return result;
}

int DatabaseMetaData::getDefaultTransactionIsolation() const {
    LOG_TRACE("Called");
    auto result = getInfoSafely<SQLUINTEGER>(connection_.native_dbc_handle(), SQL_DEFAULT_TXN_ISOLATION, 0);
    LOG_TRACE("Returning: {}", result);
    return result;
}

int DatabaseMetaData::getResultSetHoldability() const {
    LOG_TRACE("Called");
    auto result = getInfoSafely<SQLUINTEGER>(connection_.native_dbc_handle(), SQL_GETDATA_EXTENSIONS, 0);
    LOG_TRACE("Returning: {}", result);
    return result;
}

int DatabaseMetaData::getRowIdLifetime() const {
    LOG_TRACE("Called");
    auto result = getInfoSafely<SQLUINTEGER>(connection_.native_dbc_handle(), SQL_ROW_UPDATES, 0);
    LOG_TRACE("Returning: {}", result);
    return result;
}

int DatabaseMetaData::getMaxTableNameLength() const {
    LOG_TRACE("Called");
    auto result = getInfoSafely<SQLUINTEGER>(connection_.native_dbc_handle(), SQL_MAX_TABLE_NAME_LEN, 0);
    LOG_TRACE("Returning: {}", result);
    return result;
}

int DatabaseMetaData::getMaxSchemaNameLength() const {
    LOG_TRACE("Called");
    auto result = getInfoSafely<SQLUINTEGER>(connection_.native_dbc_handle(), SQL_MAX_SCHEMA_NAME_LEN, 0);
    LOG_TRACE("Returning: {}", result);
    return result;
}

int DatabaseMetaData::getMaxCatalogNameLength() const {
    LOG_TRACE("Called");
    auto result = getInfoSafely<SQLUINTEGER>(connection_.native_dbc_handle(), SQL_MAX_CATALOG_NAME_LEN, 0);
    LOG_TRACE("Returning: {}", result);
    return result;
}

int DatabaseMetaData::getMaxColumnNameLength() const {
    LOG_TRACE("Called");
    auto result = getInfoSafely<SQLUINTEGER>(connection_.native_dbc_handle(), SQL_MAX_COLUMN_NAME_LEN, 0);
    LOG_TRACE("Returning: {}", result);
    return result;
}

int DatabaseMetaData::getMaxColumnsInGroupBy() const {
    LOG_TRACE("Called");
    auto result = getInfoSafely<SQLUINTEGER>(connection_.native_dbc_handle(), SQL_MAX_COLUMNS_IN_GROUP_BY, 0);
    LOG_TRACE("Returning: {}", result);
    return result;
}

int DatabaseMetaData::getMaxColumnsInOrderBy() const {
    LOG_TRACE("Called");
    auto result = getInfoSafely<SQLUINTEGER>(connection_.native_dbc_handle(), SQL_MAX_COLUMNS_IN_ORDER_BY, 0);
    LOG_TRACE("Returning: {}", result);
    return result;
}

int DatabaseMetaData::getMaxColumnsInSelect() const {
    LOG_TRACE("Called");
    auto result = getInfoSafely<SQLUINTEGER>(connection_.native_dbc_handle(), SQL_MAX_COLUMNS_IN_SELECT, 0);
    LOG_TRACE("Returning: {}", result);
    return result;
}

int DatabaseMetaData::getMaxColumnsInTable() const {
    LOG_TRACE("Called");
    auto result = getInfoSafely<SQLUINTEGER>(connection_.native_dbc_handle(), SQL_MAX_COLUMNS_IN_TABLE, 0);
    LOG_TRACE("Returning: {}", result);
    return result;
}

int DatabaseMetaData::getMaxColumnsInIndex() const {
    LOG_TRACE("Called");
    auto result = getInfoSafely<SQLUINTEGER>(connection_.native_dbc_handle(), SQL_MAX_COLUMNS_IN_INDEX, 0);
    LOG_TRACE("Returning: {}", result);
    return result;
}

int DatabaseMetaData::getMaxStatementLength() const {
    LOG_TRACE("Called");
    auto result = getInfoSafely<SQLUINTEGER>(connection_.native_dbc_handle(), SQL_MAX_STATEMENT_LEN, 0);
    LOG_TRACE("Returning: {}", result);
    return result;
}

int DatabaseMetaData::getMaxStatements() const {
    LOG_TRACE("Called");
    LOG_TRACE("Returning: 0 (неизвестно / не ограничено)");
    return 0;  // неизвестно / не ограничено
}

int DatabaseMetaData::getMaxTablesInSelect() const {
    LOG_TRACE("Called");
    auto result = getInfoSafely<SQLUINTEGER>(connection_.native_dbc_handle(), SQL_MAX_TABLES_IN_SELECT, 0);
    LOG_TRACE("Returning: {}", result);
    return result;
}

int DatabaseMetaData::getMaxUserNameLength() const {
    LOG_TRACE("Called");
    auto result = getInfoSafely<SQLUINTEGER>(connection_.native_dbc_handle(), SQL_MAX_USER_NAME_LEN, 0);
    LOG_TRACE("Returning: {}", result);
    return result;
}

int DatabaseMetaData::getMaxRowSize() const {
    LOG_TRACE("Called");
    auto result = getInfoSafely<SQLUINTEGER>(connection_.native_dbc_handle(), SQL_MAX_ROW_SIZE, 0);
    LOG_TRACE("Returning: {}", result);
    return result;
}

int DatabaseMetaData::getDatabaseMajorVersion() const {
    LOG_TRACE("Called");
    nanodbc::string ver = getDatabaseProductVersion();
    if (ver.empty()) {
        LOG_TRACE("Version string is empty, returning 0");
        return 0;
    }
    auto [major, minor] = processingVersionString(to_string(ver));
    return major;
}

int DatabaseMetaData::getDatabaseMinorVersion() const {
    LOG_TRACE("Called");
    nanodbc::string ver = getDatabaseProductVersion();
    if (ver.empty()) {
        LOG_TRACE("Version string is empty, returning 0");
        return 0;
    }
    auto [major, minor] = processingVersionString(to_string(ver));
    return minor;
}

int DatabaseMetaData::getDriverMajorVersion() const {
    LOG_TRACE("Called");
    nanodbc::string ver = getDriverVersion();
    if (ver.empty()) {
        LOG_TRACE("Driver version string is empty, returning 0");
        return 0;
    }
    auto [major, minor] = processingVersionString(to_string(ver));
    return major;    
}

int DatabaseMetaData::getDriverMinorVersion() const {
    LOG_TRACE("Called");
    nanodbc::string ver = getDriverVersion();
    if (ver.empty()) {
        LOG_TRACE("Driver version string is empty, returning 0");
        return 0;
    }
    auto [major, minor] = processingVersionString(to_string(ver));
    return minor;
}

int DatabaseMetaData::getMaxBinaryLiteralLength() const {
    LOG_TRACE("Called");
    auto result = getInfoSafely<SQLUINTEGER>(connection_.native_dbc_handle(), SQL_MAX_BINARY_LITERAL_LEN, 0);
    LOG_TRACE("Returning: {}", result);
    return result;
}

int DatabaseMetaData::getMaxCharLiteralLength() const {
    LOG_TRACE("Called");
    auto result = getInfoSafely<SQLUINTEGER>(connection_.native_dbc_handle(), SQL_MAX_CHAR_LITERAL_LEN, 0);
    LOG_TRACE("Returning: {}", result);
    return result;
}

int DatabaseMetaData::getMaxConnections() const {
    LOG_TRACE("Called");
    auto result = getInfoSafely<SQLUSMALLINT>(connection_.native_dbc_handle(), SQL_MAX_DRIVER_CONNECTIONS, 0);
    LOG_TRACE("Returning: {}", result);
    return result;
}

int DatabaseMetaData::getMaxCursorNameLength() const {
    LOG_TRACE("Called");
    auto result = getInfoSafely<SQLUINTEGER>(connection_.native_dbc_handle(), SQL_MAX_CURSOR_NAME_LEN, 0);
    LOG_TRACE("Returning: {}", result);
    return result;
}

int DatabaseMetaData::getMaxIndexLength() const {
    LOG_TRACE("Called");
    auto result = getInfoSafely<SQLUINTEGER>(connection_.native_dbc_handle(), SQL_MAX_INDEX_SIZE, 0);
    LOG_TRACE("Returning: {}", result);
    return result;
}

int DatabaseMetaData::getMaxProcedureNameLength() const {
    LOG_TRACE("Called");
    auto result = getInfoSafely<SQLUSMALLINT>(connection_.native_dbc_handle(), SQL_MAX_PROCEDURE_NAME_LEN, 0);
    LOG_TRACE("Returning: {}", result);
    return result;
}

// === Schemas ===
nanodbc::result DatabaseMetaData::getSchemas() const {
    return DatabaseCatalog(connection_).getSchemasResult();
}

// === Catalogs ===
nanodbc::result DatabaseMetaData::getCatalogs() const {
    return DatabaseCatalog(connection_).getCatalogsResult();
}

// === TableTypes ===
nanodbc::result DatabaseMetaData::getTableTypes() const {
    return DatabaseCatalog(connection_).getTableTypesResult();
}

// === Tables ===
nanodbc::result DatabaseMetaData::getTables(const nanodbc::string& catalog, const nanodbc::string& schema, const nanodbc::string& table, const nanodbc::string& type) const {
    auto tables_result = nanodbc::catalog(connection_).find_tables(table, type, schema, catalog);
    return DatabaseTables(tables_result).getResult();
}


// === Columns ===
nanodbc::result DatabaseMetaData::getColumns(const nanodbc::string& catalog, const nanodbc::string& schema, const nanodbc::string& table, const nanodbc::string& column) const {
    auto columns_result = nanodbc::catalog(connection_).find_columns(column, table, schema, catalog);
    return DatabaseColumns(columns_result).getResult();
}


// === Primary Keys ===
nanodbc::result DatabaseMetaData::getPrimaryKeys(const nanodbc::string& catalog, const nanodbc::string& schema,  const nanodbc::string& table) const {
    auto primary_keys_result = nanodbc::catalog(connection_).find_primary_keys(table, schema, catalog);
    return DatabasePrimaryKeys(primary_keys_result).getResult();
}

// === Imported Keys ===
nanodbc::result DatabaseMetaData::getImportedKeys(const nanodbc::string& catalog, const nanodbc::string& schema, const nanodbc::string& table) const {
    nanodbc::statement stmt(connection_);
    RETCODE rc;
    
    NANODBC_CALL_RC(
        NANODBC_FUNC(SQLForeignKeys),
        rc,
        stmt.native_statement_handle(),        
        nullptr, 0,
        nullptr, 0,
        nullptr, 0,
        (NANODBC_SQLCHAR*)(catalog.empty() ? NANODBC_TEXT("") : catalog.c_str()), SQL_NTS,
        (NANODBC_SQLCHAR*)(schema.empty() ? NANODBC_TEXT("") : schema.c_str()), SQL_NTS,
        (NANODBC_SQLCHAR*)(table.empty() ? NANODBC_TEXT("") : table.c_str()), SQL_NTS
    );

    if (!SQL_SUCCEEDED(rc))
        NANODBC_THROW_DATABASE_ERROR(stmt.native_statement_handle(), SQL_HANDLE_STMT);
    return ResultSet(std::move(stmt), 1);
}

// === Exported Keys ===
nanodbc::result DatabaseMetaData::getExportedKeys(const nanodbc::string& catalog, const nanodbc::string& schema, const nanodbc::string& table) const {
    nanodbc::statement stmt(connection_);
    RETCODE rc;

    NANODBC_CALL_RC(
        NANODBC_FUNC(SQLForeignKeys),
        rc,
        stmt.native_statement_handle(),
        (NANODBC_SQLCHAR*)(catalog.empty() ? NANODBC_TEXT("") : catalog.c_str()), SQL_NTS,
        (NANODBC_SQLCHAR*)(schema.empty() ? NANODBC_TEXT("") : schema.c_str()), SQL_NTS,
        (NANODBC_SQLCHAR*)(table.empty() ? NANODBC_TEXT("") : table.c_str()), SQL_NTS,
        nullptr, 0,
        nullptr, 0,
        nullptr, 0
    );

    if (!SQL_SUCCEEDED(rc))
        NANODBC_THROW_DATABASE_ERROR(stmt.native_statement_handle(), SQL_HANDLE_STMT);
    return ResultSet(std::move(stmt), 1);
}

// === TypeInfo ===
nanodbc::result DatabaseMetaData::getTypeInfo() const {
    nanodbc::statement stmt(connection_);
    RETCODE rc = NANODBC_FUNC(SQLGetTypeInfo)(
        stmt.native_statement_handle(),
        SQL_ALL_TYPES
    );
    if (!SQL_SUCCEEDED(rc))
        NANODBC_THROW_DATABASE_ERROR(stmt.native_statement_handle(), SQL_HANDLE_STMT);
    return ResultSet(std::move(stmt), 1);
}

nanodbc::result DatabaseMetaData::getColumnPrivileges(const nanodbc::string& catalog, const nanodbc::string& schema, const nanodbc::string& table, const nanodbc::string& columnNamePattern) const {
    LOG_TRACE("Called getColumnPrivileges({}, {}, {}, {})", to_string(catalog), to_string(schema), to_string(table), to_string(columnNamePattern));

    nanodbc::statement stmt(connection_);
    RETCODE rc = NANODBC_FUNC(SQLColumnPrivileges)(
        stmt.native_statement_handle(),
        (NANODBC_SQLCHAR*)(catalog.empty() ? NANODBC_TEXT("") : catalog.c_str()), SQL_NTS,
        (NANODBC_SQLCHAR*)(schema.empty() ? NANODBC_TEXT("") : schema.c_str()), SQL_NTS,
        (NANODBC_SQLCHAR*)(table.empty() ? NANODBC_TEXT("") : table.c_str()), SQL_NTS,
        (NANODBC_SQLCHAR*)(columnNamePattern.empty() ? NANODBC_TEXT("") : columnNamePattern.c_str()), SQL_NTS
        );

    if (!SQL_SUCCEEDED(rc))
        NANODBC_THROW_DATABASE_ERROR(stmt.native_statement_handle(), SQL_HANDLE_STMT);

    LOG_TRACE("Returning column privileges result set");
    return ResultSet(std::move(stmt), 1);
}

nanodbc::result DatabaseMetaData::getTablePrivileges(const nanodbc::string& catalog, const nanodbc::string& schemaPattern, const nanodbc::string& tableNamePattern) const {
    LOG_TRACE("Called getTablePrivileges({}, {}, {})", to_string(catalog), to_string(schemaPattern), to_string(tableNamePattern));

    nanodbc::statement stmt(connection_);
    RETCODE rc = NANODBC_FUNC(SQLTablePrivileges)(
        stmt.native_statement_handle(),
        (NANODBC_SQLCHAR*)(catalog.empty() ? NANODBC_TEXT("") : catalog.c_str()), SQL_NTS,
        (NANODBC_SQLCHAR*)(schemaPattern.empty() ? NANODBC_TEXT("") : schemaPattern.c_str()), SQL_NTS,
        (NANODBC_SQLCHAR*)(tableNamePattern.empty() ? NANODBC_TEXT("") : tableNamePattern.c_str()), SQL_NTS
        );

    if (!SQL_SUCCEEDED(rc))
        NANODBC_THROW_DATABASE_ERROR(stmt.native_statement_handle(), SQL_HANDLE_STMT);

    LOG_TRACE("Returning table privileges result set");
    return ResultSet(std::move(stmt), 1);
}

nanodbc::result DatabaseMetaData::getBestRowIdentifier(const nanodbc::string& catalog, const nanodbc::string& schema, const nanodbc::string& table, int scope, bool nullable) const {
    LOG_TRACE("Called getBestRowIdentifier({}, {}, {}, {}, {})", to_string(catalog), to_string(schema), to_string(table), scope, nullable);

    nanodbc::statement stmt(connection_);
    RETCODE rc = NANODBC_FUNC(SQLSpecialColumns)(
        stmt.native_statement_handle(),
        SQL_BEST_ROWID,
        (NANODBC_SQLCHAR*)(catalog.empty() ? NANODBC_TEXT("") : catalog.c_str()), SQL_NTS,
        (NANODBC_SQLCHAR*)(schema.empty() ? NANODBC_TEXT("") : schema.c_str()), SQL_NTS,
        (NANODBC_SQLCHAR*)(table.empty() ? NANODBC_TEXT("") : table.c_str()), SQL_NTS,
        static_cast<SQLUSMALLINT>(scope),
        static_cast<SQLUSMALLINT>(nullable ? SQL_TRUE : SQL_FALSE)
        );

    if (!SQL_SUCCEEDED(rc))
        NANODBC_THROW_DATABASE_ERROR(stmt.native_statement_handle(), SQL_HANDLE_STMT);

    LOG_TRACE("Returning best row identifier result set");
    return ResultSet(std::move(stmt), 1);
}

nanodbc::result DatabaseMetaData::getVersionColumns(const nanodbc::string& catalog, const nanodbc::string& schema, const nanodbc::string& table) const {
    LOG_TRACE("Called getVersionColumns({}, {}, {})", to_string(catalog), to_string(schema), to_string(table));

    nanodbc::statement stmt(connection_);
    RETCODE rc = NANODBC_FUNC(SQLSpecialColumns)(
        stmt.native_statement_handle(),
        SQL_ROWVER,
        (NANODBC_SQLCHAR*)(catalog.empty() ? NANODBC_TEXT("") : catalog.c_str()), SQL_NTS,
        (NANODBC_SQLCHAR*)(schema.empty() ? NANODBC_TEXT("") : schema.c_str()), SQL_NTS,
        (NANODBC_SQLCHAR*)(table.empty() ? NANODBC_TEXT("") : table.c_str()), SQL_NTS,
        0, // scope - not used for SQL_ROWVER
        SQL_FALSE // nullable - not used for SQL_ROWVER
        );

    if (!SQL_SUCCEEDED(rc))
        NANODBC_THROW_DATABASE_ERROR(stmt.native_statement_handle(), SQL_HANDLE_STMT);

    LOG_TRACE("Returning version columns result set");
    return ResultSet(std::move(stmt), 1);
}

nanodbc::result DatabaseMetaData::getCrossReference(const nanodbc::string& parentCatalog, const nanodbc::string& parentSchema, const nanodbc::string& parentTable,
                                                    const nanodbc::string& foreignCatalog, const nanodbc::string& foreignSchema, const nanodbc::string& foreignTable) const {
    LOG_TRACE("Called getCrossReference({}, {}, {}, {}, {}, {})",
        to_string(parentCatalog), to_string(parentSchema), to_string(parentTable), to_string(foreignCatalog), to_string(foreignSchema), to_string(foreignTable));

    nanodbc::statement stmt(connection_);
    RETCODE rc = NANODBC_FUNC(SQLForeignKeys)(
        stmt.native_statement_handle(),
        (NANODBC_SQLCHAR*)(parentCatalog.empty() ? NANODBC_TEXT("") : parentCatalog.c_str()), SQL_NTS,
        (NANODBC_SQLCHAR*)(parentSchema.empty() ? NANODBC_TEXT("") : parentSchema.c_str()), SQL_NTS,
        (NANODBC_SQLCHAR*)(parentTable.empty() ? NANODBC_TEXT("") : parentTable.c_str()), SQL_NTS,
        (NANODBC_SQLCHAR*)(foreignCatalog.empty() ? NANODBC_TEXT("") : foreignCatalog.c_str()), SQL_NTS,
        (NANODBC_SQLCHAR*)(foreignSchema.empty() ? NANODBC_TEXT("") : foreignSchema.c_str()), SQL_NTS,
        (NANODBC_SQLCHAR*)(foreignTable.empty() ? NANODBC_TEXT("") : foreignTable.c_str()), SQL_NTS
        );

    if (!SQL_SUCCEEDED(rc))
        NANODBC_THROW_DATABASE_ERROR(stmt.native_statement_handle(), SQL_HANDLE_STMT);

    LOG_TRACE("Returning cross reference result set");
    return ResultSet(std::move(stmt), 1);
}

nanodbc::result DatabaseMetaData::getIndexInfo(const nanodbc::string& catalog, const nanodbc::string& schema, const nanodbc::string& table, bool unique, bool approximate) const {
    LOG_TRACE("Called getIndexInfo({}, {}, {}, {}, {})", to_string(catalog), to_string(schema), to_string(table), unique, approximate);

    nanodbc::statement stmt(connection_);
    RETCODE rc = NANODBC_FUNC(SQLStatistics)(
        stmt.native_statement_handle(),
        (NANODBC_SQLCHAR*)(catalog.empty() ? NANODBC_TEXT("") : catalog.c_str()), SQL_NTS,
        (NANODBC_SQLCHAR*)(schema.empty() ? NANODBC_TEXT("") : schema.c_str()), SQL_NTS,
        (NANODBC_SQLCHAR*)(table.empty() ? NANODBC_TEXT("") : table.c_str()), SQL_NTS,
        static_cast<SQLUSMALLINT>(unique ? SQL_INDEX_UNIQUE : SQL_INDEX_ALL),
        static_cast<SQLUSMALLINT>(approximate ? SQL_QUICK : SQL_ENSURE)
        );

    if (!SQL_SUCCEEDED(rc))
        NANODBC_THROW_DATABASE_ERROR(stmt.native_statement_handle(), SQL_HANDLE_STMT);

    LOG_TRACE("Returning index info result set");
    return ResultSet(std::move(stmt), 1);
}

// === Procedures ===
nanodbc::result DatabaseMetaData::getProcedures(const nanodbc::string& catalog, const nanodbc::string& schema, const nanodbc::string& procedure) const {
    auto procedures_keys_result = nanodbc::catalog(connection_).find_procedures (procedure, schema, catalog);
    return DatabaseProcedures(procedures_keys_result).getResult();
}

// === Procedure Columns ===
nanodbc::result DatabaseMetaData::getProcedureColumns(const nanodbc::string& catalog, const nanodbc::string& schema, const nanodbc::string& procedure, const nanodbc::string& column) const {
    auto procedure_columns_result = nanodbc::catalog(connection_).find_procedure_columns (column, procedure, schema, catalog);
    return DatabaseProcedureColumns(procedure_columns_result).getResult();
}
