package io.github.michael1297.jdbc;

import io.github.michael1297.exceptions.NanodbcSQLException;
import io.github.michael1297.exceptions.NativeException;
import io.github.michael1297.internal.dto.OdbcDatabaseMetaData;
import io.github.michael1297.internal.handler.OdbcDatabaseMetaDataHandler;
import io.github.michael1297.internal.pointer.ResultSetPtr;

import java.lang.ref.WeakReference;
import java.sql.Connection;
import java.sql.DatabaseMetaData;
import java.sql.ResultSet;
import java.sql.RowIdLifetime;
import java.sql.SQLException;
import java.sql.SQLFeatureNotSupportedException;
import java.util.Arrays;
import java.util.Objects;
import java.util.stream.Collectors;

public class NanodbcDatabaseMetaData implements DatabaseMetaData {
    private final WeakReference<NanodbcConnection> connection;
    private final OdbcDatabaseMetaData metaData;

    public NanodbcDatabaseMetaData(NanodbcConnection connection, OdbcDatabaseMetaData metaData) {
        this.connection = new WeakReference<>(connection);
        this.metaData = metaData;
    }

    @Override
    public boolean allProceduresAreCallable() throws SQLException {
        return false;
    }

    @Override
    public boolean allTablesAreSelectable() throws SQLException {
        return false;
    }

    @Override
    public String getURL() throws SQLException {
        return "";
    }

    @Override
    public String getUserName() throws SQLException {
        return "";
    }

    @Override
    public boolean isReadOnly() throws SQLException {
        return metaData.isReadOnly;
    }

    @Override
    public boolean nullsAreSortedHigh() throws SQLException {
        return false;
    }

    @Override
    public boolean nullsAreSortedLow() throws SQLException {
        return false;
    }

    @Override
    public boolean nullsAreSortedAtStart() throws SQLException {
        return false;
    }

    @Override
    public boolean nullsAreSortedAtEnd() throws SQLException {
        return false;
    }

    @Override
    public String getDatabaseProductName() throws SQLException {
        return metaData.databaseProductName;
    }

    @Override
    public String getDatabaseProductVersion() throws SQLException {
        return metaData.databaseProductVersion;
    }

    @Override
    public String getDriverName() throws SQLException {
        return metaData.driverName;
    }

    @Override
    public String getDriverVersion() throws SQLException {
        return metaData.driverVersion;
    }

    @Override
    public int getDriverMajorVersion() {
        return metaData.driverMajorVersion;
    }

    @Override
    public int getDriverMinorVersion() {
        return metaData.driverMinorVersion;
    }

    @Override
    public boolean usesLocalFiles() throws SQLException {
        return false;
    }

    @Override
    public boolean usesLocalFilePerTable() throws SQLException {
        return false;
    }

    @Override
    public boolean supportsMixedCaseIdentifiers() throws SQLException {
        return false;
    }

    @Override
    public boolean storesUpperCaseIdentifiers() throws SQLException {
        return false;
    }

    @Override
    public boolean storesLowerCaseIdentifiers() throws SQLException {
        return false;
    }

    @Override
    public boolean storesMixedCaseIdentifiers() throws SQLException {
        return false;
    }

    @Override
    public boolean supportsMixedCaseQuotedIdentifiers() throws SQLException {
        return false;
    }

    @Override
    public boolean storesUpperCaseQuotedIdentifiers() throws SQLException {
        return false;
    }

    @Override
    public boolean storesLowerCaseQuotedIdentifiers() throws SQLException {
        return false;
    }

    @Override
    public boolean storesMixedCaseQuotedIdentifiers() throws SQLException {
        return false;
    }

    @Override
    public String getIdentifierQuoteString() throws SQLException {
        return metaData.identifierQuoteString != null ? metaData.identifierQuoteString : "";
    }

    @Override
    public String getSQLKeywords() throws SQLException {
        return metaData.sqlKeywords != null ? metaData.sqlKeywords : "";
    }

    @Override
    public String getNumericFunctions() throws SQLException {
        return metaData.numericFunctions != null ? metaData.numericFunctions : "";
    }

    @Override
    public String getStringFunctions() throws SQLException {
        return metaData.stringFunctions != null ? metaData.stringFunctions : "";
    }

    @Override
    public String getSystemFunctions() throws SQLException {
        return metaData.systemFunctions != null ? metaData.systemFunctions : "";
    }

    @Override
    public String getTimeDateFunctions() throws SQLException {
        return metaData.timeDateFunctions != null ? metaData.timeDateFunctions : "";
    }

    @Override
    public String getSearchStringEscape() throws SQLException {
        return metaData.searchStringEscape != null ? metaData.searchStringEscape : "";
    }

    @Override
    public String getExtraNameCharacters() throws SQLException {
        return metaData.extraNameCharacters != null ? metaData.extraNameCharacters : "";
    }

    @Override
    public boolean supportsAlterTableWithAddColumn() throws SQLException {
        return metaData.supportsAlterTableWithAddColumn;
    }

    @Override
    public boolean supportsAlterTableWithDropColumn() throws SQLException {
        return false;
    }

    @Override
    public boolean supportsColumnAliasing() throws SQLException {
        return metaData.supportsColumnAliasing;
    }

    @Override
    public boolean nullPlusNonNullIsNull() throws SQLException {
        return metaData.nullPlusNonNullIsNull;
    }

    @Override
    public boolean supportsConvert() throws SQLException {
        return false;
    }

    @Override
    public boolean supportsConvert(int fromType, int toType) throws SQLException {
        return false;
    }

    @Override
    public boolean supportsTableCorrelationNames() throws SQLException {
        return false;
    }

    @Override
    public boolean supportsDifferentTableCorrelationNames() throws SQLException {
        return false;
    }

    @Override
    public boolean supportsExpressionsInOrderBy() throws SQLException {
        return metaData.supportsExpressionsInOrderBy;
    }

    @Override
    public boolean supportsOrderByUnrelated() throws SQLException {
        return metaData.supportsOrderByUnrelated;
    }

    @Override
    public boolean supportsGroupBy() throws SQLException {
        return metaData.supportsGroupBy;
    }

    @Override
    public boolean supportsGroupByUnrelated() throws SQLException {
        return metaData.supportsGroupByUnrelated;
    }

    @Override
    public boolean supportsGroupByBeyondSelect() throws SQLException {
        return metaData.supportsGroupByBeyondSelect;
    }

    @Override
    public boolean supportsLikeEscapeClause() throws SQLException {
        return metaData.supportsLikeEscapeClause;
    }

    @Override
    public boolean supportsMultipleResultSets() throws SQLException {
        return false;
    }

    @Override
    public boolean supportsMultipleTransactions() throws SQLException {
        return false;
    }

    @Override
    public boolean supportsNonNullableColumns() throws SQLException {
        return false;
    }

    @Override
    public boolean supportsMinimumSQLGrammar() throws SQLException {
        return false;
    }

    @Override
    public boolean supportsCoreSQLGrammar() throws SQLException {
        return false;
    }

    @Override
    public boolean supportsExtendedSQLGrammar() throws SQLException {
        return false;
    }

    @Override
    public boolean supportsANSI92EntryLevelSQL() throws SQLException {
        return false;
    }

    @Override
    public boolean supportsANSI92IntermediateSQL() throws SQLException {
        return false;
    }

    @Override
    public boolean supportsANSI92FullSQL() throws SQLException {
        return false;
    }

    @Override
    public boolean supportsIntegrityEnhancementFacility() throws SQLException {
        return metaData.supportsIntegrityEnhancementFacility;
    }

    @Override
    public boolean supportsOuterJoins() throws SQLException {
        return metaData.supportsOuterJoins;
    }

    @Override
    public boolean supportsFullOuterJoins() throws SQLException {
        return metaData.supportsFullOuterJoins;
    }

    @Override
    public boolean supportsLimitedOuterJoins() throws SQLException {
        return metaData.supportsLimitedOuterJoins;
    }

    @Override
    public String getSchemaTerm() throws SQLException {
        return metaData.schemaTerm != null ? metaData.schemaTerm : "SCHEMA";
    }

    @Override
    public String getProcedureTerm() throws SQLException {
        return metaData.procedureTerm != null ? metaData.procedureTerm : "PROCEDURE";
    }

    @Override
    public String getCatalogTerm() throws SQLException {
        return metaData.catalogTerm != null ? metaData.catalogTerm : "CATALOG";
    }

    @Override
    public boolean isCatalogAtStart() throws SQLException {
        // Обычно да: CATALOG.SCHEMA.TABLE
        return true;
    }

    @Override
    public String getCatalogSeparator() throws SQLException {
        return metaData.catalogSeparator != null ? metaData.catalogSeparator : ".";
    }

    @Override
    public boolean supportsSchemasInDataManipulation() throws SQLException {
        return metaData.supportsSchemasInDataManipulation;
    }

    @Override
    public boolean supportsSchemasInProcedureCalls() throws SQLException {
        return metaData.supportsSchemasInProcedureCalls;
    }

    @Override
    public boolean supportsSchemasInTableDefinitions() throws SQLException {
        return metaData.supportsSchemasInTableDefinitions;
    }

    @Override
    public boolean supportsSchemasInIndexDefinitions() throws SQLException {
        return metaData.supportsSchemasInIndexDefinitions;
    }

    @Override
    public boolean supportsSchemasInPrivilegeDefinitions() throws SQLException {
        return metaData.supportsSchemasInPrivilegeDefinitions;
    }

    @Override
    public boolean supportsCatalogsInDataManipulation() throws SQLException {
        return metaData.supportsCatalogsInDataManipulation;
    }

    @Override
    public boolean supportsCatalogsInProcedureCalls() throws SQLException {
        return metaData.supportsCatalogsInProcedureCalls;
    }

    @Override
    public boolean supportsCatalogsInTableDefinitions() throws SQLException {
        return metaData.supportsCatalogsInTableDefinitions;
    }

    @Override
    public boolean supportsCatalogsInIndexDefinitions() throws SQLException {
        return metaData.supportsCatalogsInIndexDefinitions;
    }

    @Override
    public boolean supportsCatalogsInPrivilegeDefinitions() throws SQLException {
        return metaData.supportsCatalogsInPrivilegeDefinitions;
    }

    @Override
    public boolean supportsPositionedDelete() throws SQLException {
        return metaData.supportsPositionedDelete;
    }

    @Override
    public boolean supportsPositionedUpdate() throws SQLException {
        return metaData.supportsPositionedUpdate;
    }

    @Override
    public boolean supportsSelectForUpdate() throws SQLException {
        return metaData.supportsSelectForUpdate;
    }

    @Override
    public boolean supportsStoredProcedures() throws SQLException {
        return metaData.supportsStoredProcedures;
    }

    @Override
    public boolean supportsSubqueriesInComparisons() throws SQLException {
        return metaData.supportsSubqueriesInComparisons;
    }

    @Override
    public boolean supportsSubqueriesInExists() throws SQLException {
        return metaData.supportsSubqueriesInExists;
    }

    @Override
    public boolean supportsSubqueriesInIns() throws SQLException {
        return metaData.supportsSubqueriesInIns;
    }

    @Override
    public boolean supportsSubqueriesInQuantifieds() throws SQLException {
        return metaData.supportsSubqueriesInQuantifieds;
    }

    @Override
    public boolean supportsCorrelatedSubqueries() throws SQLException {
        return metaData.supportsCorrelatedSubqueries;
    }

    @Override
    public boolean supportsUnion() throws SQLException {
        return metaData.supportsUnion;
    }

    @Override
    public boolean supportsUnionAll() throws SQLException {
        return metaData.supportsUnionAll;
    }

    @Override
    public boolean supportsOpenCursorsAcrossCommit() throws SQLException {
        return metaData.supportsOpenCursorsAcrossCommit;
    }

    @Override
    public boolean supportsOpenCursorsAcrossRollback() throws SQLException {
        return metaData.supportsOpenCursorsAcrossRollback;
    }

    @Override
    public boolean supportsOpenStatementsAcrossCommit() throws SQLException {
        return metaData.supportsOpenStatementsAcrossCommit;
    }

    @Override
    public boolean supportsOpenStatementsAcrossRollback() throws SQLException {
        return metaData.supportsOpenStatementsAcrossRollback;
    }

    @Override
    public int getMaxBinaryLiteralLength() throws SQLException {
        return 0;
    }

    @Override
    public int getMaxCharLiteralLength() throws SQLException {
        return 0;
    }

    @Override
    public int getMaxColumnNameLength() throws SQLException {
        return metaData.maxColumnNameLength;
    }

    @Override
    public int getMaxColumnsInGroupBy() throws SQLException {
        return metaData.maxColumnsInGroupBy;
    }

    @Override
    public int getMaxColumnsInIndex() throws SQLException {
        return metaData.maxColumnsInIndex;
    }

    @Override
    public int getMaxColumnsInOrderBy() throws SQLException {
        return metaData.maxColumnsInOrderBy;
    }

    @Override
    public int getMaxColumnsInSelect() throws SQLException {
        return metaData.maxColumnsInSelect;
    }

    @Override
    public int getMaxColumnsInTable() throws SQLException {
        return metaData.maxColumnsInTable;
    }

    @Override
    public int getMaxConnections() throws SQLException {
        return 0;
    }

    @Override
    public int getMaxCursorNameLength() throws SQLException {
        return 0;
    }

    @Override
    public int getMaxIndexLength() throws SQLException {
        return 0;
    }

    @Override
    public int getMaxSchemaNameLength() throws SQLException {
        return metaData.maxSchemaNameLength;
    }

    @Override
    public int getMaxProcedureNameLength() throws SQLException {
        return 0;
    }

    @Override
    public int getMaxCatalogNameLength() throws SQLException {
        return metaData.maxCatalogNameLength;
    }

    @Override
    public int getMaxRowSize() throws SQLException {
        return metaData.maxRowSize;
    }

    @Override
    public boolean doesMaxRowSizeIncludeBlobs() throws SQLException {
        return false;
    }

    @Override
    public int getMaxStatementLength() throws SQLException {
        return metaData.maxStatementLength;
    }

    @Override
    public int getMaxStatements() throws SQLException {
        return metaData.maxStatements;
    }

    @Override
    public int getMaxTableNameLength() throws SQLException {
        return metaData.maxTableNameLength;
    }

    @Override
    public int getMaxTablesInSelect() throws SQLException {
        return metaData.maxTablesInSelect;
    }

    @Override
    public int getMaxUserNameLength() throws SQLException {
        return metaData.maxUserNameLength;
    }

    @Override
    public int getDefaultTransactionIsolation() throws SQLException {
        return metaData.defaultTransactionIsolation;
    }

    @Override
    public boolean supportsTransactions() throws SQLException {
        return metaData.supportsTransactions;
    }

    @Override
    public boolean supportsTransactionIsolationLevel(int level) throws SQLException {
        return (metaData.supportsTransactionIsolationLevel & level) != 0;
    }

    @Override
    public boolean supportsDataDefinitionAndDataManipulationTransactions() throws SQLException {
        return true; // По умолчанию
    }

    @Override
    public boolean supportsDataManipulationTransactionsOnly() throws SQLException {
        return false;
    }

    @Override
    public boolean dataDefinitionCausesTransactionCommit() throws SQLException {
        return false;
    }

    @Override
    public boolean dataDefinitionIgnoredInTransactions() throws SQLException {
        return false;
    }

    @Override
    public ResultSet getProcedures(String catalog, String schemaPattern, String procedureNamePattern) throws SQLException {
        return null;
    }

    @Override
    public ResultSet getProcedureColumns(String catalog, String schemaPattern, String procedureNamePattern, String columnNamePattern) throws SQLException {
        return null;
    }

    @Override
    public ResultSet getTables(String catalog, String schemaPattern, String tableNamePattern, String[] types) throws SQLException {
        // https://msdn.microsoft.com/en-us/library/ms710171.aspx
        String typePattern = types == null ? null : Arrays.stream(types)
                .filter(Objects::nonNull)
                .map(t -> "'" + t + "'")
                .collect(Collectors.joining(","));
        try {
            ResultSetPtr resultSetPtr = OdbcDatabaseMetaDataHandler.getTables(connection.get().getConnectionPtr(), catalog, schemaPattern, tableNamePattern, typePattern);
            return new NanodbcResultSet(resultSetPtr);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    @Override
    public ResultSet getSchemas() throws SQLException {
        try {
            ResultSetPtr resultSetPtr = OdbcDatabaseMetaDataHandler.getSchemas(connection.get().getConnectionPtr());
            return new NanodbcResultSet(resultSetPtr);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    @Override
    public ResultSet getCatalogs() throws SQLException {
        try {
            ResultSetPtr resultSetPtr = OdbcDatabaseMetaDataHandler.getCatalogs(connection.get().getConnectionPtr());
            return new NanodbcResultSet(resultSetPtr);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    @Override
    public ResultSet getTableTypes() throws SQLException {
        try {
            ResultSetPtr resultSetPtr = OdbcDatabaseMetaDataHandler.getTableTypes(connection.get().getConnectionPtr());
            return new NanodbcResultSet(resultSetPtr);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    @Override
    public ResultSet getColumns(String catalog, String schemaPattern, String tableNamePattern, String columnNamePattern) throws SQLException {
        try {
            ResultSetPtr resultSetPtr = OdbcDatabaseMetaDataHandler.getColumns(connection.get().getConnectionPtr(), catalog, schemaPattern, tableNamePattern, columnNamePattern);
            return new NanodbcResultSet(resultSetPtr);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    @Override
    public ResultSet getColumnPrivileges(String catalog, String schema, String table, String columnNamePattern) throws SQLException {
        return null;
    }

    @Override
    public ResultSet getTablePrivileges(String catalog, String schemaPattern, String tableNamePattern) throws SQLException {
        return null;
    }

    @Override
    public ResultSet getBestRowIdentifier(String catalog, String schema, String table, int scope, boolean nullable) throws SQLException {
        return null;
    }

    @Override
    public ResultSet getVersionColumns(String catalog, String schema, String table) throws SQLException {
        return null;
    }

    @Override
    public ResultSet getPrimaryKeys(String catalog, String schema, String table) throws SQLException {
        try {
            ResultSetPtr resultSetPtr = OdbcDatabaseMetaDataHandler.getPrimaryKeys(connection.get().getConnectionPtr(), catalog, schema, table);
            return new NanodbcResultSet(resultSetPtr);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    @Override
    public ResultSet getImportedKeys(String catalog, String schema, String table) throws SQLException {
        return null;
    }

    @Override
    public ResultSet getExportedKeys(String catalog, String schema, String table) throws SQLException {
        return null;
    }

    @Override
    public ResultSet getCrossReference(String parentCatalog, String parentSchema, String parentTable, String foreignCatalog, String foreignSchema, String foreignTable) throws SQLException {
        return null;
    }

    @Override
    public ResultSet getTypeInfo() throws SQLException {
        return null;
    }

    @Override
    public ResultSet getIndexInfo(String catalog, String schema, String table, boolean unique, boolean approximate) throws SQLException {
        return null;
    }

    @Override
    public boolean supportsResultSetType(int type) throws SQLException {
        return switch (type) {
            case ResultSet.TYPE_FORWARD_ONLY, ResultSet.TYPE_SCROLL_INSENSITIVE -> true;
            default -> false;
        };
    }

    @Override
    public boolean supportsResultSetConcurrency(int type, int concurrency) throws SQLException {
        return concurrency == ResultSet.CONCUR_READ_ONLY;
    }

    @Override
    public boolean ownUpdatesAreVisible(int type) throws SQLException {
        return false;
    }

    @Override
    public boolean ownDeletesAreVisible(int type) throws SQLException {
        return false;
    }

    @Override
    public boolean ownInsertsAreVisible(int type) throws SQLException {
        return false;
    }

    @Override
    public boolean othersUpdatesAreVisible(int type) throws SQLException {
        return false;
    }

    @Override
    public boolean othersDeletesAreVisible(int type) throws SQLException {
        return false;
    }

    @Override
    public boolean othersInsertsAreVisible(int type) throws SQLException {
        return false;
    }

    @Override
    public boolean updatesAreDetected(int type) throws SQLException {
        return false;
    }

    @Override
    public boolean deletesAreDetected(int type) throws SQLException {
        return false;
    }

    @Override
    public boolean insertsAreDetected(int type) throws SQLException {
        return false;
    }

    @Override
    public boolean supportsBatchUpdates() throws SQLException {
        return metaData.supportsBatchUpdates;
    }

    @Override
    public ResultSet getUDTs(String catalog, String schemaPattern, String typeNamePattern, int[] types) throws SQLException {
        return null;
    }

    @Override
    public Connection getConnection() {
        return connection.get();
    }

    @Override
    public boolean supportsSavepoints() throws SQLException {
        return metaData.supportsSavepoints;
    }

    @Override
    public boolean supportsNamedParameters() throws SQLException {
        return metaData.supportsNamedParameters;
    }

    @Override
    public boolean supportsMultipleOpenResults() throws SQLException {
        return false;
    }

    @Override
    public boolean supportsGetGeneratedKeys() throws SQLException {
        return false;
    }

    @Override
    public ResultSet getSuperTypes(String catalog, String schemaPattern, String typeNamePattern) throws SQLException {
        return null;
    }

    @Override
    public ResultSet getSuperTables(String catalog, String schemaPattern, String tableNamePattern) throws SQLException {
        return null;
    }

    @Override
    public ResultSet getAttributes(String catalog, String schemaPattern, String typeNamePattern, String attributeNamePattern) throws SQLException {
        return null;
    }

    @Override
    public boolean supportsResultSetHoldability(int holdability) throws SQLException {
        return holdability == ResultSet.HOLD_CURSORS_OVER_COMMIT;
    }

    @Override
    public int getResultSetHoldability() throws SQLException {
        return metaData.resultSetHoldability;
    }

    @Override
    public int getDatabaseMajorVersion() throws SQLException {
        return metaData.databaseMajorVersion;
    }

    @Override
    public int getDatabaseMinorVersion() throws SQLException {
        return metaData.databaseMinorVersion;
    }

    @Override
    public int getJDBCMajorVersion() throws SQLException {
        return NanodbcDriver.MAJOR_VERSION;
    }

    @Override
    public int getJDBCMinorVersion() throws SQLException {
        return NanodbcDriver.MINOR_VERSION;
    }

    @Override
    public int getSQLStateType() throws SQLException {
        return metaData.sqlStateType;
    }

    @Override
    public boolean locatorsUpdateCopy() throws SQLException {
        return metaData.locatorsUpdateCopy;
    }

    @Override
    public boolean supportsStatementPooling() throws SQLException {
        return metaData.supportsStatementPooling;
    }

    @Override
    public RowIdLifetime getRowIdLifetime() throws SQLException {
        return null;
    }

    @Override
    public ResultSet getSchemas(String catalog, String schemaPattern) throws SQLException {
        try {
            ResultSetPtr resultSetPtr = OdbcDatabaseMetaDataHandler.getSchemas(connection.get().getConnectionPtr(), catalog, schemaPattern);
            return new NanodbcResultSet(resultSetPtr);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    @Override
    public boolean supportsStoredFunctionsUsingCallSyntax() throws SQLException {
        return metaData.supportsStoredFunctionsUsingCallSyntax;
    }

    @Override
    public boolean autoCommitFailureClosesAllResultSets() throws SQLException {
        return metaData.autoCommitFailureClosesAllResultSets;
    }

    @Override
    public ResultSet getClientInfoProperties() throws SQLException {
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public ResultSet getFunctions(String catalog, String schemaPattern, String functionNamePattern) throws SQLException {
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public ResultSet getFunctionColumns(String catalog, String schemaPattern, String functionNamePattern, String columnNamePattern) throws SQLException {
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public ResultSet getPseudoColumns(String catalog, String schemaPattern, String tableNamePattern, String columnNamePattern) throws SQLException {
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public boolean generatedKeyAlwaysReturned() throws SQLException {
        return metaData.generatedKeyAlwaysReturned;
    }

    @Override
    public <T> T unwrap(Class<T> iface) throws SQLException {
        if (isWrapperFor(iface)) {
            return iface.cast(this);
        }
        throw new NanodbcSQLException("Cannot unwrap to " + iface.getName());
    }

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
