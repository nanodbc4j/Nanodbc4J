package io.github.nanodbc4j.internal.cstruct;

import com.sun.jna.Pointer;
import com.sun.jna.Structure;
import lombok.NoArgsConstructor;

@NoArgsConstructor
@Structure.FieldOrder({
        // === Строковые значения ===
        "databaseProductName",
        "databaseProductVersion",
        "driverName",
        "driverVersion",
        "identifierQuoteString",
        "schemaTerm",
        "procedureTerm",
        "catalogTerm",
        "catalogSeparator",
        "sqlKeywords",
        "numericFunctions",
        "stringFunctions",
        "systemFunctions",
        "timeDateFunctions",
        "searchStringEscape",
        "extraNameCharacters",

        // === Булевы значения ===
        "isReadOnly",
        "supportsTransactions",
        "supportsSavepoints",
        "supportsNamedParameters",
        "supportsBatchUpdates",
        "supportsUnion",
        "supportsUnionAll",
        "supportsLikeEscapeClause",
        "supportsGroupBy",
        "supportsGroupByUnrelated",
        "supportsGroupByBeyondSelect",
        "supportsOrderByUnrelated",
        "supportsAlterTableWithAddColumn",
        "supportsColumnAliasing",
        "nullPlusNonNullIsNull",
        "supportsExpressionsInOrderBy",
        "supportsSelectForUpdate",
        "supportsStoredProcedures",
        "supportsSubqueriesInComparisons",
        "supportsSubqueriesInExists",
        "supportsSubqueriesInIns",
        "supportsSubqueriesInQuantifieds",
        "supportsCorrelatedSubqueries",
        "supportsIntegrityEnhancementFacility",
        "supportsOuterJoins",
        "supportsFullOuterJoins",
        "supportsLimitedOuterJoins",
        "supportsSchemasInDataManipulation",
        "supportsSchemasInProcedureCalls",
        "supportsSchemasInTableDefinitions",
        "supportsSchemasInIndexDefinitions",
        "supportsSchemasInPrivilegeDefinitions",
        "supportsCatalogsInDataManipulation",
        "supportsCatalogsInProcedureCalls",
        "supportsCatalogsInTableDefinitions",
        "supportsCatalogsInIndexDefinitions",
        "supportsCatalogsInPrivilegeDefinitions",
        "supportsPositionedDelete",
        "supportsPositionedUpdate",
        "supportsOpenCursorsAcrossCommit",
        "supportsOpenCursorsAcrossRollback",
        "supportsOpenStatementsAcrossCommit",
        "supportsOpenStatementsAcrossRollback",
        "locatorsUpdateCopy",
        "supportsStatementPooling",
        "allProceduresAreCallable",
        "allTablesAreSelectable",
        "supportsMultipleResultSets",
        "supportsMultipleTransactions",
        "autoCommitFailureClosesAllResultSets",
        "supportsStoredFunctionsUsingCallSyntax",
        "generatedKeyAlwaysReturned",

        // === Целочисленные значения ===
        "supportsTransactionIsolationLevel",
        "nullCollation",
        "sqlStateType",
        "defaultTransactionIsolation",
        "resultSetHoldability",
        "rowIdLifetime",
        "maxTableNameLength",
        "maxSchemaNameLength",
        "maxCatalogNameLength",
        "maxColumnNameLength",
        "maxColumnsInGroupBy",
        "maxColumnsInOrderBy",
        "maxColumnsInSelect",
        "maxColumnsInTable",
        "maxColumnsInIndex",
        "maxStatementLength",
        "maxStatements",
        "maxTablesInSelect",
        "maxUserNameLength",
        "maxRowSize",
        "databaseMajorVersion",
        "databaseMinorVersion",
        "driverMajorVersion",
        "driverMinorVersion"
})
public class DatabaseMetaDataStruct extends Structure {
    // === Строковые значения ===
    public Pointer databaseProductName;
    public Pointer databaseProductVersion;
    public Pointer driverName;
    public Pointer driverVersion;
    public Pointer identifierQuoteString;
    public Pointer schemaTerm;
    public Pointer procedureTerm;
    public Pointer catalogTerm;
    public Pointer catalogSeparator;
    public Pointer sqlKeywords;
    public Pointer numericFunctions;
    public Pointer stringFunctions;
    public Pointer systemFunctions;
    public Pointer timeDateFunctions;
    public Pointer searchStringEscape;
    public Pointer extraNameCharacters;

    // === Булевы значения ===
    public byte isReadOnly;
    public byte supportsTransactions;
    public byte supportsSavepoints;
    public byte supportsNamedParameters;
    public byte supportsBatchUpdates;
    public byte supportsUnion;
    public byte supportsUnionAll;
    public byte supportsLikeEscapeClause;
    public byte supportsGroupBy;
    public byte supportsGroupByUnrelated;
    public byte supportsGroupByBeyondSelect;
    public byte supportsOrderByUnrelated;
    public byte supportsAlterTableWithAddColumn;
    public byte supportsColumnAliasing;
    public byte nullPlusNonNullIsNull;
    public byte supportsExpressionsInOrderBy;
    public byte supportsSelectForUpdate;
    public byte supportsStoredProcedures;
    public byte supportsSubqueriesInComparisons;
    public byte supportsSubqueriesInExists;
    public byte supportsSubqueriesInIns;
    public byte supportsSubqueriesInQuantifieds;
    public byte supportsCorrelatedSubqueries;
    public byte supportsIntegrityEnhancementFacility;
    public byte supportsOuterJoins;
    public byte supportsFullOuterJoins;
    public byte supportsLimitedOuterJoins;
    public byte supportsSchemasInDataManipulation;
    public byte supportsSchemasInProcedureCalls;
    public byte supportsSchemasInTableDefinitions;
    public byte supportsSchemasInIndexDefinitions;
    public byte supportsSchemasInPrivilegeDefinitions;
    public byte supportsCatalogsInDataManipulation;
    public byte supportsCatalogsInProcedureCalls;
    public byte supportsCatalogsInTableDefinitions;
    public byte supportsCatalogsInIndexDefinitions;
    public byte supportsCatalogsInPrivilegeDefinitions;
    public byte supportsPositionedDelete;
    public byte supportsPositionedUpdate;
    public byte supportsOpenCursorsAcrossCommit;
    public byte supportsOpenCursorsAcrossRollback;
    public byte supportsOpenStatementsAcrossCommit;
    public byte supportsOpenStatementsAcrossRollback;
    public byte locatorsUpdateCopy;
    public byte supportsStatementPooling;
    public byte allProceduresAreCallable;
    public byte allTablesAreSelectable;
    public byte supportsMultipleResultSets;
    public byte supportsMultipleTransactions;
    public byte autoCommitFailureClosesAllResultSets;
    public byte supportsStoredFunctionsUsingCallSyntax;
    public byte generatedKeyAlwaysReturned;

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

    public DatabaseMetaDataStruct(Pointer p) {
        super(p);
        read();
    }

    public void setPointer(Pointer p) {
        useMemory(p);
        read();
    }
}
