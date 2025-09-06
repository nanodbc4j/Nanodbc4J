package io.github.michael1297.internal.dto;

import lombok.EqualsAndHashCode;
import lombok.ToString;

@ToString
@EqualsAndHashCode
public class OdbcDatabaseMetaData {
    // === Строковые значения ===
    public String databaseProductName;
    public String databaseProductVersion;
    public String driverName;
    public String driverVersion;
    public String identifierQuoteString;
    public String schemaTerm;
    public String procedureTerm;
    public String catalogTerm;
    public String catalogSeparator;
    public String sqlKeywords;
    public String numericFunctions;
    public String stringFunctions;
    public String systemFunctions;
    public String timeDateFunctions;
    public String searchStringEscape;
    public String extraNameCharacters;

    // === Булевы значения ===
    public boolean isReadOnly;
    public boolean supportsTransactions;
    public boolean supportsSavepoints;
    public boolean supportsNamedParameters;
    public boolean supportsBatchUpdates;
    public boolean supportsUnion;
    public boolean supportsUnionAll;
    public boolean supportsLikeEscapeClause;
    public boolean supportsGroupBy;
    public boolean supportsGroupByUnrelated;
    public boolean supportsGroupByBeyondSelect;
    public boolean supportsOrderByUnrelated;
    public boolean supportsAlterTableWithAddColumn;
    public boolean supportsColumnAliasing;
    public boolean nullPlusNonNullIsNull;
    public boolean supportsExpressionsInOrderBy;
    public boolean supportsSelectForUpdate;
    public boolean supportsStoredProcedures;
    public boolean supportsSubqueriesInComparisons;
    public boolean supportsSubqueriesInExists;
    public boolean supportsSubqueriesInIns;
    public boolean supportsSubqueriesInQuantifieds;
    public boolean supportsCorrelatedSubqueries;
    public boolean supportsIntegrityEnhancementFacility;
    public boolean supportsOuterJoins;
    public boolean supportsFullOuterJoins;
    public boolean supportsLimitedOuterJoins;
    public boolean supportsSchemasInDataManipulation;
    public boolean supportsSchemasInProcedureCalls;
    public boolean supportsSchemasInTableDefinitions;
    public boolean supportsSchemasInIndexDefinitions;
    public boolean supportsSchemasInPrivilegeDefinitions;
    public boolean supportsCatalogsInDataManipulation;
    public boolean supportsCatalogsInProcedureCalls;
    public boolean supportsCatalogsInTableDefinitions;
    public boolean supportsCatalogsInIndexDefinitions;
    public boolean supportsCatalogsInPrivilegeDefinitions;
    public boolean supportsPositionedDelete;
    public boolean supportsPositionedUpdate;
    public boolean supportsOpenCursorsAcrossCommit;
    public boolean supportsOpenCursorsAcrossRollback;
    public boolean supportsOpenStatementsAcrossCommit;
    public boolean supportsOpenStatementsAcrossRollback;
    public boolean locatorsUpdateCopy;
    public boolean supportsStatementPooling;
    public boolean autoCommitFailureClosesAllResultSets;
    public boolean supportsStoredFunctionsUsingCallSyntax;
    public boolean generatedKeyAlwaysReturned;

    // === Целочисленные значения ===
    public int supportsTransactionIsolationLevel;
    public int nullCollation;
    public int sqlStateType;
    public int defaultTransactionIsolation;
    public int resultSetHoldability;
    public int rowIdLifetime;
    public int maxTableNameLength;
    public int maxSchemaNameLength;
    public int maxCatalogNameLength;
    public int maxColumnNameLength;
    public int maxColumnsInGroupBy;
    public int maxColumnsInOrderBy;
    public int maxColumnsInSelect;
    public int maxColumnsInTable;
    public int maxColumnsInIndex;
    public int maxStatementLength;
    public int maxStatements;
    public int maxTablesInSelect;
    public int maxUserNameLength;
    public int maxRowSize;
    public int databaseMajorVersion;
    public int databaseMinorVersion;
    public int driverMajorVersion;
    public int driverMinorVersion;
}
