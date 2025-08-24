#include "struct/database_metadata_c.h"
#include "utils/string_utils.hpp"
#include "utils/logger.hpp"

using namespace utils;

inline static const char16_t* convert(const std::wstring& str) {
    LOG_TRACE_W(L"Converting wstring to char16_t*: '{}'", str);
    auto u16str = to_u16string(str);
    const char16_t* result = duplicate_string(u16str.c_str());
    LOG_TRACE("Converted string duplicated at {}", (void*)result);
    return result;
}

CDatabaseMetaData::CDatabaseMetaData(const CDatabaseMetaData& other) {
    // === Строковые значения ===
    databaseProductName = duplicate_string(other.databaseProductName);
    databaseProductVersion = duplicate_string(other.databaseProductVersion);
    driverName = duplicate_string(other.driverName);
    driverVersion = duplicate_string(other.driverVersion);
    identifierQuoteString = duplicate_string(other.identifierQuoteString);
    schemaTerm = duplicate_string(other.schemaTerm);
    procedureTerm = duplicate_string(other.procedureTerm);
    catalogTerm = duplicate_string(other.catalogTerm);
    catalogSeparator = duplicate_string(other.catalogSeparator);
    sqlKeywords = duplicate_string(other.sqlKeywords);
    numericFunctions = duplicate_string(other.numericFunctions);
    stringFunctions = duplicate_string(other.stringFunctions);
    systemFunctions = duplicate_string(other.systemFunctions);
    timeDateFunctions = duplicate_string(other.timeDateFunctions);
    searchStringEscape = duplicate_string(other.searchStringEscape);
    extraNameCharacters = duplicate_string(other.extraNameCharacters);

    // === Булевы значения ===
    isReadOnly = other.isReadOnly;
    supportsTransactions = other.supportsTransactions;
    supportsSavepoints = other.supportsSavepoints;
    supportsNamedParameters = other.supportsNamedParameters;
    supportsBatchUpdates = other.supportsBatchUpdates;
    supportsUnion = other.supportsUnion;
    supportsUnionAll = other.supportsUnionAll;
    supportsLikeEscapeClause = other.supportsLikeEscapeClause;
    supportsGroupBy = other.supportsGroupBy;
    supportsGroupByUnrelated = other.supportsGroupByUnrelated;
    supportsGroupByBeyondSelect = other.supportsGroupByBeyondSelect;
    supportsOrderByUnrelated = other.supportsOrderByUnrelated;
    supportsAlterTableWithAddColumn = other.supportsAlterTableWithAddColumn;
    supportsColumnAliasing = other.supportsColumnAliasing;
    nullPlusNonNullIsNull = other.nullPlusNonNullIsNull;
    supportsExpressionsInOrderBy = other.supportsExpressionsInOrderBy;
    supportsSelectForUpdate = other.supportsSelectForUpdate;
    supportsStoredProcedures = other.supportsStoredProcedures;
    supportsSubqueriesInComparisons = other.supportsSubqueriesInComparisons;
    supportsSubqueriesInExists = other.supportsSubqueriesInExists;
    supportsSubqueriesInIns = other.supportsSubqueriesInIns;
    supportsSubqueriesInQuantifieds = other.supportsSubqueriesInQuantifieds;
    supportsCorrelatedSubqueries = other.supportsCorrelatedSubqueries;
    supportsIntegrityEnhancementFacility = other.supportsIntegrityEnhancementFacility;
    supportsOuterJoins = other.supportsOuterJoins;
    supportsFullOuterJoins = other.supportsFullOuterJoins;
    supportsLimitedOuterJoins = other.supportsLimitedOuterJoins;
    supportsSchemasInDataManipulation = other.supportsSchemasInDataManipulation;
    supportsSchemasInProcedureCalls = other.supportsSchemasInProcedureCalls;
    supportsSchemasInTableDefinitions = other.supportsSchemasInTableDefinitions;
    supportsSchemasInIndexDefinitions = other.supportsSchemasInIndexDefinitions;
    supportsSchemasInPrivilegeDefinitions = other.supportsSchemasInPrivilegeDefinitions;
    supportsCatalogsInDataManipulation = other.supportsCatalogsInDataManipulation;
    supportsCatalogsInProcedureCalls = other.supportsCatalogsInProcedureCalls;
    supportsCatalogsInTableDefinitions = other.supportsCatalogsInTableDefinitions;
    supportsCatalogsInIndexDefinitions = other.supportsCatalogsInIndexDefinitions;
    supportsCatalogsInPrivilegeDefinitions = other.supportsCatalogsInPrivilegeDefinitions;
    supportsPositionedDelete = other.supportsPositionedDelete;
    supportsPositionedUpdate = other.supportsPositionedUpdate;
    supportsOpenCursorsAcrossCommit = other.supportsOpenCursorsAcrossCommit;
    supportsOpenCursorsAcrossRollback = other.supportsOpenCursorsAcrossRollback;
    supportsOpenStatementsAcrossCommit = other.supportsOpenStatementsAcrossCommit;
    supportsOpenStatementsAcrossRollback = other.supportsOpenStatementsAcrossRollback;
    locatorsUpdateCopy = other.locatorsUpdateCopy;
    supportsStatementPooling = other.supportsStatementPooling;
    autoCommitFailureClosesAllResultSets = other.autoCommitFailureClosesAllResultSets;
    supportsStoredFunctionsUsingCallSyntax = other.supportsStoredFunctionsUsingCallSyntax;
    generatedKeyAlwaysReturned = other.generatedKeyAlwaysReturned;

    // === Целочисленные значения ===
    supportsTransactionIsolationLevel = other.supportsTransactionIsolationLevel;
    nullCollation = other.nullCollation;
    sqlStateType = other.sqlStateType;
    defaultTransactionIsolation = other.defaultTransactionIsolation;
    resultSetHoldability = other.resultSetHoldability;
    rowIdLifetime = other.rowIdLifetime;
    maxTableNameLength = other.maxTableNameLength;
    maxSchemaNameLength = other.maxSchemaNameLength;
    maxCatalogNameLength = other.maxCatalogNameLength;
    maxColumnNameLength = other.maxColumnNameLength;
    maxColumnsInGroupBy = other.maxColumnsInGroupBy;
    maxColumnsInOrderBy = other.maxColumnsInOrderBy;
    maxColumnsInSelect = other.maxColumnsInSelect;
    maxColumnsInTable = other.maxColumnsInTable;
    maxColumnsInIndex = other.maxColumnsInIndex;
    maxStatementLength = other.maxStatementLength;
    maxStatements = other.maxStatements;
    maxTablesInSelect = other.maxTablesInSelect;
    maxUserNameLength = other.maxUserNameLength;
    maxRowSize = other.maxRowSize;
    databaseMajorVersion = other.databaseMajorVersion;
    databaseMinorVersion = other.databaseMinorVersion;
    driverMajorVersion = other.driverMajorVersion;
    driverMinorVersion = other.driverMinorVersion;
}

CDatabaseMetaData::CDatabaseMetaData(const DatabaseMetaData& other) {
    // === Строковые значения ===
    databaseProductName = convert(other.getDatabaseProductName());
    databaseProductVersion = convert(other.getDatabaseProductVersion());
    driverName = convert(other.getDriverName());
    driverVersion = convert(other.getDriverVersion());
    identifierQuoteString = convert(other.getIdentifierQuoteString());
    schemaTerm = convert(other.getSchemaTerm());
    procedureTerm = convert(other.getProcedureTerm());
    catalogTerm = convert(other.getCatalogTerm());
    catalogSeparator = convert(other.getCatalogSeparator());
    sqlKeywords = convert(other.getSQLKeywords());
    numericFunctions = convert(other.getNumericFunctions());
    stringFunctions = convert(other.getStringFunctions());
    systemFunctions = convert(other.getSystemFunctions());
    timeDateFunctions = convert(other.getTimeDateFunctions());
    searchStringEscape = convert(other.getSearchStringEscape());
    extraNameCharacters = convert(other.getExtraNameCharacters());

    // === Булевы значения ===
    isReadOnly = other.isReadOnly();
    supportsTransactions = other.supportsTransactions();
    supportsSavepoints = other.supportsSavepoints();
    supportsNamedParameters = other.supportsNamedParameters();
    supportsBatchUpdates = other.supportsBatchUpdates();
    supportsUnion = other.supportsUnion();
    supportsUnionAll = other.supportsUnionAll();
    supportsLikeEscapeClause = other.supportsLikeEscapeClause();
    supportsGroupBy = other.supportsGroupBy();
    supportsGroupByUnrelated = other.supportsGroupByUnrelated();
    supportsGroupByBeyondSelect = other.supportsGroupByBeyondSelect();
    supportsOrderByUnrelated = other.supportsOrderByUnrelated();
    supportsAlterTableWithAddColumn = other.supportsAlterTableWithAddColumn();
    supportsColumnAliasing = other.supportsColumnAliasing();
    nullPlusNonNullIsNull = other.nullPlusNonNullIsNull();
    supportsExpressionsInOrderBy = other.supportsExpressionsInOrderBy();
    supportsSelectForUpdate = other.supportsSelectForUpdate();
    supportsStoredProcedures = other.supportsStoredProcedures();
    supportsSubqueriesInComparisons = other.supportsSubqueriesInComparisons();
    supportsSubqueriesInExists = other.supportsSubqueriesInExists();
    supportsSubqueriesInIns = other.supportsSubqueriesInIns();
    supportsSubqueriesInQuantifieds = other.supportsSubqueriesInQuantifieds();
    supportsCorrelatedSubqueries = other.supportsCorrelatedSubqueries();
    supportsIntegrityEnhancementFacility = other.supportsIntegrityEnhancementFacility();
    supportsOuterJoins = other.supportsOuterJoins();
    supportsFullOuterJoins = other.supportsFullOuterJoins();
    supportsLimitedOuterJoins = other.supportsLimitedOuterJoins();
    supportsSchemasInDataManipulation = other.supportsSchemasInDataManipulation();
    supportsSchemasInProcedureCalls = other.supportsSchemasInProcedureCalls();
    supportsSchemasInTableDefinitions = other.supportsSchemasInTableDefinitions();
    supportsSchemasInIndexDefinitions = other.supportsSchemasInIndexDefinitions();
    supportsSchemasInPrivilegeDefinitions = other.supportsSchemasInPrivilegeDefinitions();
    supportsCatalogsInDataManipulation = other.supportsCatalogsInDataManipulation();
    supportsCatalogsInProcedureCalls = other.supportsCatalogsInProcedureCalls();
    supportsCatalogsInTableDefinitions = other.supportsCatalogsInTableDefinitions();
    supportsCatalogsInIndexDefinitions = other.supportsCatalogsInIndexDefinitions();
    supportsCatalogsInPrivilegeDefinitions = other.supportsCatalogsInPrivilegeDefinitions();
    supportsPositionedDelete = other.supportsPositionedDelete();
    supportsPositionedUpdate = other.supportsPositionedUpdate();
    supportsOpenCursorsAcrossCommit = other.supportsOpenCursorsAcrossCommit();
    supportsOpenCursorsAcrossRollback = other.supportsOpenCursorsAcrossRollback();
    supportsOpenStatementsAcrossCommit = other.supportsOpenStatementsAcrossCommit();
    supportsOpenStatementsAcrossRollback = other.supportsOpenStatementsAcrossRollback();
    locatorsUpdateCopy = other.locatorsUpdateCopy();
    supportsStatementPooling = other.supportsStatementPooling();
    autoCommitFailureClosesAllResultSets = other.autoCommitFailureClosesAllResultSets();
    supportsStoredFunctionsUsingCallSyntax = other.supportsStoredFunctionsUsingCallSyntax();
    generatedKeyAlwaysReturned = other.generatedKeyAlwaysReturned();

    // === Целочисленные значения ===
    supportsTransactionIsolationLevel = other.supportsTransactionIsolationLevel();
    nullCollation = other.getNullCollation();
    sqlStateType = other.getSQLStateType();
    defaultTransactionIsolation = other.getDefaultTransactionIsolation();
    resultSetHoldability = other.getResultSetHoldability();
    rowIdLifetime = other.getRowIdLifetime();
    maxTableNameLength = other.getMaxTableNameLength();
    maxSchemaNameLength = other.getMaxSchemaNameLength();
    maxCatalogNameLength = other.getMaxCatalogNameLength();
    maxColumnNameLength = other.getMaxColumnNameLength();
    maxColumnsInGroupBy = other.getMaxColumnsInGroupBy();
    maxColumnsInOrderBy = other.getMaxColumnsInOrderBy();
    maxColumnsInSelect = other.getMaxColumnsInSelect();
    maxColumnsInTable = other.getMaxColumnsInTable();
    maxColumnsInIndex = other.getMaxColumnsInIndex();
    maxStatementLength = other.getMaxStatementLength();
    maxStatements = other.getMaxStatements();
    maxTablesInSelect = other.getMaxTablesInSelect();
    maxUserNameLength = other.getMaxUserNameLength();
    maxRowSize = other.getMaxRowSize();
    databaseMajorVersion = other.getDatabaseMajorVersion();
    databaseMinorVersion = other.getDatabaseMinorVersion();
    driverMajorVersion = other.getDriverMajorVersion();
    driverMinorVersion = other.getDriverMinorVersion();
}

CDatabaseMetaData::~CDatabaseMetaData() {
    auto str_free = [&](const char16_t*& str) {
        if (str) free((void*)str);
        str = nullptr;
    };

    // === Строковые значения ===
    str_free(databaseProductName);
    str_free(databaseProductVersion);
    str_free(driverName);
    str_free(driverVersion);
    str_free(identifierQuoteString);
    str_free(schemaTerm);
    str_free(procedureTerm);
    str_free(catalogTerm);
    str_free(catalogSeparator);
    str_free(sqlKeywords);
    str_free(numericFunctions);
    str_free(stringFunctions);
    str_free(systemFunctions);
    str_free(timeDateFunctions);
    str_free(searchStringEscape);
    str_free(extraNameCharacters);

    // === Булевы значения ===
    isReadOnly = false;
    supportsTransactions = false;
    supportsSavepoints = false;
    supportsNamedParameters = false;
    supportsBatchUpdates = false;
    supportsUnion = false;
    supportsUnionAll = false;
    supportsLikeEscapeClause = false;
    supportsGroupBy = false;
    supportsGroupByUnrelated = false;
    supportsGroupByBeyondSelect = false;
    supportsOrderByUnrelated = false;
    supportsAlterTableWithAddColumn = false;
    supportsColumnAliasing = false;
    nullPlusNonNullIsNull = false;
    supportsExpressionsInOrderBy = false;
    supportsSelectForUpdate = false;
    supportsStoredProcedures = false;
    supportsSubqueriesInComparisons = false;
    supportsSubqueriesInExists = false;
    supportsSubqueriesInIns = false;
    supportsSubqueriesInQuantifieds = false;
    supportsCorrelatedSubqueries = false;
    supportsIntegrityEnhancementFacility = false;
    supportsOuterJoins = false;
    supportsFullOuterJoins = false;
    supportsLimitedOuterJoins = false;
    supportsSchemasInDataManipulation = false;
    supportsSchemasInProcedureCalls = false;
    supportsSchemasInTableDefinitions = false;
    supportsSchemasInIndexDefinitions = false;
    supportsSchemasInPrivilegeDefinitions = false;
    supportsCatalogsInDataManipulation = false;
    supportsCatalogsInProcedureCalls = false;
    supportsCatalogsInTableDefinitions = false;
    supportsCatalogsInIndexDefinitions = false;
    supportsCatalogsInPrivilegeDefinitions = false;
    supportsPositionedDelete = false;
    supportsPositionedUpdate = false;
    supportsOpenCursorsAcrossCommit = false;
    supportsOpenCursorsAcrossRollback = false;
    supportsOpenStatementsAcrossCommit = false;
    supportsOpenStatementsAcrossRollback = false;
    locatorsUpdateCopy = false;
    supportsStatementPooling = false;
    autoCommitFailureClosesAllResultSets = false;
    supportsStoredFunctionsUsingCallSyntax = false;
    generatedKeyAlwaysReturned = false;

    // === Целочисленные значения ===
    supportsTransactionIsolationLevel = 0;
    nullCollation = 0;
    sqlStateType = 0;
    defaultTransactionIsolation = 0;
    resultSetHoldability = 0;
    rowIdLifetime = 0;
    maxTableNameLength = 0;
    maxSchemaNameLength = 0;
    maxCatalogNameLength = 0;
    maxColumnNameLength = 0;
    maxColumnsInGroupBy = 0;
    maxColumnsInOrderBy = 0;
    maxColumnsInSelect = 0;
    maxColumnsInTable = 0;
    maxColumnsInIndex = 0;
    maxStatementLength = 0;
    maxStatements = 0;
    maxTablesInSelect = 0;
    maxUserNameLength = 0;
    maxRowSize = 0;
    databaseMajorVersion = 0;
    databaseMinorVersion = 0;
    driverMajorVersion = 0;
    driverMinorVersion = 0;
}