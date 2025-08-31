#pragma once

#include <wtypes.h>
#include <sqlext.h>
#include <nanodbc/nanodbc.h>

class DatabaseMetaData {
private:
    nanodbc::connection& connection_;
    SQLHDBC hdbc_;

public:
    DatabaseMetaData(const nanodbc::connection& connection);

    // === Строковые методы ===
    std::wstring getDatabaseProductName() const;
    std::wstring getDatabaseProductVersion() const;
    std::wstring getDriverName() const;
    std::wstring getDriverVersion() const;
    std::wstring getIdentifierQuoteString() const;
    std::wstring getSchemaTerm() const;
    std::wstring getProcedureTerm() const;
    std::wstring getCatalogTerm() const;
    std::wstring getCatalogSeparator() const;
    std::wstring getSQLKeywords() const;
    std::wstring getNumericFunctions() const;
    std::wstring getStringFunctions() const;
    std::wstring getSystemFunctions() const;
    std::wstring getTimeDateFunctions() const;
    std::wstring getSearchStringEscape() const;
    std::wstring getExtraNameCharacters() const;

    // === Булевы методы ===
    bool isReadOnly() const;
    bool supportsTransactions() const;
    bool supportsSavepoints() const;
    bool supportsNamedParameters() const;
    bool supportsBatchUpdates() const;
    bool supportsUnion() const;
    bool supportsUnionAll() const;
    bool supportsLikeEscapeClause() const;
    bool supportsGroupBy() const;
    bool supportsGroupByUnrelated() const;
    bool supportsGroupByBeyondSelect() const;
    bool supportsOrderByUnrelated() const;
    bool supportsAlterTableWithAddColumn() const;
    bool supportsColumnAliasing() const;
    bool nullPlusNonNullIsNull() const;
    bool supportsExpressionsInOrderBy() const;
    bool supportsSelectForUpdate() const;
    bool supportsStoredProcedures() const;
    bool supportsSubqueriesInComparisons() const;
    bool supportsSubqueriesInExists() const;
    bool supportsSubqueriesInIns() const;
    bool supportsSubqueriesInQuantifieds() const;
    bool supportsCorrelatedSubqueries() const;
    bool supportsIntegrityEnhancementFacility() const;
    bool supportsOuterJoins() const;
    bool supportsFullOuterJoins() const;
    bool supportsLimitedOuterJoins() const;
    bool supportsSchemasInDataManipulation() const;
    bool supportsSchemasInProcedureCalls() const;
    bool supportsSchemasInTableDefinitions() const;
    bool supportsSchemasInIndexDefinitions() const;
    bool supportsSchemasInPrivilegeDefinitions() const;
    bool supportsCatalogsInDataManipulation() const;
    bool supportsCatalogsInProcedureCalls() const;
    bool supportsCatalogsInTableDefinitions() const;
    bool supportsCatalogsInIndexDefinitions() const;
    bool supportsCatalogsInPrivilegeDefinitions() const;
    bool supportsPositionedDelete() const;
    bool supportsPositionedUpdate() const;
    bool supportsOpenCursorsAcrossCommit() const;
    bool supportsOpenCursorsAcrossRollback() const;
    bool supportsOpenStatementsAcrossCommit() const;
    bool supportsOpenStatementsAcrossRollback() const;
    bool locatorsUpdateCopy() const;
    bool supportsStatementPooling() const;
    bool autoCommitFailureClosesAllResultSets() const;
    bool supportsStoredFunctionsUsingCallSyntax() const;
    bool generatedKeyAlwaysReturned() const;

    // === Целочисленные методы ===
    int supportsTransactionIsolationLevel() const;
    int getNullCollation() const;
    int getSQLStateType() const;
    int getDefaultTransactionIsolation() const;
    int getResultSetHoldability() const;
    int getRowIdLifetime() const;

    int getMaxTableNameLength() const;
    int getMaxSchemaNameLength() const;
    int getMaxCatalogNameLength() const;
    int getMaxColumnNameLength() const;
    int getMaxColumnsInGroupBy() const;
    int getMaxColumnsInOrderBy() const;
    int getMaxColumnsInSelect() const;
    int getMaxColumnsInTable() const;
    int getMaxColumnsInIndex() const;
    int getMaxStatementLength() const;
    int getMaxStatements() const;
    int getMaxTablesInSelect() const;
    int getMaxUserNameLength() const;
    int getMaxRowSize() const;
    int getDatabaseMajorVersion() const;
    int getDatabaseMinorVersion() const;
    int getDriverMajorVersion() const;
    int getDriverMinorVersion() const;

    /*

    // === Методы, возвращающие result (каталоги) ===
    nanodbc::result getTables(const std::wstring& catalog, const std::wstring& schema,
        const std::wstring& table, const std::wstring& type) const;

    nanodbc::result getColumns(const std::wstring& catalog, const std::wstring& schema,
        const std::wstring& table, const std::wstring& column) const;

    nanodbc::result getPrimaryKeys(const std::wstring& catalog, const std::wstring& schema,
        const std::wstring& table) const;

    nanodbc::result getImportedKeys(const std::wstring& catalog, const std::wstring& schema,
        const std::wstring& table) const;

    nanodbc::result getTypeInfo(short sqlType = 0) const;

    nanodbc::result getProcedures(const std::wstring& catalog, const std::wstring& schema,
        const std::wstring& procedure) const;

    nanodbc::result getProcedureColumns(const std::wstring& catalog, const std::wstring& schema,
        const std::wstring& procedure, const std::wstring& column) const;
     */
};