#pragma once

#include <nanodbc/nanodbc.h>

class DatabaseMetaData {
    nanodbc::connection& connection_;

public:
    explicit DatabaseMetaData(nanodbc::connection& connection);

    // === Строковые методы ===
    nanodbc::string getDatabaseProductName() const;
    nanodbc::string getDatabaseProductVersion() const;
    nanodbc::string getDriverName() const;
    nanodbc::string getDriverVersion() const;
    nanodbc::string getIdentifierQuoteString() const;
    nanodbc::string getSchemaTerm() const;
    nanodbc::string getProcedureTerm() const;
    nanodbc::string getCatalogTerm() const;
    nanodbc::string getCatalogSeparator() const;
    nanodbc::string getSQLKeywords() const;
    nanodbc::string getNumericFunctions() const;
    nanodbc::string getStringFunctions() const;
    nanodbc::string getSystemFunctions() const;
    nanodbc::string getTimeDateFunctions() const;
    nanodbc::string getSearchStringEscape() const;
    nanodbc::string getExtraNameCharacters() const;
    nanodbc::string getUserName() const;

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
    bool allProceduresAreCallable() const;
    bool allTablesAreSelectable() const;
    bool supportsMultipleResultSets() const;
    bool supportsMultipleTransactions() const;
    bool autoCommitFailureClosesAllResultSets() const;
    bool supportsStoredFunctionsUsingCallSyntax() const;
    bool generatedKeyAlwaysReturned() const;
    bool nullsAreSortedHigh() const;
    bool nullsAreSortedLow() const;
    bool nullsAreSortedAtStart() const;
    bool nullsAreSortedAtEnd() const;
    bool usesLocalFiles() const;
    bool usesLocalFilePerTable() const;
    bool supportsMixedCaseIdentifiers() const;
    bool storesUpperCaseIdentifiers() const;
    bool storesLowerCaseIdentifiers() const;
    bool storesMixedCaseIdentifiers() const;
    bool supportsMixedCaseQuotedIdentifiers() const;
    bool storesUpperCaseQuotedIdentifiers() const;
    bool storesLowerCaseQuotedIdentifiers() const;
    bool storesMixedCaseQuotedIdentifiers() const;
    bool supportsAlterTableWithDropColumn() const;
    bool supportsConvert() const;
    bool supportsConvert(int fromType, int toType) const;
    bool supportsTableCorrelationNames() const;
    bool supportsDifferentTableCorrelationNames() const;
    bool supportsNonNullableColumns() const;
    bool supportsMinimumSQLGrammar() const;
    bool supportsCoreSQLGrammar() const;
    bool supportsExtendedSQLGrammar() const;
    bool supportsANSI92EntryLevelSQL() const;
    bool supportsANSI92IntermediateSQL() const;
    bool supportsANSI92FullSQL() const;
    bool supportsDataDefinitionAndDataManipulationTransactions() const;
    bool supportsDataManipulationTransactionsOnly() const;
    bool dataDefinitionCausesTransactionCommit() const;
    bool dataDefinitionIgnoredInTransactions() const;
    bool supportsMultipleOpenResults() const;
    bool supportsGetGeneratedKeys() const;
    bool doesMaxRowSizeIncludeBlobs() const;
    bool isCatalogAtStart() const;

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
    int getMaxBinaryLiteralLength() const;
    int getMaxCharLiteralLength() const;
    int getMaxConnections() const;
    int getMaxCursorNameLength() const;
    int getMaxIndexLength() const;
    int getMaxProcedureNameLength() const;

    nanodbc::result getSchemas() const;
    nanodbc::result getCatalogs() const;
    nanodbc::result getTableTypes() const;
    nanodbc::result getTables(const nanodbc::string& catalog, const nanodbc::string& schema,  const nanodbc::string& table, const nanodbc::string& type) const;    
    nanodbc::result getColumns(const nanodbc::string& catalog, const nanodbc::string& schema, const nanodbc::string& table, const nanodbc::string& column) const;
    nanodbc::result getPrimaryKeys(const nanodbc::string& catalog, const nanodbc::string& schema,  const nanodbc::string& table) const;
    nanodbc::result getImportedKeys(const nanodbc::string& catalog, const nanodbc::string& schema,  const nanodbc::string& table) const;
    nanodbc::result getExportedKeys(const nanodbc::string& catalog, const nanodbc::string& schema, const nanodbc::string& table) const;
    nanodbc::result getTypeInfo() const;
    nanodbc::result getColumnPrivileges(const nanodbc::string& catalog, const nanodbc::string& schema, const nanodbc::string& table, const nanodbc::string& columnNamePattern) const;
    nanodbc::result getTablePrivileges(const nanodbc::string& catalog, const nanodbc::string& schemaPattern, const nanodbc::string& tableNamePattern) const;
    nanodbc::result getBestRowIdentifier(const nanodbc::string& catalog, const nanodbc::string& schema, const nanodbc::string& table, int scope, bool nullable) const;
    nanodbc::result getVersionColumns(const nanodbc::string& catalog, const nanodbc::string& schema, const nanodbc::string& table) const;
    nanodbc::result getCrossReference(const nanodbc::string& parentCatalog, const nanodbc::string& parentSchema, const nanodbc::string& parentTable,
                                      const nanodbc::string& foreignCatalog, const nanodbc::string& foreignSchema, const nanodbc::string& foreignTable) const;
    nanodbc::result getIndexInfo(const nanodbc::string& catalog, const nanodbc::string& schema, const nanodbc::string& table, bool unique, bool approximate) const;
    nanodbc::result getProcedures(const nanodbc::string& catalog, const nanodbc::string& schema, const nanodbc::string& procedure) const;
    nanodbc::result getProcedureColumns(const nanodbc::string& catalog, const nanodbc::string& schema, const nanodbc::string& procedure, const nanodbc::string& column) const;

};