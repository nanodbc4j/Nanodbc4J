package io.github.nanodbc4j.jdbc;

import io.github.nanodbc4j.exceptions.NanodbcSQLException;
import io.github.nanodbc4j.exceptions.NanodbcSQLFeatureNotSupportedException;
import io.github.nanodbc4j.exceptions.NativeException;
import io.github.nanodbc4j.internal.dto.DatabaseMetaDataDto;
import io.github.nanodbc4j.internal.handler.DatabaseMetaDataHandler;
import io.github.nanodbc4j.internal.pointer.ResultSetPtr;
import lombok.extern.java.Log;

import java.lang.ref.WeakReference;
import java.sql.Connection;
import java.sql.DatabaseMetaData;
import java.sql.ResultSet;
import java.sql.RowIdLifetime;
import java.sql.SQLException;
import java.util.Arrays;
import java.util.Objects;
import java.util.function.Predicate;
import java.util.logging.Level;
import java.util.stream.Collectors;

@Log
public class NanodbcDatabaseMetaData implements DatabaseMetaData, JdbcWrapper {
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
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.allProceduresAreCallable");
        return metaData.allProceduresAreCallable;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean allTablesAreSelectable() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.allTablesAreSelectable");
        return metaData.allTablesAreSelectable;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String getURL() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getURL");
        return connection.get().getUrl();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String getUserName() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getUserName");
        return metaData.userName != null ? metaData.userName : null;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean isReadOnly() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.isReadOnly");
        return metaData.isReadOnly;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean nullsAreSortedHigh() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.nullsAreSortedHigh");
        return metaData.nullsAreSortedHigh;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean nullsAreSortedLow() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.nullsAreSortedLow");
        return metaData.nullsAreSortedLow;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean nullsAreSortedAtStart() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.nullsAreSortedAtStart");
        return metaData.nullsAreSortedAtStart;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean nullsAreSortedAtEnd() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.nullsAreSortedAtEnd");
        return metaData.nullsAreSortedAtEnd;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String getDatabaseProductName() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getDatabaseProductName");
        return metaData.databaseProductName;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String getDatabaseProductVersion() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getDatabaseProductVersion");
        return metaData.databaseProductVersion;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String getDriverName() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getDriverName");
        return "JDBC-ODBC Bridge (" + metaData.driverName + ")";
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String getDriverVersion() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getDriverVersion");
        return metaData.driverVersion;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getDriverMajorVersion() {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getDriverMajorVersion");
        return metaData.driverMajorVersion;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getDriverMinorVersion() {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getDriverMinorVersion");
        return metaData.driverMinorVersion;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean usesLocalFiles() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.usesLocalFiles");
        return metaData.usesLocalFiles;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean usesLocalFilePerTable() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.usesLocalFilePerTable");
        return metaData.usesLocalFilePerTable;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsMixedCaseIdentifiers() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsMixedCaseIdentifiers");
        return metaData.supportsMixedCaseIdentifiers;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean storesUpperCaseIdentifiers() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.storesUpperCaseIdentifiers");
        return metaData.storesUpperCaseIdentifiers;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean storesLowerCaseIdentifiers() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.storesLowerCaseIdentifiers");
        return metaData.storesLowerCaseIdentifiers;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean storesMixedCaseIdentifiers() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.storesMixedCaseIdentifiers");
        return metaData.storesMixedCaseIdentifiers;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsMixedCaseQuotedIdentifiers() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsMixedCaseQuotedIdentifiers");
        return metaData.supportsMixedCaseQuotedIdentifiers;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean storesUpperCaseQuotedIdentifiers() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.storesUpperCaseQuotedIdentifiers");
        return metaData.storesUpperCaseQuotedIdentifiers;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean storesLowerCaseQuotedIdentifiers() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.storesLowerCaseQuotedIdentifiers");
        return metaData.storesLowerCaseQuotedIdentifiers;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean storesMixedCaseQuotedIdentifiers() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.storesMixedCaseQuotedIdentifiers");
        return metaData.storesMixedCaseQuotedIdentifiers;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String getIdentifierQuoteString() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getIdentifierQuoteString");
        return metaData.identifierQuoteString != null ? metaData.identifierQuoteString : "";
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String getSQLKeywords() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getSQLKeywords");
        return metaData.sqlKeywords != null ? metaData.sqlKeywords : "";
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String getNumericFunctions() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getNumericFunctions");
        return metaData.numericFunctions != null ? metaData.numericFunctions : "";
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String getStringFunctions() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getStringFunctions");
        return metaData.stringFunctions != null ? metaData.stringFunctions : "";
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String getSystemFunctions() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getSystemFunctions");
        return metaData.systemFunctions != null ? metaData.systemFunctions : "";
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String getTimeDateFunctions() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getTimeDateFunctions");
        return metaData.timeDateFunctions != null ? metaData.timeDateFunctions : "";
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String getSearchStringEscape() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getSearchStringEscape");
        return metaData.searchStringEscape != null ? metaData.searchStringEscape : "";
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String getExtraNameCharacters() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getExtraNameCharacters");
        return metaData.extraNameCharacters != null ? metaData.extraNameCharacters : "";
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsAlterTableWithAddColumn() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsAlterTableWithAddColumn");
        return metaData.supportsAlterTableWithAddColumn;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsAlterTableWithDropColumn() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsAlterTableWithDropColumn");
        return metaData.supportsAlterTableWithDropColumn;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsColumnAliasing() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsColumnAliasing");
        return metaData.supportsColumnAliasing;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean nullPlusNonNullIsNull() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.nullPlusNonNullIsNull");
        return metaData.nullPlusNonNullIsNull;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsConvert() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsConvert");
        return metaData.supportsConvert;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsConvert(int fromType, int toType) throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsConvert");
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
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsTableCorrelationNames");
        return metaData.supportsTableCorrelationNames;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsDifferentTableCorrelationNames() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsDifferentTableCorrelationNames");
        return metaData.supportsDifferentTableCorrelationNames;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsExpressionsInOrderBy() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsExpressionsInOrderBy");
        return metaData.supportsExpressionsInOrderBy;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsOrderByUnrelated() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsOrderByUnrelated");
        return metaData.supportsOrderByUnrelated;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsGroupBy() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsGroupBy");
        return metaData.supportsGroupBy;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsGroupByUnrelated() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsGroupByUnrelated");
        return metaData.supportsGroupByUnrelated;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsGroupByBeyondSelect() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsGroupByBeyondSelect");
        return metaData.supportsGroupByBeyondSelect;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsLikeEscapeClause() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsLikeEscapeClause");
        return metaData.supportsLikeEscapeClause;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsMultipleResultSets() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsMultipleResultSets");
        return metaData.supportsMultipleResultSets;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsMultipleTransactions() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsMultipleTransactions");
        return metaData.supportsMultipleTransactions;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsNonNullableColumns() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsNonNullableColumns");
        return metaData.supportsNonNullableColumns;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsMinimumSQLGrammar() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsMinimumSQLGrammar");
        return metaData.supportsMinimumSQLGrammar;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsCoreSQLGrammar() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsCoreSQLGrammar");
        return metaData.supportsCoreSQLGrammar;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsExtendedSQLGrammar() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsExtendedSQLGrammar");
        return metaData.supportsExtendedSQLGrammar;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsANSI92EntryLevelSQL() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsANSI92EntryLevelSQL");
        return metaData.supportsANSI92EntryLevelSQL;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsANSI92IntermediateSQL() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsANSI92IntermediateSQL");
        return metaData.supportsANSI92IntermediateSQL;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsANSI92FullSQL() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsANSI92FullSQL");
        return metaData.supportsANSI92FullSQL;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsIntegrityEnhancementFacility() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsIntegrityEnhancementFacility");
        return metaData.supportsIntegrityEnhancementFacility;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsOuterJoins() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsOuterJoins");
        return metaData.supportsOuterJoins;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsFullOuterJoins() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsFullOuterJoins");
        return metaData.supportsFullOuterJoins;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsLimitedOuterJoins() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsLimitedOuterJoins");
        return metaData.supportsLimitedOuterJoins;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String getSchemaTerm() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getSchemaTerm");
        return metaData.schemaTerm != null ? metaData.schemaTerm : "SCHEMA";
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String getProcedureTerm() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getProcedureTerm");
        return metaData.procedureTerm != null ? metaData.procedureTerm : "PROCEDURE";
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String getCatalogTerm() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getCatalogTerm");
        return metaData.catalogTerm != null ? metaData.catalogTerm : "CATALOG";
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean isCatalogAtStart() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.isCatalogAtStart");
        return metaData.isCatalogAtStart;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String getCatalogSeparator() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getCatalogSeparator");
        return metaData.catalogSeparator != null ? metaData.catalogSeparator : ".";
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsSchemasInDataManipulation() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsSchemasInDataManipulation");
        return metaData.supportsSchemasInDataManipulation;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsSchemasInProcedureCalls() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsSchemasInProcedureCalls");
        return metaData.supportsSchemasInProcedureCalls;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsSchemasInTableDefinitions() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsSchemasInTableDefinitions");
        return metaData.supportsSchemasInTableDefinitions;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsSchemasInIndexDefinitions() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsSchemasInIndexDefinitions");
        return metaData.supportsSchemasInIndexDefinitions;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsSchemasInPrivilegeDefinitions() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsSchemasInPrivilegeDefinitions");
        return metaData.supportsSchemasInPrivilegeDefinitions;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsCatalogsInDataManipulation() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsCatalogsInDataManipulation");
        return metaData.supportsCatalogsInDataManipulation;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsCatalogsInProcedureCalls() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsCatalogsInProcedureCalls");
        return metaData.supportsCatalogsInProcedureCalls;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsCatalogsInTableDefinitions() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsCatalogsInTableDefinitions");
        return metaData.supportsCatalogsInTableDefinitions;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsCatalogsInIndexDefinitions() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsCatalogsInIndexDefinitions");
        return metaData.supportsCatalogsInIndexDefinitions;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsCatalogsInPrivilegeDefinitions() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsCatalogsInPrivilegeDefinitions");
        return metaData.supportsCatalogsInPrivilegeDefinitions;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsPositionedDelete() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsPositionedDelete");
        return metaData.supportsPositionedDelete;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsPositionedUpdate() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsPositionedUpdate");
        return metaData.supportsPositionedUpdate;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsSelectForUpdate() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsSelectForUpdate");
        return metaData.supportsSelectForUpdate;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsStoredProcedures() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsStoredProcedures");
        return metaData.supportsStoredProcedures;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsSubqueriesInComparisons() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsSubqueriesInComparisons");
        return metaData.supportsSubqueriesInComparisons;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsSubqueriesInExists() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsSubqueriesInExists");
        return metaData.supportsSubqueriesInExists;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsSubqueriesInIns() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsSubqueriesInIns");
        return metaData.supportsSubqueriesInIns;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsSubqueriesInQuantifieds() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsSubqueriesInQuantifieds");
        return metaData.supportsSubqueriesInQuantifieds;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsCorrelatedSubqueries() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsCorrelatedSubqueries");
        return metaData.supportsCorrelatedSubqueries;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsUnion() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsUnion");
        return metaData.supportsUnion;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsUnionAll() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsUnionAll");
        return metaData.supportsUnionAll;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsOpenCursorsAcrossCommit() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsOpenCursorsAcrossCommit");
        return metaData.supportsOpenCursorsAcrossCommit;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsOpenCursorsAcrossRollback() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsOpenCursorsAcrossRollback");
        return metaData.supportsOpenCursorsAcrossRollback;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsOpenStatementsAcrossCommit() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsOpenStatementsAcrossCommit");
        return metaData.supportsOpenStatementsAcrossCommit;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsOpenStatementsAcrossRollback() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsOpenStatementsAcrossRollback");
        return metaData.supportsOpenStatementsAcrossRollback;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getMaxBinaryLiteralLength() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getMaxBinaryLiteralLength");
        return metaData.getMaxBinaryLiteralLength;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getMaxCharLiteralLength() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getMaxCharLiteralLength");
        return metaData.getMaxCharLiteralLength;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getMaxColumnNameLength() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getMaxColumnNameLength");
        return metaData.maxColumnNameLength;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getMaxColumnsInGroupBy() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getMaxColumnsInGroupBy");
        return metaData.maxColumnsInGroupBy;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getMaxColumnsInIndex() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getMaxColumnsInIndex");
        return metaData.maxColumnsInIndex;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getMaxColumnsInOrderBy() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getMaxColumnsInOrderBy");
        return metaData.maxColumnsInOrderBy;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getMaxColumnsInSelect() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getMaxColumnsInSelect");
        return metaData.maxColumnsInSelect;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getMaxColumnsInTable() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getMaxColumnsInTable");
        return metaData.maxColumnsInTable;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getMaxConnections() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getMaxConnections");
        return metaData.getMaxConnections;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getMaxCursorNameLength() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getMaxCursorNameLength");
        return metaData.getMaxCursorNameLength;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getMaxIndexLength() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getMaxIndexLength");
        return metaData.getMaxIndexLength;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getMaxSchemaNameLength() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getMaxSchemaNameLength");
        return metaData.maxSchemaNameLength;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getMaxProcedureNameLength() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getMaxProcedureNameLength");
        return metaData.getMaxProcedureNameLength;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getMaxCatalogNameLength() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getMaxCatalogNameLength");
        return metaData.maxCatalogNameLength;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getMaxRowSize() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getMaxRowSize");
        return metaData.maxRowSize;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean doesMaxRowSizeIncludeBlobs() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.doesMaxRowSizeIncludeBlobs");
        return metaData.doesMaxRowSizeIncludeBlobs;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getMaxStatementLength() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getMaxStatementLength");
        return metaData.maxStatementLength;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getMaxStatements() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getMaxStatements");
        return metaData.maxStatements;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getMaxTableNameLength() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getMaxTableNameLength");
        return metaData.maxTableNameLength;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getMaxTablesInSelect() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getDatabaseProductName");
        return metaData.maxTablesInSelect;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getMaxUserNameLength() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getMaxUserNameLength");
        return metaData.maxUserNameLength;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getDefaultTransactionIsolation() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getDefaultTransactionIsolation");
        return metaData.defaultTransactionIsolation;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsTransactions() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsTransactions");
        return metaData.supportsTransactions;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsTransactionIsolationLevel(int level) throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsTransactionIsolationLevel");
        return (metaData.supportsTransactionIsolationLevel & level) != 0;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsDataDefinitionAndDataManipulationTransactions() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsDataDefinitionAndDataManipulationTransactions");
        return metaData.supportsDataDefinitionAndDataManipulationTransactions;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsDataManipulationTransactionsOnly() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsDataManipulationTransactionsOnly");
        return metaData.supportsDataManipulationTransactionsOnly;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean dataDefinitionCausesTransactionCommit() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.dataDefinitionCausesTransactionCommit");
        return metaData.dataDefinitionCausesTransactionCommit;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean dataDefinitionIgnoredInTransactions() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.dataDefinitionIgnoredInTransactions");
        return metaData.dataDefinitionIgnoredInTransactions;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public ResultSet getProcedures(String catalog, String schemaPattern, String procedureNamePattern) throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getProcedures");
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
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getProcedureColumns");
        try {
            ResultSetPtr resultSetPtr = DatabaseMetaDataHandler.getProcedureColumns(connection.get().getConnectionPtr(), catalog, schemaPattern, procedureNamePattern, columnNamePattern);
            NanodbcResultSet rs = new NanodbcResultSet(resultSetPtr);
            //rename some columns in order to be JDBC compilant.
            rs.setAliasColumnName("PRECISION", 8);
            rs.setAliasColumnName("LENGTH", 9);
            rs.setAliasColumnName("SCALE", 10);
            rs.setAliasColumnName("RADIX", 11);
            return rs;
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public ResultSet getTables(String catalog, String schemaPattern, String tableNamePattern, String[] types) throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getTables");
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
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getSchemas");
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
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getCatalogs");
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
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getTableTypes");
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
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getColumns");
        try {
            ResultSetPtr resultSetPtr =
                    DatabaseMetaDataHandler.getColumns(connection.get().getConnectionPtr(), catalog, schemaPattern, tableNamePattern, columnNamePattern);
            NanodbcResultSet rs = new NanodbcResultSet(resultSetPtr);
            //rename some columns in order to be JDBC compilant.
            rs.setAliasColumnName ("SQL_DATETIME_SUB", 15);
            return rs;
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public ResultSet getColumnPrivileges(String catalog, String schema, String table, String columnNamePattern) throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getColumnPrivileges");
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
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getTablePrivileges");
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
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getBestRowIdentifier");
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
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getVersionColumns");
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
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getPrimaryKeys");
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
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getImportedKeys");
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
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getExportedKeys");
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
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getCrossReference");
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
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getTypeInfo");
        try {
            ResultSetPtr resultSetPtr = DatabaseMetaDataHandler.getTypeInfo(connection.get().getConnectionPtr());
            NanodbcResultSet rs = new NanodbcResultSet(resultSetPtr);
            //rename some columns in order to be JDBC compilant.
            rs.setAliasColumnName("PRECISION", 3);
            rs.setAliasColumnName("AUTO_INCREMENT", 12);
            return rs;
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public ResultSet getIndexInfo(String catalog, String schema, String table, boolean unique, boolean approximate) throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getIndexInfo");
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
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsResultSetType");
        return type == ResultSet.TYPE_FORWARD_ONLY;
//        return switch (type) {    TODO
//            case ResultSet.TYPE_FORWARD_ONLY, ResultSet.TYPE_SCROLL_INSENSITIVE -> true;
//            default -> false;
//        };
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsResultSetConcurrency(int type, int concurrency) throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getDatabaseProductName");
        // Support only FORWARD_ONLY + READ_ONLY
        return supportsResultSetType(type) && (concurrency == ResultSet.CONCUR_READ_ONLY);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean ownUpdatesAreVisible(int type) throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.ownUpdatesAreVisible");
        return false;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean ownDeletesAreVisible(int type) throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.ownDeletesAreVisible");
        return false;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean ownInsertsAreVisible(int type) throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.ownInsertsAreVisible");
        return false;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean othersUpdatesAreVisible(int type) throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.othersUpdatesAreVisible");
        return false;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean othersDeletesAreVisible(int type) throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.othersDeletesAreVisible");
        return false;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean othersInsertsAreVisible(int type) throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.othersInsertsAreVisible");
        return false;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean updatesAreDetected(int type) throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.updatesAreDetected");
        return false;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean deletesAreDetected(int type) throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.deletesAreDetected");
        return false;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean insertsAreDetected(int type) throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.insertsAreDetected");
        return false;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsBatchUpdates() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsBatchUpdates");
        return metaData.supportsBatchUpdates;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public ResultSet getUDTs(String catalog, String schemaPattern, String typeNamePattern, int[] types) throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getUDTs");
        return null;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Connection getConnection() {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getConnection");
        return connection.get();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsSavepoints() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsSavepoints");
        return metaData.supportsSavepoints;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsNamedParameters() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsNamedParameters");
        return metaData.supportsNamedParameters;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsMultipleOpenResults() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsMultipleOpenResults");
        return metaData.supportsMultipleOpenResults;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsGetGeneratedKeys() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsGetGeneratedKeys");
        return metaData.supportsGetGeneratedKeys;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public ResultSet getSuperTypes(String catalog, String schemaPattern, String typeNamePattern) throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getSuperTypes");
        // sun.jdbc.odbc does not support this
        throw new NanodbcSQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public ResultSet getSuperTables(String catalog, String schemaPattern, String tableNamePattern) throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getSuperTables");
        // sun.jdbc.odbc does not support this
        throw new NanodbcSQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public ResultSet getAttributes(String catalog, String schemaPattern, String typeNamePattern, String attributeNamePattern) throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getAttributes");
        // sun.jdbc.odbc does not support this
        throw new NanodbcSQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsResultSetHoldability(int holdability) throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsResultSetHoldability");
        return holdability == ResultSet.HOLD_CURSORS_OVER_COMMIT;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getResultSetHoldability() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getResultSetHoldability");
        return metaData.resultSetHoldability;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getDatabaseMajorVersion() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getDatabaseMajorVersion");
        return metaData.databaseMajorVersion;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getDatabaseMinorVersion() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getDatabaseMinorVersion");
        return metaData.databaseMinorVersion;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getJDBCMajorVersion() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getJDBCMajorVersion");
        return NanodbcDriver.MAJOR_VERSION;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getJDBCMinorVersion() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getJDBCMinorVersion");
        return NanodbcDriver.MINOR_VERSION;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getSQLStateType() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getSQLStateType");
        return metaData.sqlStateType;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean locatorsUpdateCopy() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.locatorsUpdateCopy");
        return metaData.locatorsUpdateCopy;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsStatementPooling() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsStatementPooling");
        return metaData.supportsStatementPooling;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public RowIdLifetime getRowIdLifetime() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getRowIdLifetime");
        // sun.jdbc.odbc does not support this
        throw new NanodbcSQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public ResultSet getSchemas(String catalog, String schemaPattern) throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getSchemas");
        // sun.jdbc.odbc does not support this
        throw new NanodbcSQLFeatureNotSupportedException("getSchemas with parameters is not supported");
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean supportsStoredFunctionsUsingCallSyntax() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.supportsStoredFunctionsUsingCallSyntax");
        return metaData.supportsStoredFunctionsUsingCallSyntax;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean autoCommitFailureClosesAllResultSets() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.autoCommitFailureClosesAllResultSets");
        return metaData.autoCommitFailureClosesAllResultSets;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public ResultSet getClientInfoProperties() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getClientInfoProperties");
        // sun.jdbc.odbc does not support this
        throw new NanodbcSQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public ResultSet getFunctions(String catalog, String schemaPattern, String functionNamePattern) throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getFunctions");
        // sun.jdbc.odbc does not support this
        throw new NanodbcSQLFeatureNotSupportedException("Operation not yet supported");
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public ResultSet getFunctionColumns(String catalog, String schemaPattern, String functionNamePattern, String columnNamePattern) throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getFunctionColumns");
        // sun.jdbc.odbc does not support this
        throw new NanodbcSQLFeatureNotSupportedException("Operation not yet supported");
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public ResultSet getPseudoColumns(String catalog, String schemaPattern, String tableNamePattern, String columnNamePattern) throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.getPseudoColumns");
        // sun.jdbc.odbc does not support this
        throw new NanodbcSQLFeatureNotSupportedException("Operation not yet supported");
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean generatedKeyAlwaysReturned() throws SQLException {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.generatedKeyAlwaysReturned");
        return metaData.generatedKeyAlwaysReturned;
    }

    @Override
    public String toString() {
        log.log(Level.FINEST, "NanodbcDatabaseMetaData.toString");
        return "NanodbcDatabaseMetaData{" +
                "metaData=" + metaData +
                '}';
    }
}
