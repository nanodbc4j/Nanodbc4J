package io.github.nanodbc4j.jdbc;

import io.github.nanodbc4j.exceptions.NanodbcSQLException;
import io.github.nanodbc4j.exceptions.NativeException;
import io.github.nanodbc4j.internal.dto.DatabaseMetaDataDto;
import io.github.nanodbc4j.internal.handler.DatabaseMetaDataHandler;
import io.github.nanodbc4j.internal.pointer.ResultSetPtr;

import java.lang.ref.WeakReference;
import java.sql.Connection;
import java.sql.DatabaseMetaData;
import java.sql.ResultSet;
import java.sql.RowIdLifetime;
import java.sql.SQLException;
import java.util.Arrays;
import java.util.Objects;
import java.util.function.Predicate;
import java.util.stream.Collectors;

public class NanodbcDatabaseMetaData implements DatabaseMetaData {
    private final WeakReference<NanodbcConnection> connection;
    private final DatabaseMetaDataDto metaData;

    public NanodbcDatabaseMetaData(NanodbcConnection connection, DatabaseMetaDataDto metaData) {
        this.connection = new WeakReference<>(connection);
        this.metaData = metaData;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean allProceduresAreCallable() throws SQLException {
        return metaData.allProceduresAreCallable;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean allTablesAreSelectable() throws SQLException {
        return metaData.allTablesAreSelectable;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String getURL() throws SQLException {
        return connection.get().getUrl();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String getUserName() throws SQLException {
        return metaData.userName != null ? metaData.userName : null;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean isReadOnly() throws SQLException {
        return metaData.isReadOnly;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean nullsAreSortedHigh() throws SQLException {
        return metaData.nullsAreSortedHigh;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean nullsAreSortedLow() throws SQLException {
        return metaData.nullsAreSortedLow;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean nullsAreSortedAtStart() throws SQLException {
        return metaData.nullsAreSortedAtStart;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean nullsAreSortedAtEnd() throws SQLException {
        return metaData.nullsAreSortedAtEnd;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String getDatabaseProductName() throws SQLException {
        return metaData.databaseProductName;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String getDatabaseProductVersion() throws SQLException {
        return metaData.databaseProductVersion;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String getDriverName() throws SQLException {
        return "JDBC-ODBC Bridge (" + metaData.driverName + ")";
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String getDriverVersion() throws SQLException {
        return metaData.driverVersion;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getDriverMajorVersion() {
        return metaData.driverMajorVersion;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getDriverMinorVersion() {
        return metaData.driverMinorVersion;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean usesLocalFiles() throws SQLException {
        return metaData.usesLocalFiles;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean usesLocalFilePerTable() throws SQLException {
        return metaData.usesLocalFilePerTable;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsMixedCaseIdentifiers() throws SQLException {
        return metaData.supportsMixedCaseIdentifiers;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean storesUpperCaseIdentifiers() throws SQLException {
        return metaData.storesUpperCaseIdentifiers;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean storesLowerCaseIdentifiers() throws SQLException {
        return metaData.storesLowerCaseIdentifiers;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean storesMixedCaseIdentifiers() throws SQLException {
        return metaData.storesMixedCaseIdentifiers;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsMixedCaseQuotedIdentifiers() throws SQLException {
        return metaData.supportsMixedCaseQuotedIdentifiers;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean storesUpperCaseQuotedIdentifiers() throws SQLException {
        return metaData.storesUpperCaseQuotedIdentifiers;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean storesLowerCaseQuotedIdentifiers() throws SQLException {
        return metaData.storesLowerCaseQuotedIdentifiers;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean storesMixedCaseQuotedIdentifiers() throws SQLException {
        return metaData.storesMixedCaseQuotedIdentifiers;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String getIdentifierQuoteString() throws SQLException {
        return metaData.identifierQuoteString != null ? metaData.identifierQuoteString : "";
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String getSQLKeywords() throws SQLException {
        return metaData.sqlKeywords != null ? metaData.sqlKeywords : "";
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String getNumericFunctions() throws SQLException {
        return metaData.numericFunctions != null ? metaData.numericFunctions : "";
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String getStringFunctions() throws SQLException {
        return metaData.stringFunctions != null ? metaData.stringFunctions : "";
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String getSystemFunctions() throws SQLException {
        return metaData.systemFunctions != null ? metaData.systemFunctions : "";
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String getTimeDateFunctions() throws SQLException {
        return metaData.timeDateFunctions != null ? metaData.timeDateFunctions : "";
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String getSearchStringEscape() throws SQLException {
        return metaData.searchStringEscape != null ? metaData.searchStringEscape : "";
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String getExtraNameCharacters() throws SQLException {
        return metaData.extraNameCharacters != null ? metaData.extraNameCharacters : "";
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsAlterTableWithAddColumn() throws SQLException {
        return metaData.supportsAlterTableWithAddColumn;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsAlterTableWithDropColumn() throws SQLException {
        return metaData.supportsAlterTableWithDropColumn;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsColumnAliasing() throws SQLException {
        return metaData.supportsColumnAliasing;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean nullPlusNonNullIsNull() throws SQLException {
        return metaData.nullPlusNonNullIsNull;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsConvert() throws SQLException {
        return metaData.supportsConvert;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsConvert(int fromType, int toType) throws SQLException {
        try {
            return DatabaseMetaDataHandler.supportsConvert(connection.get().getConnectionPtr(), fromType, toType);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsTableCorrelationNames() throws SQLException {
        return metaData.supportsTableCorrelationNames;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsDifferentTableCorrelationNames() throws SQLException {
        return metaData.supportsDifferentTableCorrelationNames;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsExpressionsInOrderBy() throws SQLException {
        return metaData.supportsExpressionsInOrderBy;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsOrderByUnrelated() throws SQLException {
        return metaData.supportsOrderByUnrelated;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsGroupBy() throws SQLException {
        return metaData.supportsGroupBy;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsGroupByUnrelated() throws SQLException {
        return metaData.supportsGroupByUnrelated;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsGroupByBeyondSelect() throws SQLException {
        return metaData.supportsGroupByBeyondSelect;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsLikeEscapeClause() throws SQLException {
        return metaData.supportsLikeEscapeClause;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsMultipleResultSets() throws SQLException {
        return metaData.supportsMultipleResultSets;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsMultipleTransactions() throws SQLException {
        return metaData.supportsMultipleTransactions;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsNonNullableColumns() throws SQLException {
        return metaData.supportsNonNullableColumns;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsMinimumSQLGrammar() throws SQLException {
        return metaData.supportsMinimumSQLGrammar;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsCoreSQLGrammar() throws SQLException {
        return metaData.supportsCoreSQLGrammar;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsExtendedSQLGrammar() throws SQLException {
        return metaData.supportsExtendedSQLGrammar;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsANSI92EntryLevelSQL() throws SQLException {
        return metaData.supportsANSI92EntryLevelSQL;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsANSI92IntermediateSQL() throws SQLException {
        return metaData.supportsANSI92IntermediateSQL;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsANSI92FullSQL() throws SQLException {
        return metaData.supportsANSI92FullSQL;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsIntegrityEnhancementFacility() throws SQLException {
        return metaData.supportsIntegrityEnhancementFacility;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsOuterJoins() throws SQLException {
        return metaData.supportsOuterJoins;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsFullOuterJoins() throws SQLException {
        return metaData.supportsFullOuterJoins;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsLimitedOuterJoins() throws SQLException {
        return metaData.supportsLimitedOuterJoins;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String getSchemaTerm() throws SQLException {
        return metaData.schemaTerm != null ? metaData.schemaTerm : "SCHEMA";
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String getProcedureTerm() throws SQLException {
        return metaData.procedureTerm != null ? metaData.procedureTerm : "PROCEDURE";
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String getCatalogTerm() throws SQLException {
        return metaData.catalogTerm != null ? metaData.catalogTerm : "CATALOG";
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean isCatalogAtStart() throws SQLException {
        return metaData.isCatalogAtStart;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String getCatalogSeparator() throws SQLException {
        return metaData.catalogSeparator != null ? metaData.catalogSeparator : ".";
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsSchemasInDataManipulation() throws SQLException {
        return metaData.supportsSchemasInDataManipulation;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsSchemasInProcedureCalls() throws SQLException {
        return metaData.supportsSchemasInProcedureCalls;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsSchemasInTableDefinitions() throws SQLException {
        return metaData.supportsSchemasInTableDefinitions;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsSchemasInIndexDefinitions() throws SQLException {
        return metaData.supportsSchemasInIndexDefinitions;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsSchemasInPrivilegeDefinitions() throws SQLException {
        return metaData.supportsSchemasInPrivilegeDefinitions;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsCatalogsInDataManipulation() throws SQLException {
        return metaData.supportsCatalogsInDataManipulation;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsCatalogsInProcedureCalls() throws SQLException {
        return metaData.supportsCatalogsInProcedureCalls;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsCatalogsInTableDefinitions() throws SQLException {
        return metaData.supportsCatalogsInTableDefinitions;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsCatalogsInIndexDefinitions() throws SQLException {
        return metaData.supportsCatalogsInIndexDefinitions;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsCatalogsInPrivilegeDefinitions() throws SQLException {
        return metaData.supportsCatalogsInPrivilegeDefinitions;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsPositionedDelete() throws SQLException {
        return metaData.supportsPositionedDelete;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsPositionedUpdate() throws SQLException {
        return metaData.supportsPositionedUpdate;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsSelectForUpdate() throws SQLException {
        return metaData.supportsSelectForUpdate;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsStoredProcedures() throws SQLException {
        return metaData.supportsStoredProcedures;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsSubqueriesInComparisons() throws SQLException {
        return metaData.supportsSubqueriesInComparisons;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsSubqueriesInExists() throws SQLException {
        return metaData.supportsSubqueriesInExists;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsSubqueriesInIns() throws SQLException {
        return metaData.supportsSubqueriesInIns;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsSubqueriesInQuantifieds() throws SQLException {
        return metaData.supportsSubqueriesInQuantifieds;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsCorrelatedSubqueries() throws SQLException {
        return metaData.supportsCorrelatedSubqueries;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsUnion() throws SQLException {
        return metaData.supportsUnion;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsUnionAll() throws SQLException {
        return metaData.supportsUnionAll;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsOpenCursorsAcrossCommit() throws SQLException {
        return metaData.supportsOpenCursorsAcrossCommit;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsOpenCursorsAcrossRollback() throws SQLException {
        return metaData.supportsOpenCursorsAcrossRollback;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsOpenStatementsAcrossCommit() throws SQLException {
        return metaData.supportsOpenStatementsAcrossCommit;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsOpenStatementsAcrossRollback() throws SQLException {
        return metaData.supportsOpenStatementsAcrossRollback;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getMaxBinaryLiteralLength() throws SQLException {
        return metaData.getMaxBinaryLiteralLength;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getMaxCharLiteralLength() throws SQLException {
        return metaData.getMaxCharLiteralLength;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getMaxColumnNameLength() throws SQLException {
        return metaData.maxColumnNameLength;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getMaxColumnsInGroupBy() throws SQLException {
        return metaData.maxColumnsInGroupBy;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getMaxColumnsInIndex() throws SQLException {
        return metaData.maxColumnsInIndex;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getMaxColumnsInOrderBy() throws SQLException {
        return metaData.maxColumnsInOrderBy;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getMaxColumnsInSelect() throws SQLException {
        return metaData.maxColumnsInSelect;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getMaxColumnsInTable() throws SQLException {
        return metaData.maxColumnsInTable;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getMaxConnections() throws SQLException {
        return metaData.getMaxConnections;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getMaxCursorNameLength() throws SQLException {
        return metaData.getMaxCursorNameLength;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getMaxIndexLength() throws SQLException {
        return metaData.getMaxIndexLength;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getMaxSchemaNameLength() throws SQLException {
        return metaData.maxSchemaNameLength;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getMaxProcedureNameLength() throws SQLException {
        return metaData.getMaxProcedureNameLength;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getMaxCatalogNameLength() throws SQLException {
        return metaData.maxCatalogNameLength;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getMaxRowSize() throws SQLException {
        return metaData.maxRowSize;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean doesMaxRowSizeIncludeBlobs() throws SQLException {
        return metaData.doesMaxRowSizeIncludeBlobs;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getMaxStatementLength() throws SQLException {
        return metaData.maxStatementLength;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getMaxStatements() throws SQLException {
        return metaData.maxStatements;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getMaxTableNameLength() throws SQLException {
        return metaData.maxTableNameLength;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getMaxTablesInSelect() throws SQLException {
        return metaData.maxTablesInSelect;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getMaxUserNameLength() throws SQLException {
        return metaData.maxUserNameLength;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getDefaultTransactionIsolation() throws SQLException {
        return metaData.defaultTransactionIsolation;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsTransactions() throws SQLException {
        return metaData.supportsTransactions;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsTransactionIsolationLevel(int level) throws SQLException {
        return (metaData.supportsTransactionIsolationLevel & level) != 0;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsDataDefinitionAndDataManipulationTransactions() throws SQLException {
        return metaData.supportsDataDefinitionAndDataManipulationTransactions;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsDataManipulationTransactionsOnly() throws SQLException {
        return metaData.supportsDataManipulationTransactionsOnly;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean dataDefinitionCausesTransactionCommit() throws SQLException {
        return metaData.dataDefinitionCausesTransactionCommit;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean dataDefinitionIgnoredInTransactions() throws SQLException {
        return metaData.dataDefinitionIgnoredInTransactions;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public ResultSet getProcedures(String catalog, String schemaPattern, String procedureNamePattern) throws SQLException {
        try {
            ResultSetPtr resultSetPtr = DatabaseMetaDataHandler.getProcedures(connection.get().getConnectionPtr(), catalog, schemaPattern, procedureNamePattern);
            return new NanodbcResultSet(resultSetPtr);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public ResultSet getProcedureColumns(String catalog, String schemaPattern, String procedureNamePattern, String columnNamePattern) throws SQLException {
        try {
            ResultSetPtr resultSetPtr = DatabaseMetaDataHandler.getProcedureColumns(connection.get().getConnectionPtr(), catalog, schemaPattern, procedureNamePattern, columnNamePattern);
            return new NanodbcResultSet(resultSetPtr);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public ResultSet getTables(String catalog, String schemaPattern, String tableNamePattern, String[] types) throws SQLException {
        // https://msdn.microsoft.com/en-us/library/ms710171.aspx
        String typePattern = types == null ? "" : Arrays.stream(types)
                .filter(Objects::nonNull)
                .map(String::trim)
                .filter(Predicate.not(String::isEmpty))
                .collect(Collectors.joining(","));
        try {
            ResultSetPtr resultSetPtr = DatabaseMetaDataHandler.getTables(connection.get().getConnectionPtr(), catalog, schemaPattern, tableNamePattern, typePattern);
            return new NanodbcResultSet(resultSetPtr);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public ResultSet getSchemas() throws SQLException {
        try {
            ResultSetPtr resultSetPtr = DatabaseMetaDataHandler.getSchemas(connection.get().getConnectionPtr());
            return new NanodbcResultSet(resultSetPtr);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public ResultSet getCatalogs() throws SQLException {
        try {
            ResultSetPtr resultSetPtr = DatabaseMetaDataHandler.getCatalogs(connection.get().getConnectionPtr());
            return new NanodbcResultSet(resultSetPtr);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public ResultSet getTableTypes() throws SQLException {
        try {
            ResultSetPtr resultSetPtr = DatabaseMetaDataHandler.getTableTypes(connection.get().getConnectionPtr());
            return new NanodbcResultSet(resultSetPtr);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public ResultSet getColumns(String catalog, String schemaPattern, String tableNamePattern, String columnNamePattern) throws SQLException {
        try {
            ResultSetPtr resultSetPtr =
                    DatabaseMetaDataHandler.getColumns(connection.get().getConnectionPtr(), catalog, schemaPattern, tableNamePattern, columnNamePattern);
            return new NanodbcResultSet(resultSetPtr);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public ResultSet getColumnPrivileges(String catalog, String schema, String table, String columnNamePattern) throws SQLException {
        try {
            ResultSetPtr resultSetPtr =
                    DatabaseMetaDataHandler.getColumnPrivileges(connection.get().getConnectionPtr(), catalog, schema, table, columnNamePattern);
            return new NanodbcResultSet(resultSetPtr);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public ResultSet getTablePrivileges(String catalog, String schemaPattern, String tableNamePattern) throws SQLException {
        try {
            ResultSetPtr resultSetPtr =
                    DatabaseMetaDataHandler.getTablePrivileges(connection.get().getConnectionPtr(), catalog, schemaPattern, tableNamePattern);
            return new NanodbcResultSet(resultSetPtr);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public ResultSet getBestRowIdentifier(String catalog, String schema, String table, int scope, boolean nullable) throws SQLException {
        try {
            ResultSetPtr resultSetPtr =
                    DatabaseMetaDataHandler.getBestRowIdentifier(connection.get().getConnectionPtr(), catalog, schema, table, scope, nullable);
            return new NanodbcResultSet(resultSetPtr);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public ResultSet getVersionColumns(String catalog, String schema, String table) throws SQLException {
        try {
            ResultSetPtr resultSetPtr =
                    DatabaseMetaDataHandler.getVersionColumns(connection.get().getConnectionPtr(), catalog, schema, table);
            return new NanodbcResultSet(resultSetPtr);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public ResultSet getPrimaryKeys(String catalog, String schema, String table) throws SQLException {
        try {
            ResultSetPtr resultSetPtr =
                    DatabaseMetaDataHandler.getPrimaryKeys(connection.get().getConnectionPtr(), catalog, schema, table);
            return new NanodbcResultSet(resultSetPtr);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public ResultSet getImportedKeys(String catalog, String schema, String table) throws SQLException {
        try {
            ResultSetPtr resultSetPtr =
                    DatabaseMetaDataHandler.getImportedKeys(connection.get().getConnectionPtr(), catalog, schema, table);
            return new NanodbcResultSet(resultSetPtr);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public ResultSet getExportedKeys(String catalog, String schema, String table) throws SQLException {
        try {
            ResultSetPtr resultSetPtr =
                    DatabaseMetaDataHandler.getExportedKeys(connection.get().getConnectionPtr(), catalog, schema, table);
            return new NanodbcResultSet(resultSetPtr);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public ResultSet getCrossReference(String parentCatalog, String parentSchema, String parentTable, String foreignCatalog, String foreignSchema, String foreignTable) throws SQLException {
        try {
            ResultSetPtr resultSetPtr =
                    DatabaseMetaDataHandler.getCrossReference(connection.get().getConnectionPtr(), parentCatalog, parentSchema, parentTable,
                    foreignCatalog, foreignSchema, foreignTable);
            return new NanodbcResultSet(resultSetPtr);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public ResultSet getTypeInfo() throws SQLException {
        try {
            ResultSetPtr resultSetPtr = DatabaseMetaDataHandler.getTypeInfo(connection.get().getConnectionPtr());
            return new NanodbcResultSet(resultSetPtr);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public ResultSet getIndexInfo(String catalog, String schema, String table, boolean unique, boolean approximate) throws SQLException {
        try {
            ResultSetPtr resultSetPtr =
                    DatabaseMetaDataHandler.getIndexInfo(connection.get().getConnectionPtr(), catalog, schema, table, unique, approximate);
            return new NanodbcResultSet(resultSetPtr);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsResultSetType(int type) throws SQLException {
        return switch (type) {
            case ResultSet.TYPE_FORWARD_ONLY, ResultSet.TYPE_SCROLL_INSENSITIVE -> true;
            default -> false;
        };
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsResultSetConcurrency(int type, int concurrency) throws SQLException {
        return concurrency == ResultSet.CONCUR_READ_ONLY;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean ownUpdatesAreVisible(int type) throws SQLException {
        return false;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean ownDeletesAreVisible(int type) throws SQLException {
        return false;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean ownInsertsAreVisible(int type) throws SQLException {
        return false;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean othersUpdatesAreVisible(int type) throws SQLException {
        return false;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean othersDeletesAreVisible(int type) throws SQLException {
        return false;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean othersInsertsAreVisible(int type) throws SQLException {
        return false;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean updatesAreDetected(int type) throws SQLException {
        return false;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean deletesAreDetected(int type) throws SQLException {
        return false;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean insertsAreDetected(int type) throws SQLException {
        return false;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsBatchUpdates() throws SQLException {
        return metaData.supportsBatchUpdates;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public ResultSet getUDTs(String catalog, String schemaPattern, String typeNamePattern, int[] types) throws SQLException {
        return null;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Connection getConnection() {
        return connection.get();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsSavepoints() throws SQLException {
        return metaData.supportsSavepoints;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsNamedParameters() throws SQLException {
        return metaData.supportsNamedParameters;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsMultipleOpenResults() throws SQLException {
        return metaData.supportsMultipleOpenResults;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsGetGeneratedKeys() throws SQLException {
        return metaData.supportsGetGeneratedKeys;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public ResultSet getSuperTypes(String catalog, String schemaPattern, String typeNamePattern) throws SQLException {
        // sun.jdbc.odbc does not support this
        throw new UnsupportedOperationException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public ResultSet getSuperTables(String catalog, String schemaPattern, String tableNamePattern) throws SQLException {
        // sun.jdbc.odbc does not support this
        throw new UnsupportedOperationException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public ResultSet getAttributes(String catalog, String schemaPattern, String typeNamePattern, String attributeNamePattern) throws SQLException {
        // sun.jdbc.odbc does not support this
        throw new UnsupportedOperationException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsResultSetHoldability(int holdability) throws SQLException {
        return holdability == ResultSet.HOLD_CURSORS_OVER_COMMIT;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getResultSetHoldability() throws SQLException {
        return metaData.resultSetHoldability;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getDatabaseMajorVersion() throws SQLException {
        return metaData.databaseMajorVersion;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getDatabaseMinorVersion() throws SQLException {
        return metaData.databaseMinorVersion;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getJDBCMajorVersion() throws SQLException {
        return NanodbcDriver.MAJOR_VERSION;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getJDBCMinorVersion() throws SQLException {
        return NanodbcDriver.MINOR_VERSION;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getSQLStateType() throws SQLException {
        return metaData.sqlStateType;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean locatorsUpdateCopy() throws SQLException {
        return metaData.locatorsUpdateCopy;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsStatementPooling() throws SQLException {
        return metaData.supportsStatementPooling;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public RowIdLifetime getRowIdLifetime() throws SQLException {
        // sun.jdbc.odbc does not support this
        throw new UnsupportedOperationException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public ResultSet getSchemas(String catalog, String schemaPattern) throws SQLException {
        // sun.jdbc.odbc does not support this
        throw new UnsupportedOperationException("getSchemas with parameters is not supported");
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsStoredFunctionsUsingCallSyntax() throws SQLException {
        return metaData.supportsStoredFunctionsUsingCallSyntax;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean autoCommitFailureClosesAllResultSets() throws SQLException {
        return metaData.autoCommitFailureClosesAllResultSets;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public ResultSet getClientInfoProperties() throws SQLException {
        // sun.jdbc.odbc does not support this
        throw new UnsupportedOperationException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public ResultSet getFunctions(String catalog, String schemaPattern, String functionNamePattern) throws SQLException {
        // sun.jdbc.odbc does not support this
        throw new UnsupportedOperationException("Operation not yet supported");
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public ResultSet getFunctionColumns(String catalog, String schemaPattern, String functionNamePattern, String columnNamePattern) throws SQLException {
        // sun.jdbc.odbc does not support this
        throw new UnsupportedOperationException("Operation not yet supported");
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public ResultSet getPseudoColumns(String catalog, String schemaPattern, String tableNamePattern, String columnNamePattern) throws SQLException {
        // sun.jdbc.odbc does not support this
        throw new UnsupportedOperationException("Operation not yet supported");
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean generatedKeyAlwaysReturned() throws SQLException {
        return metaData.generatedKeyAlwaysReturned;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public <T> T unwrap(Class<T> iface) throws SQLException {
        if (isWrapperFor(iface)) {
            return iface.cast(this);
        }
        throw new NanodbcSQLException("Cannot unwrap to " + iface.getName());
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean isWrapperFor(Class<?> iface) throws SQLException {
        return iface.isInstance(this) || iface == DatabaseMetaData.class;
    }

    @Override
    public String toString() {
        return "NanodbcDatabaseMetaData{" +
                "metaData=" + metaData +
                '}';
    }
}
