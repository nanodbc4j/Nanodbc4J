#include "core/database_metadata.hpp"
#include <string>
#include <algorithm>
#include <locale>
#include <codecvt>
#include <sstream>
#include <cwctype>
#include <sql.h>
#include "utils/logger.hpp"

#define BUFFER_SIZE 1024

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

    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
        LOG_ERROR("SQLGetInfo(attr={}) failed with return code {}", attr, ret);
    }

    return (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) ? value : defaultValue;
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

inline static std::pair<int, int> processingVersionString(std::wstring ver) {
    // Пропускаем не-цифры и не-знаки в начале строки
    auto it = ver.begin();
    while (it != ver.end() && !std::iswdigit(*it) && *it != L'+' && *it != L'-' && *it != L'.') {
        ++it;
    }

    if (it == ver.end()) {
        LOG_TRACE("No version info found; returning {{0, 0}}");
        return { 0, 0 };
    }
    // Если строка начинается с точки — значит, major = 0
    bool leadingDot = (*it == L'.');
    std::wstring cleaned = leadingDot ? (L"0" + std::wstring(it, ver.end())) : std::wstring(it, ver.end());
    std::wistringstream wiss(cleaned);

    int major = 0;
    wchar_t dot = 0;
    int minor = 0;

    // Пробуем прочитать major
    if (!(wiss >> major)) {
        LOG_TRACE("Failed to parse major version; returning {{0, 0}}");
        return { 0, 0 };
    }

    // Читаем точку и minor, если есть
    if (wiss.peek() == L'.') {
        wchar_t dot;
        wiss >> dot;  // Пропускаем точку

        if (!(wiss >> minor)) {
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
    , hdbc_(static_cast<SQLHDBC>(connection.native_dbc_handle()))
{
    LOG_TRACE("Called");
    if (!hdbc_) {
        LOG_TRACE("Invalid statement handle");
        throw std::runtime_error("Invalid statement handle");
    }
    LOG_TRACE("Initialized successfully");
}

// === Строковые методы ===

std::wstring DatabaseMetaData::getDatabaseProductName() const {
    LOG_TRACE_W("Called");
    auto result = connection_.dbms_name();
    LOG_TRACE_W(L"Returning: {}", result);
    return result;
}

std::wstring DatabaseMetaData::getDatabaseProductVersion() const {
    LOG_TRACE_W("Called");
    auto result = connection_.dbms_version();
    LOG_TRACE_W(L"Returning: {}", result);
    return result;
}

std::wstring DatabaseMetaData::getDriverName() const {
    LOG_TRACE_W("Called");
    auto result = connection_.driver_name();
    LOG_TRACE_W(L"Returning: {}", result);
    return result;
}

std::wstring DatabaseMetaData::getDriverVersion() const {
    LOG_TRACE_W("Called");
    auto result = connection_.driver_version();
    LOG_TRACE_W(L"Returning: {}", result);
    return result;
}

std::wstring DatabaseMetaData::getIdentifierQuoteString() const {
    LOG_TRACE_W("Called");
    auto result = getInfoSafely<nanodbc::string>(connection_, SQL_IDENTIFIER_QUOTE_CHAR);
    LOG_TRACE_W(L"Returning: {}", result);
    return result;
}

std::wstring DatabaseMetaData::getSchemaTerm() const {
    LOG_TRACE_W("Called");
    auto result = getInfoSafely<nanodbc::string>(connection_, SQL_SCHEMA_TERM);
    LOG_TRACE_W(L"Returning: {}", result);
    return result;
}

std::wstring DatabaseMetaData::getProcedureTerm() const {
    LOG_TRACE_W("Called");
    auto result = getInfoSafely<nanodbc::string>(connection_, SQL_PROCEDURE_TERM);
    LOG_TRACE_W(L"Returning: {}", result);
    return result;
}

std::wstring DatabaseMetaData::getCatalogTerm() const {
    LOG_TRACE_W("Called");
    auto result = getInfoSafely<nanodbc::string>(connection_, SQL_CATALOG_TERM);
    LOG_TRACE_W(L"Returning: {}", result);
    return result;
}

std::wstring DatabaseMetaData::getCatalogSeparator() const {
    LOG_TRACE_W("Called");
    auto result = getInfoSafely<nanodbc::string>(connection_, SQL_CATALOG_NAME_SEPARATOR);
    LOG_TRACE_W(L"Returning: {}", result);
    return result;
}

std::wstring DatabaseMetaData::getSQLKeywords() const {
    LOG_TRACE_W("Called");
    auto result = getInfoSafely<nanodbc::string>(connection_, SQL_KEYWORDS);
    LOG_TRACE_W(L"Returning: {}", result);
    return result;
}

std::wstring DatabaseMetaData::getNumericFunctions() const {
    LOG_TRACE("Called");
    SQLUINTEGER mask = 0;
    SQLRETURN ret = SQLGetInfo(hdbc_, SQL_NUMERIC_FUNCTIONS, &mask, sizeof(mask), nullptr);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
        LOG_ERROR("SQLGetInfo(SQL_NUMERIC_FUNCTIONS) failed: {}", ret);
        LOG_TRACE_W("Returning: L\"\"");
        return L"";
    }

    std::vector<std::wstring> funcs;

#define ADD_IF_SET(bit, name) if (mask & (bit)) funcs.push_back(L##name)

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

    std::wstring result = joinString<std::wstring>(funcs, L",");

    LOG_TRACE_W(L"Returning: {}", result);
    return result;
}

std::wstring DatabaseMetaData::getStringFunctions() const {
    LOG_TRACE("Called");
    SQLUINTEGER mask = 0;
    SQLRETURN ret = SQLGetInfo(hdbc_, SQL_STRING_FUNCTIONS, &mask, sizeof(mask), nullptr);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
        LOG_ERROR("SQLGetInfo(SQL_STRING_FUNCTIONS) failed: {}", ret);
        LOG_TRACE_W("Returning: L\"\"");
        return L"";
    }

    std::vector<std::wstring> funcs;

#define ADD_IF_SET(bit, name) if (mask & (bit)) funcs.push_back(L##name)

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

    std::wstring result = joinString<std::wstring>(funcs, L",");

    LOG_TRACE_W(L"Returning: {}", result);
    return result;
}

std::wstring DatabaseMetaData::getSystemFunctions() const {
    LOG_TRACE("Called");
    SQLUINTEGER mask = 0;
    SQLRETURN ret = SQLGetInfo(hdbc_, SQL_SYSTEM_FUNCTIONS, &mask, sizeof(mask), nullptr);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
        LOG_ERROR("SQLGetInfo(SQL_SYSTEM_FUNCTIONS) failed: {}", ret);
        LOG_TRACE_W("Returning: L\"\"");
        return L"";
    }

    std::vector<std::wstring> funcs;

#define ADD_IF_SET(bit, name) if (mask & (bit)) funcs.push_back(L##name)

    ADD_IF_SET(SQL_FN_SYS_DBNAME, "DBNAME");
    ADD_IF_SET(SQL_FN_SYS_IFNULL, "IFNULL");
    ADD_IF_SET(SQL_FN_SYS_USERNAME, "USERNAME");

#undef ADD_IF_SET

    std::wstring result = joinString<std::wstring>(funcs, L",");

    LOG_TRACE_W(L"Returning: {}", result);
    return result;
}

std::wstring DatabaseMetaData::getTimeDateFunctions() const {
    LOG_TRACE("Called");
    SQLUINTEGER mask = 0;
    SQLRETURN ret = SQLGetInfo(hdbc_, SQL_TIMEDATE_FUNCTIONS, &mask, sizeof(mask), nullptr);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
        LOG_ERROR("SQLGetInfo(SQL_TIMEDATE_FUNCTIONS) failed: {}", ret);
        LOG_TRACE_W("Returning: L\"\"");
        return L"";
    }

    std::vector<std::wstring> funcs;

#define ADD_IF_SET(bit, name) if (mask & (bit)) funcs.push_back(L##name)

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

    std::wstring result = joinString<std::wstring>(funcs, L",");

    LOG_TRACE_W(L"Returning: {}", result);
    return result;
}

std::wstring DatabaseMetaData::getSearchStringEscape() const {
    LOG_TRACE_W("Called");
    auto result = getInfoSafely<nanodbc::string>(connection_, SQL_SEARCH_PATTERN_ESCAPE);
    LOG_TRACE_W(L"Returning: {}", result);
    return result;
}

std::wstring DatabaseMetaData::getExtraNameCharacters() const {
    LOG_TRACE_W("Called");
    auto result = getInfoSafely<nanodbc::string>(connection_, SQL_SPECIAL_CHARACTERS);
    LOG_TRACE_W(L"Returning: {}", result);
    return result;
}

// === Булевы методы ===

bool DatabaseMetaData::isReadOnly() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(hdbc_, SQL_DATA_SOURCE_READ_ONLY, SQL_FALSE);
    bool result = (val == SQL_TRUE);
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsTransactions() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(hdbc_, SQL_TXN_CAPABLE, SQL_TC_NONE);
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
    auto val = getInfoSafely<SQLUINTEGER>(hdbc_, SQL_PARAM_ARRAY_ROW_COUNTS, 0);
    bool result = (val == 0x0001); // 0x0001 = SQL_PARC_YES_ROW_COUNTS
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsUnion() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(hdbc_, SQL_UNION, SQL_FALSE);
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
    auto val = getInfoSafely<SQLUINTEGER>(hdbc_, SQL_LIKE_ESCAPE_CLAUSE, SQL_FALSE);
    bool result = (val == SQL_TRUE);
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsGroupBy() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(hdbc_, SQL_GROUP_BY, SQL_GB_NOT_SUPPORTED);
    bool result = (val != SQL_GB_NOT_SUPPORTED);
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsGroupByUnrelated() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(hdbc_, SQL_GROUP_BY, SQL_GB_NOT_SUPPORTED);
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
    auto val = getInfoSafely<SQLUINTEGER>(hdbc_, SQL_ORDER_BY_COLUMNS_IN_SELECT, SQL_TRUE);
    bool result = (val == SQL_FALSE);
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsAlterTableWithAddColumn() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(hdbc_, SQL_ALTER_TABLE, 0);
    bool result = (val & SQL_AT_ADD_COLUMN) != 0;
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsColumnAliasing() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(hdbc_, SQL_COLUMN_ALIAS, SQL_FALSE);
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
    auto val = getInfoSafely<SQLUINTEGER>(hdbc_, 125 /* SQL_FOR_UPDATE */, SQL_FALSE);
    bool result = (val == SQL_TRUE);
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsStoredProcedures() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(hdbc_, SQL_PROCEDURES, SQL_FALSE);
    bool result = (val == SQL_TRUE);
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsSubqueriesInComparisons() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(hdbc_, SQL_SUBQUERIES, 0);
    bool result = (val & SQL_SQ_COMPARISON) != 0;
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsSubqueriesInExists() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(hdbc_, SQL_SUBQUERIES, 0);
    bool result = (val & SQL_SQ_EXISTS) != 0;
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsSubqueriesInIns() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(hdbc_, SQL_SUBQUERIES, 0);
    bool result = (val & SQL_SQ_IN) != 0;
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsSubqueriesInQuantifieds() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(hdbc_, SQL_SUBQUERIES, 0);
    bool result = (val & SQL_SQ_QUANTIFIED) != 0;
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsCorrelatedSubqueries() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(hdbc_, SQL_SUBQUERIES, 0);
    bool result = (val & 0x00000010) != 0; // 0x00000010 = SQL_SQ_CORRELATED
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsIntegrityEnhancementFacility() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(hdbc_, SQL_INTEGRITY, SQL_FALSE);
    bool result = (val == SQL_TRUE);
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsOuterJoins() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(hdbc_, SQL_OUTER_JOINS, SQL_FALSE);
    bool result = (val == SQL_TRUE);
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsFullOuterJoins() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(hdbc_, SQL_OUTER_JOINS, 0);
    bool result = (val & 0x00000008) != 0; // SQL_OJ_FULL_OUTER_JOIN = 0x00000008
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsLimitedOuterJoins() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(hdbc_, SQL_OUTER_JOINS, 0);
    bool result = (val == SQL_TRUE);
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsSchemasInDataManipulation() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(hdbc_, SQL_SCHEMA_USAGE, 0);
    bool result = (val & SQL_SU_DML_STATEMENTS) != 0;
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsSchemasInProcedureCalls() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(hdbc_, SQL_SCHEMA_USAGE, 0);
    bool result = (val & SQL_SU_PROCEDURE_INVOCATION) != 0;
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsSchemasInTableDefinitions() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(hdbc_, SQL_SCHEMA_USAGE, 0);
    bool result = (val & SQL_SU_TABLE_DEFINITION) != 0;
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsSchemasInIndexDefinitions() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(hdbc_, SQL_SCHEMA_USAGE);
    bool result = (val & SQL_SU_INDEX_DEFINITION) != 0;
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsSchemasInPrivilegeDefinitions() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(hdbc_, SQL_SCHEMA_USAGE);
    bool result = (val & SQL_SU_PRIVILEGE_DEFINITION) != 0;
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsCatalogsInDataManipulation() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(hdbc_, SQL_CATALOG_USAGE, 0);
    bool result = (val & SQL_CU_DML_STATEMENTS) != 0;
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsCatalogsInProcedureCalls() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(hdbc_, SQL_CATALOG_USAGE, 0);
    bool result = (val & SQL_CU_PROCEDURE_INVOCATION) != 0;
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsCatalogsInTableDefinitions() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(hdbc_, SQL_CATALOG_USAGE, 0);
    bool result = (val & SQL_CU_TABLE_DEFINITION) != 0;
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsCatalogsInIndexDefinitions() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(hdbc_, SQL_CATALOG_USAGE, 0);
    bool result = (val & SQL_CU_INDEX_DEFINITION) != 0;
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsCatalogsInPrivilegeDefinitions() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(hdbc_, SQL_CATALOG_USAGE, 0);
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
    auto val = getInfoSafely<SQLUINTEGER>(hdbc_, SQL_CURSOR_COMMIT_BEHAVIOR, SQL_CB_CLOSE);
    bool result = (val == SQL_CB_PRESERVE);
    LOG_TRACE("Returning: {}", result);
    return result;
}

bool DatabaseMetaData::supportsOpenCursorsAcrossRollback() const {
    LOG_TRACE("Called");
    auto val = getInfoSafely<SQLUINTEGER>(hdbc_, SQL_CURSOR_ROLLBACK_BEHAVIOR, SQL_CB_CLOSE);
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

bool DatabaseMetaData::autoCommitFailureClosesAllResultSets() const {
    LOG_TRACE("Called");
    // SQL_MULT_RESULT_SETS: если поддерживает множественные результаты
    auto val = getInfoSafely<SQLUINTEGER>(hdbc_, SQL_MULT_RESULT_SETS, SQL_FALSE);
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
    auto val = getInfoSafely<SQLUINTEGER>(hdbc_, SQL_GETDATA_EXTENSIONS, 0);
    bool result = (val & SQL_GD_ANY_COLUMN) != 0;
    LOG_TRACE("Returning: {}", result);
    return result;
}

// === Целочисленные методы ===
int DatabaseMetaData::supportsTransactionIsolationLevel() const {   // === Поддержка уровней изоляции ===
    LOG_TRACE("Called");
    SQLUINTEGER supported = 0;
    SQLRETURN ret = SQLGetInfo(hdbc_, SQL_TXN_ISOLATION_OPTION, &supported, 0, nullptr);
    int result = (ret == SQL_SUCCESS) ? static_cast<int>(supported) : 0x00000000;
    LOG_TRACE("Returning: 0x{:08X}", result);
    return result;
}

int DatabaseMetaData::getNullCollation() const {
    LOG_TRACE("Called");
    auto result = getInfoSafely<SQLUINTEGER>(hdbc_, SQL_NULL_COLLATION, 0);
    LOG_TRACE("Returning: {}", result);
    return result;
}

int DatabaseMetaData::getSQLStateType() const {
    LOG_TRACE("Called");
    auto result = getInfoSafely<SQLUINTEGER>(hdbc_, SQL_SQL_CONFORMANCE, 0);
    LOG_TRACE("Returning: {}", result);
    return result;
}

int DatabaseMetaData::getDefaultTransactionIsolation() const {
    LOG_TRACE("Called");
    auto result = getInfoSafely<SQLUINTEGER>(hdbc_, SQL_DEFAULT_TXN_ISOLATION, 0);
    LOG_TRACE("Returning: {}", result);
    return result;
}

int DatabaseMetaData::getResultSetHoldability() const {
    LOG_TRACE("Called");
    auto result = getInfoSafely<SQLUINTEGER>(hdbc_, SQL_GETDATA_EXTENSIONS, 0);
    LOG_TRACE("Returning: {}", result);
    return result;
}

int DatabaseMetaData::getRowIdLifetime() const {
    LOG_TRACE("Called");
    auto result = getInfoSafely<SQLUINTEGER>(hdbc_, SQL_ROW_UPDATES, 0);
    LOG_TRACE("Returning: {}", result);
    return result;
}

int DatabaseMetaData::getMaxTableNameLength() const {
    LOG_TRACE("Called");
    auto result = getInfoSafely<SQLUINTEGER>(hdbc_, SQL_MAX_TABLE_NAME_LEN, 0);
    LOG_TRACE("Returning: {}", result);
    return result;
}

int DatabaseMetaData::getMaxSchemaNameLength() const {
    LOG_TRACE("Called");
    auto result = getInfoSafely<SQLUINTEGER>(hdbc_, SQL_MAX_SCHEMA_NAME_LEN, 0);
    LOG_TRACE("Returning: {}", result);
    return result;
}

int DatabaseMetaData::getMaxCatalogNameLength() const {
    LOG_TRACE("Called");
    auto result = getInfoSafely<SQLUINTEGER>(hdbc_, SQL_MAX_CATALOG_NAME_LEN, 0);
    LOG_TRACE("Returning: {}", result);
    return result;
}

int DatabaseMetaData::getMaxColumnNameLength() const {
    LOG_TRACE("Called");
    auto result = getInfoSafely<SQLUINTEGER>(hdbc_, SQL_MAX_COLUMN_NAME_LEN, 0);
    LOG_TRACE("Returning: {}", result);
    return result;
}

int DatabaseMetaData::getMaxColumnsInGroupBy() const {
    LOG_TRACE("Called");
    auto result = getInfoSafely<SQLUINTEGER>(hdbc_, SQL_MAX_COLUMNS_IN_GROUP_BY, 0);
    LOG_TRACE("Returning: {}", result);
    return result;
}

int DatabaseMetaData::getMaxColumnsInOrderBy() const {
    LOG_TRACE("Called");
    auto result = getInfoSafely<SQLUINTEGER>(hdbc_, SQL_MAX_COLUMNS_IN_ORDER_BY, 0);
    LOG_TRACE("Returning: {}", result);
    return result;
}

int DatabaseMetaData::getMaxColumnsInSelect() const {
    LOG_TRACE("Called");
    auto result = getInfoSafely<SQLUINTEGER>(hdbc_, SQL_MAX_COLUMNS_IN_SELECT, 0);
    LOG_TRACE("Returning: {}", result);
    return result;
}

int DatabaseMetaData::getMaxColumnsInTable() const {
    LOG_TRACE("Called");
    auto result = getInfoSafely<SQLUINTEGER>(hdbc_, SQL_MAX_COLUMNS_IN_TABLE, 0);
    LOG_TRACE("Returning: {}", result);
    return result;
}

int DatabaseMetaData::getMaxColumnsInIndex() const {
    LOG_TRACE("Called");
    auto result = getInfoSafely<SQLUINTEGER>(hdbc_, SQL_MAX_COLUMNS_IN_INDEX, 0);
    LOG_TRACE("Returning: {}", result);
    return result;
}

int DatabaseMetaData::getMaxStatementLength() const {
    LOG_TRACE("Called");
    auto result = getInfoSafely<SQLUINTEGER>(hdbc_, SQL_MAX_STATEMENT_LEN, 0);
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
    auto result = getInfoSafely<SQLUINTEGER>(hdbc_, SQL_MAX_TABLES_IN_SELECT, 0);
    LOG_TRACE("Returning: {}", result);
    return result;
}

int DatabaseMetaData::getMaxUserNameLength() const {
    LOG_TRACE("Called");
    auto result = getInfoSafely<SQLUINTEGER>(hdbc_, SQL_MAX_USER_NAME_LEN, 0);
    LOG_TRACE("Returning: {}", result);
    return result;
}

int DatabaseMetaData::getMaxRowSize() const {
    LOG_TRACE("Called");
    auto result = getInfoSafely<SQLUINTEGER>(hdbc_, SQL_MAX_ROW_SIZE, 0);
    LOG_TRACE("Returning: {}", result);
    return result;
}

int DatabaseMetaData::getDatabaseMajorVersion() const {
    LOG_TRACE("Called");
    std::wstring ver = getDatabaseProductVersion();
    if (ver.empty()) {
        LOG_TRACE("Version string is empty, returning 0");
        return 0;
    }
    auto [major, minor] = processingVersionString(ver);
    return major;
}

int DatabaseMetaData::getDatabaseMinorVersion() const {
    LOG_TRACE("Called");
    std::wstring ver = getDatabaseProductVersion();
    if (ver.empty()) {
        LOG_TRACE("Version string is empty, returning 0");
        return 0;
    }
    auto [major, minor] = processingVersionString(ver);
    return minor;
}

int DatabaseMetaData::getDriverMajorVersion() const {
    LOG_TRACE("Called");
    std::wstring ver = getDriverVersion();
    if (ver.empty()) {
        LOG_TRACE("Driver version string is empty, returning 0");
        return 0;
    }
    auto [major, minor] = processingVersionString(ver);
    return major;    
}

int DatabaseMetaData::getDriverMinorVersion() const {
    LOG_TRACE("Called");
    std::wstring ver = getDriverVersion();
    if (ver.empty()) {
        LOG_TRACE("Driver version string is empty, returning 0");
        return 0;
    }
    auto [major, minor] = processingVersionString(ver);
    return minor;
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
nanodbc::result DatabaseMetaData::getTables(const std::wstring& catalog, const std::wstring& schema,
        const std::wstring& table, const std::wstring& type) const {
    auto tables_result = nanodbc::catalog(connection_).find_tables(table, type, schema, catalog);
    return DatabaseTables(tables_result).getResult();
}


// === Columns ===
nanodbc::result DatabaseMetaData::getColumns(const std::wstring& catalog, const std::wstring& schema,
        const std::wstring& table, const std::wstring& column) const {
    auto columns_result = nanodbc::catalog(connection_).find_columns(column, table, schema, catalog);
    return DatabaseColumns(columns_result).getResult();
}


// === Primary Keys ===
nanodbc::result DatabaseMetaData::getPrimaryKeys(const std::wstring& catalog, const std::wstring& schema,
        const std::wstring& table) const {
    auto primary_keys_result = nanodbc::catalog(connection_).find_primary_keys(table, schema, catalog);
    return DatabasePrimaryKeys(primary_keys_result).getResult();
}

nanodbc::result DatabaseMetaData::getImportedKeys(const std::wstring& catalog, const std::wstring& schema,
    const std::wstring& table) const {
    nanodbc::statement stmt(connection_);

    return {};
}

nanodbc::result DatabaseMetaData::getTypeInfo(short sqlType) const {
    return {};
}

// === Procedures ===
nanodbc::result DatabaseMetaData::getProcedures(const std::wstring& catalog, const std::wstring& schema,
    const std::wstring& procedure) const {
    auto procedures_keys_result = nanodbc::catalog(connection_).find_procedures (procedure, schema, catalog);
    return DatabaseProcedures(procedures_keys_result).getResult();
}

// === Procedure Columns ===
nanodbc::result DatabaseMetaData::getProcedureColumns(const std::wstring& catalog, const std::wstring& schema,
    const std::wstring& procedure, const std::wstring& column) const {
    auto procedure_columns_result = nanodbc::catalog(connection_).find_procedure_columns (column, procedure, schema, catalog);
    return DatabaseProcedureColumns(procedure_columns_result).getResult();
}
