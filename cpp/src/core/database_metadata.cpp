#include "core/database_metadata.hpp"
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

// === Универсальный getAttribute для числовых типов ===
template<typename T>
static T getAttribute(SQLHDBC hdbc, SQLUSMALLINT attr) {
    T value = 0;
    SQLRETURN ret = SQLGetInfo(hdbc, attr, &value, 0, nullptr);
    return (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) ? value : 0;
}

// === Получение строки ===
static std::wstring getString(SQLHDBC hdbc, SQLUSMALLINT attr) {
    SQLWCHAR buffer[BUFFER_SIZE] = { 0 };
    SQLSMALLINT byteLength = 0;
    SQLRETURN ret = SQLGetInfo(hdbc, attr, buffer, sizeof(buffer), &byteLength);
    if (!isOdbcSuccess(ret) || byteLength == 0) {
        return std::wstring();
    }

    // Преобразуем байты в количество символов
    SQLSMALLINT numChars = byteLength / sizeof(SQLWCHAR);
    // Защита от переполнения
    numChars = std::min(numChars, static_cast<SQLSMALLINT>(BUFFER_SIZE - 1));
    return std::wstring(buffer, numChars);
}

// === Конструктор ===
DatabaseMetaData::DatabaseMetaData(SQLHDBC hdbc) : hdbc_(hdbc) {}

// === Строковые методы ===

std::wstring DatabaseMetaData::getDatabaseProductName() const {
    return getString(hdbc_, SQL_DBMS_NAME);
}

std::wstring DatabaseMetaData::getDatabaseProductVersion() const {
    return getString(hdbc_, SQL_DBMS_VER);
}

std::wstring DatabaseMetaData::getDriverName() const {
    return getString(hdbc_, SQL_DRIVER_NAME);
}

std::wstring DatabaseMetaData::getDriverVersion() const {
    return getString(hdbc_, SQL_DRIVER_VER);
}

std::wstring DatabaseMetaData::getIdentifierQuoteString() const {
    return getString(hdbc_, SQL_IDENTIFIER_QUOTE_CHAR);
}

std::wstring DatabaseMetaData::getSchemaTerm() const {
    return getString(hdbc_, SQL_SCHEMA_TERM);
}

std::wstring DatabaseMetaData::getProcedureTerm() const {
    return getString(hdbc_, SQL_PROCEDURE_TERM);
}

std::wstring DatabaseMetaData::getCatalogTerm() const {
    return getString(hdbc_, SQL_CATALOG_TERM);
}

std::wstring DatabaseMetaData::getCatalogSeparator() const {
    return getString(hdbc_, SQL_CATALOG_NAME_SEPARATOR);
}

std::wstring DatabaseMetaData::getSQLKeywords() const {
    return getString(hdbc_, SQL_KEYWORDS);
}

std::wstring DatabaseMetaData::getNumericFunctions() const {
    return getString(hdbc_, SQL_NUMERIC_FUNCTIONS);
}

std::wstring DatabaseMetaData::getStringFunctions() const {
    return getString(hdbc_, SQL_STRING_FUNCTIONS);
}

std::wstring DatabaseMetaData::getSystemFunctions() const {
    return getString(hdbc_, SQL_SYSTEM_FUNCTIONS);
}

std::wstring DatabaseMetaData::getTimeDateFunctions() const {
    return getString(hdbc_, SQL_TIMEDATE_FUNCTIONS);
}

std::wstring DatabaseMetaData::getSearchStringEscape() const {
    return getString(hdbc_, SQL_SEARCH_PATTERN_ESCAPE);
}

std::wstring DatabaseMetaData::getExtraNameCharacters() const {
    return getString(hdbc_, SQL_SPECIAL_CHARACTERS);
}

// === Булевы методы ===

bool DatabaseMetaData::isReadOnly() const {
    return getAttribute<SQLUSMALLINT>(hdbc_, SQL_DATA_SOURCE_READ_ONLY) == SQL_TRUE;
}

bool DatabaseMetaData::supportsTransactions() const {
    return getAttribute<SQLUSMALLINT>(hdbc_, SQL_TXN_CAPABLE) != SQL_TC_NONE;
}

bool DatabaseMetaData::supportsSavepoints() const {
    return false;  // Savepoints — это JDBC-фича, в ODBC нет прямого аналога.
}

bool DatabaseMetaData::supportsNamedParameters() const {
    return false;  // ODBC не поддерживает именованные параметры по стандарту
}

bool DatabaseMetaData::supportsBatchUpdates() const {
    SQLUINTEGER caps = 0;
    SQLRETURN ret = SQLGetInfo(hdbc_, SQL_PARAM_ARRAY_ROW_COUNTS, &caps, 0, nullptr);
    return ret == SQL_SUCCESS && caps == 0x0001; // 0x0001 = SQL_PARC_YES_ROW_COUNTS
}

bool DatabaseMetaData::supportsUnion() const {
    return getAttribute<SQLUSMALLINT>(hdbc_, SQL_UNION) == SQL_TRUE;
}

bool DatabaseMetaData::supportsUnionAll() const {
    // SQL_UNION_ALL не определён в некоторых SDK, но
    // если SQL_UNION == SQL_TRUE, то UNION ALL тоже поддерживается
    return getAttribute<SQLUSMALLINT>(hdbc_, SQL_UNION) == SQL_TRUE;
}

bool DatabaseMetaData::supportsLikeEscapeClause() const {
    return getAttribute<SQLUSMALLINT>(hdbc_, SQL_LIKE_ESCAPE_CLAUSE) == SQL_TRUE;
}

bool DatabaseMetaData::supportsGroupBy() const {
    return getAttribute<SQLUSMALLINT>(hdbc_, SQL_GROUP_BY) != SQL_GB_NOT_SUPPORTED;
}

bool DatabaseMetaData::supportsGroupByUnrelated() const {
    return getAttribute<SQLUSMALLINT>(hdbc_, SQL_GROUP_BY) == SQL_GB_COLLATE;
}

bool DatabaseMetaData::supportsGroupByBeyondSelect() const {
    return getAttribute<SQLUSMALLINT>(hdbc_, SQL_GROUP_BY) == SQL_GB_COLLATE;
}

bool DatabaseMetaData::supportsOrderByUnrelated() const {
    return getAttribute<SQLUSMALLINT>(hdbc_, SQL_ORDER_BY_COLUMNS_IN_SELECT) == SQL_FALSE;
}

bool DatabaseMetaData::supportsAlterTableWithAddColumn() const {
    SQLUSMALLINT cap = getAttribute<SQLUSMALLINT>(hdbc_, SQL_ALTER_TABLE);
    return (cap & SQL_AT_ADD_COLUMN) != 0;
}

bool DatabaseMetaData::supportsColumnAliasing() const {
    return getAttribute<SQLUSMALLINT>(hdbc_, SQL_COLUMN_ALIAS) == SQL_TRUE;
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
    // SQL_SELECT_FOR_UPDATE может не быть
    // Альтернатива: SQL_FOR_UPDATE — в некоторых драйверах
    SQLUSMALLINT supports = 0;
    SQLRETURN ret = SQLGetInfo(hdbc_, 125 /* SQL_FOR_UPDATE */, &supports, 0, nullptr);
    return ret == SQL_SUCCESS && supports == SQL_TRUE;
}

bool DatabaseMetaData::supportsStoredProcedures() const {
    return getAttribute<SQLUSMALLINT>(hdbc_, SQL_PROCEDURES) == SQL_TRUE;
}

bool DatabaseMetaData::supportsSubqueriesInComparisons() const {
    return getAttribute<SQLUSMALLINT>(hdbc_, SQL_SUBQUERIES) != SQL_SQ_COMPARISON;
}

bool DatabaseMetaData::supportsSubqueriesInExists() const {
    SQLUSMALLINT subs = getAttribute<SQLUSMALLINT>(hdbc_, SQL_SUBQUERIES);
    return (subs & 0x00000010) != 0;  // 0x10 = SQL_SQ_CORRELATED
}

bool DatabaseMetaData::supportsSubqueriesInIns() const {
    return getAttribute<SQLUSMALLINT>(hdbc_, SQL_SUBQUERIES) != SQL_SQ_QUANTIFIED;
}

bool DatabaseMetaData::supportsSubqueriesInQuantifieds() const {
    SQLUSMALLINT subs = 0;
    SQLRETURN ret = SQLGetInfo(hdbc_, SQL_SUBQUERIES, &subs, 0, nullptr);
    if (ret != SQL_SUCCESS) {
        return false;
    }
    return (subs & 0x00000008) != 0;  // SQL_SQ_QUANTIFIED
}

bool DatabaseMetaData::supportsCorrelatedSubqueries() const {
    SQLUSMALLINT subs = 0;
    SQLRETURN ret = SQLGetInfo(hdbc_, SQL_SUBQUERIES, &subs, 0, nullptr);
    if (ret != SQL_SUCCESS) {
        return false;
    }
    return (subs & 0x00000010) != 0;  // SQL_SQ_CORRELATED
}

bool DatabaseMetaData::supportsIntegrityEnhancementFacility() const {
    return getAttribute<SQLUSMALLINT>(hdbc_, SQL_INTEGRITY) == SQL_TRUE;
}

bool DatabaseMetaData::supportsOuterJoins() const {
    return getAttribute<SQLUSMALLINT>(hdbc_, SQL_OUTER_JOINS) == SQL_TRUE;
}

bool DatabaseMetaData::supportsFullOuterJoins() const {
    // SQL_FULL_OUTER_JOIN может не быть
    // Проверим SQL_OJ_FULL_OUTER_JOIN через SQL_OUTER_JOINS
    SQLUSMALLINT outerJoins = 0;
    SQLRETURN ret = SQLGetInfo(hdbc_, SQL_OUTER_JOINS, &outerJoins, 0, nullptr);
    if (ret != SQL_SUCCESS) return false;
    // SQL_OJ_FULL_OUTER_JOIN = 0x00000008
    return (outerJoins & 0x00000008) != 0;
}

bool DatabaseMetaData::supportsSchemasInDataManipulation() const {
    SQLUSMALLINT usage = getAttribute<SQLUSMALLINT>(hdbc_, SQL_SCHEMA_USAGE);
    return (usage & SQL_SU_DML_STATEMENTS) != 0;
}

bool DatabaseMetaData::supportsSchemasInProcedureCalls() const {
    SQLUSMALLINT usage = getAttribute<SQLUSMALLINT>(hdbc_, SQL_SCHEMA_USAGE);
    return (usage & SQL_SU_PROCEDURE_INVOCATION) != 0;
}

bool DatabaseMetaData::supportsSchemasInTableDefinitions() const {
    SQLUSMALLINT usage = getAttribute<SQLUSMALLINT>(hdbc_, SQL_SCHEMA_USAGE);
    return (usage & SQL_SU_TABLE_DEFINITION) != 0;
}

bool DatabaseMetaData::supportsSchemasInIndexDefinitions() const {
    SQLUSMALLINT usage = getAttribute<SQLUSMALLINT>(hdbc_, SQL_SCHEMA_USAGE);
    return (usage & SQL_SU_INDEX_DEFINITION) != 0;
}

bool DatabaseMetaData::supportsSchemasInPrivilegeDefinitions() const {
    SQLUSMALLINT usage = getAttribute<SQLUSMALLINT>(hdbc_, SQL_SCHEMA_USAGE);
    return (usage & SQL_SU_PRIVILEGE_DEFINITION) != 0;
}

bool DatabaseMetaData::supportsCatalogsInDataManipulation() const {
    SQLUSMALLINT usage = getAttribute<SQLUSMALLINT>(hdbc_, SQL_CATALOG_USAGE);
    return (usage & SQL_CU_DML_STATEMENTS) != 0;
}

bool DatabaseMetaData::supportsCatalogsInProcedureCalls() const {
    SQLUSMALLINT usage = getAttribute<SQLUSMALLINT>(hdbc_, SQL_CATALOG_USAGE);
    return (usage & SQL_CU_PROCEDURE_INVOCATION) != 0;
}

bool DatabaseMetaData::supportsCatalogsInTableDefinitions() const {
    SQLUSMALLINT usage = getAttribute<SQLUSMALLINT>(hdbc_, SQL_CATALOG_USAGE);
    return (usage & SQL_CU_TABLE_DEFINITION) != 0;
}

bool DatabaseMetaData::supportsCatalogsInIndexDefinitions() const {
    SQLUSMALLINT usage = getAttribute<SQLUSMALLINT>(hdbc_, SQL_CATALOG_USAGE);
    return (usage & SQL_CU_INDEX_DEFINITION) != 0;
}

bool DatabaseMetaData::supportsCatalogsInPrivilegeDefinitions() const {
    SQLUSMALLINT usage = getAttribute<SQLUSMALLINT>(hdbc_, SQL_CATALOG_USAGE);
    return (usage & SQL_CU_PRIVILEGE_DEFINITION) != 0;
}

bool DatabaseMetaData::supportsPositionedDelete() const {
    return false;
}

bool DatabaseMetaData::supportsPositionedUpdate() const {
    return false;
}

bool DatabaseMetaData::supportsOpenCursorsAcrossCommit() const {
    return getAttribute<SQLUSMALLINT>(hdbc_, SQL_CURSOR_COMMIT_BEHAVIOR) == SQL_CB_PRESERVE;
}

bool DatabaseMetaData::supportsOpenCursorsAcrossRollback() const {
    return getAttribute<SQLUSMALLINT>(hdbc_, SQL_CURSOR_ROLLBACK_BEHAVIOR) == SQL_CB_PRESERVE;
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
    return getAttribute<SQLUSMALLINT>(hdbc_, SQL_MULT_RESULT_SETS) == SQL_TRUE;
}

bool DatabaseMetaData::supportsStoredFunctionsUsingCallSyntax() const {
    return false;
}

bool DatabaseMetaData::generatedKeyAlwaysReturned() const {
    // SQL_GETDATA_EXTENSIONS: если поддерживает SQL_GD_OUTPUT_PARAMS
    return (::getAttribute<SQLUSMALLINT>(hdbc_, SQL_GETDATA_EXTENSIONS) & SQL_GD_ANY_COLUMN) != 0;
}

// === Целочисленные методы ===

int DatabaseMetaData::getNullCollation() const {
    return static_cast<int>(::getAttribute<SQLUSMALLINT>(hdbc_, SQL_NULL_COLLATION));
}

int DatabaseMetaData::getSQLStateType() const {
    return static_cast<int>(::getAttribute<SQLUSMALLINT>(hdbc_, SQL_SQL_CONFORMANCE));
}

int DatabaseMetaData::getDefaultTransactionIsolation() const {
    return static_cast<int>(::getAttribute<SQLUSMALLINT>(hdbc_, SQL_DEFAULT_TXN_ISOLATION));
}

int DatabaseMetaData::getResultSetHoldability() const {
    return static_cast<int>(::getAttribute<SQLUSMALLINT>(hdbc_, SQL_GETDATA_EXTENSIONS));
}

int DatabaseMetaData::getRowIdLifetime() const {
    return static_cast<int>(::getAttribute<SQLUSMALLINT>(hdbc_, SQL_ROW_UPDATES));
}

int DatabaseMetaData::getMaxTableNameLength() const {
    return static_cast<int>(::getAttribute<SQLUSMALLINT>(hdbc_, SQL_MAX_TABLE_NAME_LEN));
}

int DatabaseMetaData::getMaxSchemaNameLength() const {
    return static_cast<int>(::getAttribute<SQLUSMALLINT>(hdbc_, SQL_MAX_SCHEMA_NAME_LEN));
}

int DatabaseMetaData::getMaxCatalogNameLength() const {
    return static_cast<int>(::getAttribute<SQLUSMALLINT>(hdbc_, SQL_MAX_CATALOG_NAME_LEN));
}

int DatabaseMetaData::getMaxColumnNameLength() const {
    return static_cast<int>(::getAttribute<SQLUSMALLINT>(hdbc_, SQL_MAX_COLUMN_NAME_LEN));
}

int DatabaseMetaData::getMaxColumnsInGroupBy() const {
    return static_cast<int>(::getAttribute<SQLUSMALLINT>(hdbc_, SQL_MAX_COLUMNS_IN_GROUP_BY));
}

int DatabaseMetaData::getMaxColumnsInOrderBy() const {
    return static_cast<int>(::getAttribute<SQLUSMALLINT>(hdbc_, SQL_MAX_COLUMNS_IN_ORDER_BY));
}

int DatabaseMetaData::getMaxColumnsInSelect() const {
    return static_cast<int>(::getAttribute<SQLUSMALLINT>(hdbc_, SQL_MAX_COLUMNS_IN_SELECT));
}

int DatabaseMetaData::getMaxColumnsInTable() const {
    return static_cast<int>(::getAttribute<SQLUSMALLINT>(hdbc_, SQL_MAX_COLUMNS_IN_TABLE));
}

int DatabaseMetaData::getMaxColumnsInIndex() const {
    return static_cast<int>(::getAttribute<SQLUSMALLINT>(hdbc_, SQL_MAX_COLUMNS_IN_INDEX));
}

int DatabaseMetaData::getMaxStatementLength() const {
    return static_cast<int>(::getAttribute<SQLUINTEGER>(hdbc_, SQL_MAX_STATEMENT_LEN));
}

int DatabaseMetaData::getMaxStatements() const {
    return 0;  // неизвестно / не ограничено
}

int DatabaseMetaData::getMaxTablesInSelect() const {
    return static_cast<int>(::getAttribute<SQLUSMALLINT>(hdbc_, SQL_MAX_TABLES_IN_SELECT));
}

int DatabaseMetaData::getMaxUserNameLength() const {
    return static_cast<int>(::getAttribute<SQLUSMALLINT>(hdbc_, SQL_MAX_USER_NAME_LEN));
}

int DatabaseMetaData::getMaxRowSize() const {
    return static_cast<int>(::getAttribute<SQLUINTEGER>(hdbc_, SQL_MAX_ROW_SIZE));
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

/*

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
    case 0:  return (supported & SQL_TXN_NONE) != 0;
    default: return false;
    }
}

// === Каталоги ===

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