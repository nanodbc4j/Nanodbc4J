#include "core/database_metadata.hpp"
#include <string>
#include <algorithm>
#include <locale>
#include <codecvt>
#include <sql.h>

#define BUFFER_SIZE 1024

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
inline T getInfoSafely(SQLHDBC hdbc, SQLUSMALLINT attr, T defaultValue = T{}) {
    // Статическая проверка: T должен быть числовым типом
    static_assert(
        std::is_arithmetic_v<T>,
        "T must be a numeric type (int, short, SQLUSMALLINT, etc.)"
        );

    T value = 0;
    SQLRETURN ret = SQLGetInfo(hdbc, attr, &value, 0, nullptr);
    return (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) ? value : defaultValue;
}

// === Конструктор ===
DatabaseMetaData::DatabaseMetaData(const nanodbc::connection& connection) 
    : connection_ (connection)
    , hdbc_ (static_cast<SQLHSTMT>(connection.native_dbc_handle()))
{
    if (!hdbc_) {
        throw std::runtime_error("Invalid statement handle");
    }
}

// === Строковые методы ===

std::wstring DatabaseMetaData::getDatabaseProductName() const {
    return connection_.dbms_name();
}

std::wstring DatabaseMetaData::getDatabaseProductVersion() const {
    return connection_.dbms_version();
}

std::wstring DatabaseMetaData::getDriverName() const {
    return connection_.driver_name();
}

std::wstring DatabaseMetaData::getDriverVersion() const {
    return connection_.driver_version();
}

std::wstring DatabaseMetaData::getIdentifierQuoteString() const {
    return getInfoSafely<nanodbc::string>(connection_, SQL_IDENTIFIER_QUOTE_CHAR);
}

std::wstring DatabaseMetaData::getSchemaTerm() const {
    return getInfoSafely<nanodbc::string>(connection_, SQL_SCHEMA_TERM);
}

std::wstring DatabaseMetaData::getProcedureTerm() const {
    return getInfoSafely<nanodbc::string>(connection_, SQL_PROCEDURE_TERM);
}

std::wstring DatabaseMetaData::getCatalogTerm() const {
    return getInfoSafely<nanodbc::string>(connection_, SQL_CATALOG_TERM);
}

std::wstring DatabaseMetaData::getCatalogSeparator() const {
    return getInfoSafely<nanodbc::string>(connection_, SQL_CATALOG_NAME_SEPARATOR);
}

std::wstring DatabaseMetaData::getSQLKeywords() const {
    return getInfoSafely<nanodbc::string>(connection_, SQL_KEYWORDS);
}

std::wstring DatabaseMetaData::getNumericFunctions() const {
    return getInfoSafely<nanodbc::string>(connection_, SQL_NUMERIC_FUNCTIONS);
}

std::wstring DatabaseMetaData::getStringFunctions() const {
    return getInfoSafely<nanodbc::string>(connection_, SQL_STRING_FUNCTIONS);
}

std::wstring DatabaseMetaData::getSystemFunctions() const {
    return getInfoSafely<nanodbc::string>(connection_, SQL_SYSTEM_FUNCTIONS);
}

std::wstring DatabaseMetaData::getTimeDateFunctions() const {
    return getInfoSafely<nanodbc::string>(connection_, SQL_TIMEDATE_FUNCTIONS);
}

std::wstring DatabaseMetaData::getSearchStringEscape() const {
    return getInfoSafely<nanodbc::string>(connection_, SQL_SEARCH_PATTERN_ESCAPE);
}

std::wstring DatabaseMetaData::getExtraNameCharacters() const {
    return getInfoSafely<nanodbc::string>(connection_, SQL_SPECIAL_CHARACTERS);
}

// === Булевы методы ===

bool DatabaseMetaData::isReadOnly() const {
    auto val = getInfoSafely<SQLUSMALLINT>(hdbc_, SQL_DATA_SOURCE_READ_ONLY, SQL_FALSE);
    return val == SQL_TRUE;
}

bool DatabaseMetaData::supportsTransactions() const {
    auto val = getInfoSafely<SQLUSMALLINT>(hdbc_, SQL_TXN_CAPABLE, SQL_TC_NONE);
    return val != SQL_TC_NONE;
}

bool DatabaseMetaData::supportsSavepoints() const {
    return false;  // Savepoints — это JDBC-фича, в ODBC нет прямого аналога.
}

bool DatabaseMetaData::supportsNamedParameters() const {
    return false;  // ODBC не поддерживает именованные параметры по стандарту
}

bool DatabaseMetaData::supportsBatchUpdates() const {
    auto val = getInfoSafely<SQLUINTEGER>(hdbc_, SQL_PARAM_ARRAY_ROW_COUNTS, 0);
    return val == 0x0001; // 0x0001 = SQL_PARC_YES_ROW_COUNTS
}

bool DatabaseMetaData::supportsUnion() const {
    auto val = getInfoSafely<SQLUSMALLINT>(hdbc_, SQL_UNION, SQL_FALSE);
    return val == SQL_TRUE;
}

bool DatabaseMetaData::supportsUnionAll() const {
    // SQL_UNION_ALL не определён в некоторых SDK, но
    // если SQL_UNION == SQL_TRUE, то UNION ALL тоже поддерживается
    return supportsUnion();
}

bool DatabaseMetaData::supportsLikeEscapeClause() const {
    auto val = getInfoSafely<SQLUSMALLINT>(hdbc_, SQL_LIKE_ESCAPE_CLAUSE, SQL_FALSE);
    return val == SQL_TRUE;
}

bool DatabaseMetaData::supportsGroupBy() const {
    auto val = getInfoSafely<SQLUSMALLINT>(hdbc_, SQL_GROUP_BY, SQL_GB_NOT_SUPPORTED);
    return val != SQL_GB_NOT_SUPPORTED;
}

bool DatabaseMetaData::supportsGroupByUnrelated() const {
    auto val = getInfoSafely<SQLUSMALLINT>(hdbc_, SQL_GROUP_BY, SQL_GB_NOT_SUPPORTED);
    return val == SQL_GB_COLLATE;
}

bool DatabaseMetaData::supportsGroupByBeyondSelect() const {
    return supportsGroupByUnrelated(); // обычно то же самое
}

bool DatabaseMetaData::supportsOrderByUnrelated() const {
    auto val = getInfoSafely<SQLUSMALLINT>(hdbc_, SQL_ORDER_BY_COLUMNS_IN_SELECT, SQL_TRUE);
    return val == SQL_FALSE;
}

bool DatabaseMetaData::supportsAlterTableWithAddColumn() const {
    auto val = getInfoSafely<SQLUSMALLINT>(hdbc_, SQL_ALTER_TABLE, 0);
    return (val & SQL_AT_ADD_COLUMN) != 0;
}

bool DatabaseMetaData::supportsColumnAliasing() const {
    auto val = getInfoSafely<SQLUSMALLINT>(hdbc_, SQL_COLUMN_ALIAS, SQL_FALSE);
    return val == SQL_TRUE;
}

bool DatabaseMetaData::nullPlusNonNullIsNull() const {
    // SQL_NULL_PLUS_NULL может не быть
    return false;  // по умолчанию — неизвестно
}

bool DatabaseMetaData::supportsExpressionsInOrderBy() const {
    // SQL_EXPRESSIONS_IN_ORDER_BY может не быть
    return true;  // большинство современных СУБД поддерживают (PostgreSQL, MySQL, SQL Server)
}

bool DatabaseMetaData::supportsSelectForUpdate() const {
    auto val = getInfoSafely<SQLUSMALLINT>(hdbc_, 125 /* SQL_FOR_UPDATE */, SQL_FALSE);
    return val == SQL_TRUE;
}

bool DatabaseMetaData::supportsStoredProcedures() const {
    auto val = getInfoSafely<SQLUSMALLINT>(hdbc_, SQL_PROCEDURES, SQL_FALSE);
    return val == SQL_TRUE;
}

bool DatabaseMetaData::supportsSubqueriesInComparisons() const {
    auto val = getInfoSafely<SQLUSMALLINT>(hdbc_, SQL_SUBQUERIES, 0);
    return (val & SQL_SQ_COMPARISON) != 0;
}

bool DatabaseMetaData::supportsSubqueriesInExists() const {
    auto val = getInfoSafely<SQLUSMALLINT>(hdbc_, SQL_SUBQUERIES, 0);
    return (val & SQL_SQ_EXISTS) != 0;
}

bool DatabaseMetaData::supportsSubqueriesInIns() const {
    auto val = getInfoSafely<SQLUSMALLINT>(hdbc_, SQL_SUBQUERIES, 0);
    return (val & SQL_SQ_IN) != 0;
}

bool DatabaseMetaData::supportsSubqueriesInQuantifieds() const {
    auto val = getInfoSafely<SQLUSMALLINT>(hdbc_, SQL_SUBQUERIES, 0);
    return (val & SQL_SQ_QUANTIFIED) != 0;
}

bool DatabaseMetaData::supportsCorrelatedSubqueries() const {
    auto val = getInfoSafely<SQLUSMALLINT>(hdbc_, SQL_SUBQUERIES, 0);
    return (val & 0x00000010) != 0; // 0x00000010 = SQL_SQ_CORRELATED
}

bool DatabaseMetaData::supportsIntegrityEnhancementFacility() const {
    auto val = getInfoSafely<SQLUSMALLINT>(hdbc_, SQL_INTEGRITY, SQL_FALSE);
    return val == SQL_TRUE;
}

bool DatabaseMetaData::supportsOuterJoins() const {
    auto val = getInfoSafely<SQLUSMALLINT>(hdbc_, SQL_OUTER_JOINS, SQL_FALSE);
    return val == SQL_TRUE;
}

bool DatabaseMetaData::supportsFullOuterJoins() const {
    auto val = getInfoSafely<SQLUSMALLINT>(hdbc_, SQL_OUTER_JOINS, 0);
    return (val & 0x00000008) != 0; // SQL_OJ_FULL_OUTER_JOIN = 0x00000008
}

bool DatabaseMetaData::supportsSchemasInDataManipulation() const {
    auto val = getInfoSafely<SQLUSMALLINT>(hdbc_, SQL_SCHEMA_USAGE, 0);
    return (val & SQL_SU_DML_STATEMENTS) != 0;
}

bool DatabaseMetaData::supportsSchemasInProcedureCalls() const {
    auto val = getInfoSafely<SQLUSMALLINT>(hdbc_, SQL_SCHEMA_USAGE, 0);
    return (val & SQL_SU_PROCEDURE_INVOCATION) != 0;
}

bool DatabaseMetaData::supportsSchemasInTableDefinitions() const {
    auto val = getInfoSafely<SQLUSMALLINT>(hdbc_, SQL_SCHEMA_USAGE, 0);
    return (val & SQL_SU_TABLE_DEFINITION) != 0;
}

bool DatabaseMetaData::supportsSchemasInIndexDefinitions() const {
    auto val = getInfoSafely<SQLUSMALLINT>(hdbc_, SQL_SCHEMA_USAGE);
    return (val & SQL_SU_INDEX_DEFINITION) != 0;
}

bool DatabaseMetaData::supportsSchemasInPrivilegeDefinitions() const {
    auto val = getInfoSafely<SQLUSMALLINT>(hdbc_, SQL_SCHEMA_USAGE);
    return (val & SQL_SU_PRIVILEGE_DEFINITION) != 0;
}

bool DatabaseMetaData::supportsCatalogsInDataManipulation() const {
    auto val = getInfoSafely<SQLUSMALLINT>(hdbc_, SQL_CATALOG_USAGE, 0);
    return (val & SQL_CU_DML_STATEMENTS) != 0;
}

bool DatabaseMetaData::supportsCatalogsInProcedureCalls() const {
    auto val = getInfoSafely<SQLUSMALLINT>(hdbc_, SQL_CATALOG_USAGE, 0);
    return (val & SQL_CU_PROCEDURE_INVOCATION) != 0;
}

bool DatabaseMetaData::supportsCatalogsInTableDefinitions() const {
    auto val = getInfoSafely<SQLUSMALLINT>(hdbc_, SQL_CATALOG_USAGE, 0);
    return (val & SQL_CU_TABLE_DEFINITION) != 0;
}

bool DatabaseMetaData::supportsCatalogsInIndexDefinitions() const {
    auto val = getInfoSafely<SQLUSMALLINT>(hdbc_, SQL_CATALOG_USAGE, 0);
    return (val & SQL_CU_INDEX_DEFINITION) != 0;
}

bool DatabaseMetaData::supportsCatalogsInPrivilegeDefinitions() const {
    auto val = getInfoSafely<SQLUSMALLINT>(hdbc_, SQL_CATALOG_USAGE, 0);
    return (val & SQL_CU_PRIVILEGE_DEFINITION) != 0;
}

bool DatabaseMetaData::supportsPositionedDelete() const {
    return false;
}

bool DatabaseMetaData::supportsPositionedUpdate() const {
    return false;
}

bool DatabaseMetaData::supportsOpenCursorsAcrossCommit() const {
    auto val = getInfoSafely<SQLUSMALLINT>(hdbc_, SQL_CURSOR_COMMIT_BEHAVIOR, SQL_CB_CLOSE);
    return val == SQL_CB_PRESERVE;
}

bool DatabaseMetaData::supportsOpenCursorsAcrossRollback() const {
    auto val = getInfoSafely<SQLUSMALLINT>(hdbc_, SQL_CURSOR_ROLLBACK_BEHAVIOR, SQL_CB_CLOSE);
    return val == SQL_CB_PRESERVE;
}

bool DatabaseMetaData::supportsOpenStatementsAcrossCommit() const {
    return false;
}

bool DatabaseMetaData::supportsOpenStatementsAcrossRollback() const {
    return false;
}

bool DatabaseMetaData::locatorsUpdateCopy() const {
    return false;
}

bool DatabaseMetaData::supportsStatementPooling() const {
    return false;
}

bool DatabaseMetaData::autoCommitFailureClosesAllResultSets() const {
    // SQL_MULT_RESULT_SETS: если поддерживает множественные результаты
    auto val = getInfoSafely<SQLUSMALLINT>(hdbc_, SQL_MULT_RESULT_SETS, SQL_FALSE);
    return val == SQL_TRUE;
}

bool DatabaseMetaData::supportsStoredFunctionsUsingCallSyntax() const {
    return false;
}

bool DatabaseMetaData::generatedKeyAlwaysReturned() const {
    // SQL_GETDATA_EXTENSIONS: если поддерживает SQL_GD_OUTPUT_PARAMS
    auto val = getInfoSafely<SQLUSMALLINT>(hdbc_, SQL_GETDATA_EXTENSIONS, 0);
    return (val & SQL_GD_ANY_COLUMN) != 0;
}

// === Целочисленные методы ===

int DatabaseMetaData::getNullCollation() const {
    return getInfoSafely<SQLUSMALLINT>(hdbc_, SQL_NULL_COLLATION, 0);
}

int DatabaseMetaData::getSQLStateType() const {
    return getInfoSafely<SQLUSMALLINT>(hdbc_, SQL_SQL_CONFORMANCE, 0);
}

int DatabaseMetaData::getDefaultTransactionIsolation() const {
    return getInfoSafely<SQLUSMALLINT>(hdbc_, SQL_DEFAULT_TXN_ISOLATION, 0);
}

int DatabaseMetaData::getResultSetHoldability() const {
    return getInfoSafely<SQLUSMALLINT>(hdbc_, SQL_GETDATA_EXTENSIONS, 0);
}

int DatabaseMetaData::getRowIdLifetime() const {
    return getInfoSafely<SQLUSMALLINT>(hdbc_, SQL_ROW_UPDATES, 0);
}

int DatabaseMetaData::getMaxTableNameLength() const {
    return getInfoSafely<SQLUSMALLINT>(hdbc_, SQL_MAX_TABLE_NAME_LEN, 0);
}

int DatabaseMetaData::getMaxSchemaNameLength() const {
    return getInfoSafely<SQLUSMALLINT>(hdbc_, SQL_MAX_SCHEMA_NAME_LEN, 0);
}

int DatabaseMetaData::getMaxCatalogNameLength() const {
    return getInfoSafely<SQLUSMALLINT>(hdbc_, SQL_MAX_CATALOG_NAME_LEN, 0);
}

int DatabaseMetaData::getMaxColumnNameLength() const {
    return getInfoSafely<SQLUSMALLINT>(hdbc_, SQL_MAX_COLUMN_NAME_LEN, 0);
}

int DatabaseMetaData::getMaxColumnsInGroupBy() const {
    return getInfoSafely<SQLUSMALLINT>(hdbc_, SQL_MAX_COLUMNS_IN_GROUP_BY, 0);
}

int DatabaseMetaData::getMaxColumnsInOrderBy() const {
    return getInfoSafely<SQLUSMALLINT>(hdbc_, SQL_MAX_COLUMNS_IN_ORDER_BY, 0);
}

int DatabaseMetaData::getMaxColumnsInSelect() const {
    return getInfoSafely<SQLUSMALLINT>(hdbc_, SQL_MAX_COLUMNS_IN_SELECT, 0);
}

int DatabaseMetaData::getMaxColumnsInTable() const {
    return getInfoSafely<SQLUSMALLINT>(hdbc_, SQL_MAX_COLUMNS_IN_TABLE, 0);
}

int DatabaseMetaData::getMaxColumnsInIndex() const {
    return getInfoSafely<SQLUSMALLINT>(hdbc_, SQL_MAX_COLUMNS_IN_INDEX, 0);
}

int DatabaseMetaData::getMaxStatementLength() const {
    return getInfoSafely<SQLUINTEGER>(hdbc_, SQL_MAX_STATEMENT_LEN, 0);
}

int DatabaseMetaData::getMaxStatements() const {
    return 0;  // неизвестно / не ограничено
}

int DatabaseMetaData::getMaxTablesInSelect() const {
    return getInfoSafely<SQLUSMALLINT>(hdbc_, SQL_MAX_TABLES_IN_SELECT, 0);
}

int DatabaseMetaData::getMaxUserNameLength() const {
    return getInfoSafely<SQLUSMALLINT>(hdbc_, SQL_MAX_USER_NAME_LEN, 0);
}

int DatabaseMetaData::getMaxRowSize() const {
    return getInfoSafely<SQLUINTEGER>(hdbc_, SQL_MAX_ROW_SIZE, 0);
}

int DatabaseMetaData::getDatabaseMajorVersion() const {
    std::wstring ver = getDatabaseProductVersion();
    if (ver.empty()) return 0;
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
    std::string utf8 = conv.to_bytes(ver);
    int major = 0, minor = 0;
    sscanf(utf8.c_str(), "%d.%d", &major, &minor);
    return major;
}

int DatabaseMetaData::getDatabaseMinorVersion() const {
    std::wstring ver = getDatabaseProductVersion();
    if (ver.empty()) return 0;
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
    std::string utf8 = conv.to_bytes(ver);
    int major = 0, minor = 0;
    sscanf(utf8.c_str(), "%d.%d", &major, &minor);
    return minor;
}

int DatabaseMetaData::getDriverMajorVersion() const {
    std::wstring ver = getDriverVersion();
    if (ver.empty()) return 0;
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
    std::string utf8 = conv.to_bytes(ver);
    int major = 0, minor = 0;
    sscanf(utf8.c_str(), "%d.%d", &major, &minor);
    return major;
}

int DatabaseMetaData::getDriverMinorVersion() const {
    std::wstring ver = getDriverVersion();
    if (ver.empty()) return 0;
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
    std::string utf8 = conv.to_bytes(ver);
    int major = 0, minor = 0;
    sscanf(utf8.c_str(), "%d.%d", &major, &minor);
    return minor;
}



// === Поддержка уровней изоляции ===
bool DatabaseMetaData::supportsTransactionIsolationLevel(int level) const {
    SQLUINTEGER supported = 0;
    SQLRETURN ret = SQLGetInfo(hdbc_, SQL_TXN_ISOLATION_OPTION, &supported, 0, nullptr);
    if (ret != SQL_SUCCESS) return false;

    switch (level) {
        case 1:  return (supported & SQL_TXN_READ_UNCOMMITTED) != 0;
        case 2:  return (supported & SQL_TXN_READ_COMMITTED) != 0;
        case 4:  return (supported & SQL_TXN_REPEATABLE_READ) != 0;
        case 8:  return (supported & SQL_TXN_SERIALIZABLE) != 0;
        case 0:  return (supported & 0x00000000) != 0;  // 0x00000000 = SQL_TXN_NONE
        default: return false;
    }
}

// === Каталоги ===
/*
nanodbc::result DatabaseMetaData::getTables(const std::wstring& catalog, const std::wstring& schema,
    const std::wstring& table, const std::wstring& type) const {
    return nanodbc::catalog::tables(hdbc_,
        catalog.empty() ? nullptr : catalog.c_str(),
        schema.empty() ? nullptr : schema.c_str(),
        table.empty() ? nullptr : table.c_str(),
        type.empty() ? nullptr : type.c_str());
}

nanodbc::result DatabaseMetaData::getColumns(const std::wstring& catalog, const std::wstring& schema,
    const std::wstring& table, const std::wstring& column) const {
    return nanodbc::catalog::columns(hdbc_,
        catalog.empty() ? nullptr : catalog.c_str(),
        schema.empty() ? nullptr : schema.c_str(),
        table.empty() ? nullptr : table.c_str(),
        column.empty() ? nullptr : column.c_str());
}

nanodbc::result DatabaseMetaData::getPrimaryKeys(const std::wstring& catalog, const std::wstring& schema,
    const std::wstring& table) const {
    return nanodbc::catalog::primary_keys(hdbc_,
        catalog.empty() ? nullptr : catalog.c_str(),
        schema.empty() ? nullptr : schema.c_str(),
        table.empty() ? nullptr : table.c_str());
}

nanodbc::result DatabaseMetaData::getImportedKeys(const std::wstring& catalog, const std::wstring& schema,
    const std::wstring& table) const {
    return nanodbc::catalog::foreign_keys(hdbc_,
        catalog.empty() ? nullptr : catalog.c_str(),
        schema.empty() ? nullptr : schema.c_str(),
        table.empty() ? nullptr : table.c_str(),
        nullptr, nullptr, nullptr);
}

nanodbc::result DatabaseMetaData::getTypeInfo(short sqlType) const {
    return nanodbc::catalog::type_info(hdbc_, sqlType);
}

nanodbc::result DatabaseMetaData::getProcedures(const std::wstring& catalog, const std::wstring& schema,
    const std::wstring& procedure) const {
    return nanodbc::catalog::procedures(hdbc_,
        catalog.empty() ? nullptr : catalog.c_str(),
        schema.empty() ? nullptr : schema.c_str(),
        procedure.empty() ? nullptr : procedure.c_str());
}

nanodbc::result DatabaseMetaData::getProcedureColumns(const std::wstring& catalog, const std::wstring& schema,
    const std::wstring& procedure, const std::wstring& column) const {
    return nanodbc::catalog::procedure_columns(hdbc_,
        catalog.empty() ? nullptr : catalog.c_str(),
        schema.empty() ? nullptr : schema.c_str(),
        procedure.empty() ? nullptr : procedure.c_str(),
        column.empty() ? nullptr : column.c_str());
}
*/