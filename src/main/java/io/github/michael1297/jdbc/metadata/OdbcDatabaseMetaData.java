package io.github.michael1297.jdbc.metadata;

import java.util.Objects;

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

    @Override
    public boolean equals(Object o) {
        if (o == null || getClass() != o.getClass()) return false;
        OdbcDatabaseMetaData that = (OdbcDatabaseMetaData) o;
        return isReadOnly == that.isReadOnly && supportsTransactions == that.supportsTransactions && supportsSavepoints == that.supportsSavepoints && supportsNamedParameters == that.supportsNamedParameters && supportsBatchUpdates == that.supportsBatchUpdates && supportsUnion == that.supportsUnion && supportsUnionAll == that.supportsUnionAll && supportsLikeEscapeClause == that.supportsLikeEscapeClause && supportsGroupBy == that.supportsGroupBy && supportsGroupByUnrelated == that.supportsGroupByUnrelated && supportsGroupByBeyondSelect == that.supportsGroupByBeyondSelect && supportsOrderByUnrelated == that.supportsOrderByUnrelated && supportsAlterTableWithAddColumn == that.supportsAlterTableWithAddColumn && supportsColumnAliasing == that.supportsColumnAliasing && nullPlusNonNullIsNull == that.nullPlusNonNullIsNull && supportsExpressionsInOrderBy == that.supportsExpressionsInOrderBy && supportsSelectForUpdate == that.supportsSelectForUpdate && supportsStoredProcedures == that.supportsStoredProcedures && supportsSubqueriesInComparisons == that.supportsSubqueriesInComparisons && supportsSubqueriesInExists == that.supportsSubqueriesInExists && supportsSubqueriesInIns == that.supportsSubqueriesInIns && supportsSubqueriesInQuantifieds == that.supportsSubqueriesInQuantifieds && supportsCorrelatedSubqueries == that.supportsCorrelatedSubqueries && supportsIntegrityEnhancementFacility == that.supportsIntegrityEnhancementFacility && supportsOuterJoins == that.supportsOuterJoins && supportsFullOuterJoins == that.supportsFullOuterJoins && supportsLimitedOuterJoins == that.supportsLimitedOuterJoins && supportsSchemasInDataManipulation == that.supportsSchemasInDataManipulation && supportsSchemasInProcedureCalls == that.supportsSchemasInProcedureCalls && supportsSchemasInTableDefinitions == that.supportsSchemasInTableDefinitions && supportsSchemasInIndexDefinitions == that.supportsSchemasInIndexDefinitions && supportsSchemasInPrivilegeDefinitions == that.supportsSchemasInPrivilegeDefinitions && supportsCatalogsInDataManipulation == that.supportsCatalogsInDataManipulation && supportsCatalogsInProcedureCalls == that.supportsCatalogsInProcedureCalls && supportsCatalogsInTableDefinitions == that.supportsCatalogsInTableDefinitions && supportsCatalogsInIndexDefinitions == that.supportsCatalogsInIndexDefinitions && supportsCatalogsInPrivilegeDefinitions == that.supportsCatalogsInPrivilegeDefinitions && supportsPositionedDelete == that.supportsPositionedDelete && supportsPositionedUpdate == that.supportsPositionedUpdate && supportsOpenCursorsAcrossCommit == that.supportsOpenCursorsAcrossCommit && supportsOpenCursorsAcrossRollback == that.supportsOpenCursorsAcrossRollback && supportsOpenStatementsAcrossCommit == that.supportsOpenStatementsAcrossCommit && supportsOpenStatementsAcrossRollback == that.supportsOpenStatementsAcrossRollback && locatorsUpdateCopy == that.locatorsUpdateCopy && supportsStatementPooling == that.supportsStatementPooling && autoCommitFailureClosesAllResultSets == that.autoCommitFailureClosesAllResultSets && supportsStoredFunctionsUsingCallSyntax == that.supportsStoredFunctionsUsingCallSyntax && generatedKeyAlwaysReturned == that.generatedKeyAlwaysReturned && supportsTransactionIsolationLevel == that.supportsTransactionIsolationLevel && nullCollation == that.nullCollation && sqlStateType == that.sqlStateType && defaultTransactionIsolation == that.defaultTransactionIsolation && resultSetHoldability == that.resultSetHoldability && rowIdLifetime == that.rowIdLifetime && maxTableNameLength == that.maxTableNameLength && maxSchemaNameLength == that.maxSchemaNameLength && maxCatalogNameLength == that.maxCatalogNameLength && maxColumnNameLength == that.maxColumnNameLength && maxColumnsInGroupBy == that.maxColumnsInGroupBy && maxColumnsInOrderBy == that.maxColumnsInOrderBy && maxColumnsInSelect == that.maxColumnsInSelect && maxColumnsInTable == that.maxColumnsInTable && maxColumnsInIndex == that.maxColumnsInIndex && maxStatementLength == that.maxStatementLength && maxStatements == that.maxStatements && maxTablesInSelect == that.maxTablesInSelect && maxUserNameLength == that.maxUserNameLength && maxRowSize == that.maxRowSize && databaseMajorVersion == that.databaseMajorVersion && databaseMinorVersion == that.databaseMinorVersion && driverMajorVersion == that.driverMajorVersion && driverMinorVersion == that.driverMinorVersion && Objects.equals(databaseProductName, that.databaseProductName) && Objects.equals(databaseProductVersion, that.databaseProductVersion) && Objects.equals(driverName, that.driverName) && Objects.equals(driverVersion, that.driverVersion) && Objects.equals(identifierQuoteString, that.identifierQuoteString) && Objects.equals(schemaTerm, that.schemaTerm) && Objects.equals(procedureTerm, that.procedureTerm) && Objects.equals(catalogTerm, that.catalogTerm) && Objects.equals(catalogSeparator, that.catalogSeparator) && Objects.equals(sqlKeywords, that.sqlKeywords) && Objects.equals(numericFunctions, that.numericFunctions) && Objects.equals(stringFunctions, that.stringFunctions) && Objects.equals(systemFunctions, that.systemFunctions) && Objects.equals(timeDateFunctions, that.timeDateFunctions) && Objects.equals(searchStringEscape, that.searchStringEscape) && Objects.equals(extraNameCharacters, that.extraNameCharacters);
    }

    @Override
    public int hashCode() {
        return Objects.hash(databaseProductName, databaseProductVersion, driverName, driverVersion, identifierQuoteString, schemaTerm, procedureTerm, catalogTerm, catalogSeparator, sqlKeywords, numericFunctions, stringFunctions, systemFunctions, timeDateFunctions, searchStringEscape, extraNameCharacters, isReadOnly, supportsTransactions, supportsSavepoints, supportsNamedParameters, supportsBatchUpdates, supportsUnion, supportsUnionAll, supportsLikeEscapeClause, supportsGroupBy, supportsGroupByUnrelated, supportsGroupByBeyondSelect, supportsOrderByUnrelated, supportsAlterTableWithAddColumn, supportsColumnAliasing, nullPlusNonNullIsNull, supportsExpressionsInOrderBy, supportsSelectForUpdate, supportsStoredProcedures, supportsSubqueriesInComparisons, supportsSubqueriesInExists, supportsSubqueriesInIns, supportsSubqueriesInQuantifieds, supportsCorrelatedSubqueries, supportsIntegrityEnhancementFacility, supportsOuterJoins, supportsFullOuterJoins, supportsLimitedOuterJoins, supportsSchemasInDataManipulation, supportsSchemasInProcedureCalls, supportsSchemasInTableDefinitions, supportsSchemasInIndexDefinitions, supportsSchemasInPrivilegeDefinitions, supportsCatalogsInDataManipulation, supportsCatalogsInProcedureCalls, supportsCatalogsInTableDefinitions, supportsCatalogsInIndexDefinitions, supportsCatalogsInPrivilegeDefinitions, supportsPositionedDelete, supportsPositionedUpdate, supportsOpenCursorsAcrossCommit, supportsOpenCursorsAcrossRollback, supportsOpenStatementsAcrossCommit, supportsOpenStatementsAcrossRollback, locatorsUpdateCopy, supportsStatementPooling, autoCommitFailureClosesAllResultSets, supportsStoredFunctionsUsingCallSyntax, generatedKeyAlwaysReturned, supportsTransactionIsolationLevel, nullCollation, sqlStateType, defaultTransactionIsolation, resultSetHoldability, rowIdLifetime, maxTableNameLength, maxSchemaNameLength, maxCatalogNameLength, maxColumnNameLength, maxColumnsInGroupBy, maxColumnsInOrderBy, maxColumnsInSelect, maxColumnsInTable, maxColumnsInIndex, maxStatementLength, maxStatements, maxTablesInSelect, maxUserNameLength, maxRowSize, databaseMajorVersion, databaseMinorVersion, driverMajorVersion, driverMinorVersion);
    }

    @Override
    public String toString() {
        return "OdbcDatabaseMetaData{" +
                "databaseProductName='" + databaseProductName + '\'' +
                ", databaseProductVersion='" + databaseProductVersion + '\'' +
                ", driverName='" + driverName + '\'' +
                ", driverVersion='" + driverVersion + '\'' +
                ", identifierQuoteString='" + identifierQuoteString + '\'' +
                ", schemaTerm='" + schemaTerm + '\'' +
                ", procedureTerm='" + procedureTerm + '\'' +
                ", catalogTerm='" + catalogTerm + '\'' +
                ", catalogSeparator='" + catalogSeparator + '\'' +
                ", sqlKeywords='" + sqlKeywords + '\'' +
                ", numericFunctions='" + numericFunctions + '\'' +
                ", stringFunctions='" + stringFunctions + '\'' +
                ", systemFunctions='" + systemFunctions + '\'' +
                ", timeDateFunctions='" + timeDateFunctions + '\'' +
                ", searchStringEscape='" + searchStringEscape + '\'' +
                ", extraNameCharacters='" + extraNameCharacters + '\'' +
                ", isReadOnly=" + isReadOnly +
                ", supportsTransactions=" + supportsTransactions +
                ", supportsSavepoints=" + supportsSavepoints +
                ", supportsNamedParameters=" + supportsNamedParameters +
                ", supportsBatchUpdates=" + supportsBatchUpdates +
                ", supportsUnion=" + supportsUnion +
                ", supportsUnionAll=" + supportsUnionAll +
                ", supportsLikeEscapeClause=" + supportsLikeEscapeClause +
                ", supportsGroupBy=" + supportsGroupBy +
                ", supportsGroupByUnrelated=" + supportsGroupByUnrelated +
                ", supportsGroupByBeyondSelect=" + supportsGroupByBeyondSelect +
                ", supportsOrderByUnrelated=" + supportsOrderByUnrelated +
                ", supportsAlterTableWithAddColumn=" + supportsAlterTableWithAddColumn +
                ", supportsColumnAliasing=" + supportsColumnAliasing +
                ", nullPlusNonNullIsNull=" + nullPlusNonNullIsNull +
                ", supportsExpressionsInOrderBy=" + supportsExpressionsInOrderBy +
                ", supportsSelectForUpdate=" + supportsSelectForUpdate +
                ", supportsStoredProcedures=" + supportsStoredProcedures +
                ", supportsSubqueriesInComparisons=" + supportsSubqueriesInComparisons +
                ", supportsSubqueriesInExists=" + supportsSubqueriesInExists +
                ", supportsSubqueriesInIns=" + supportsSubqueriesInIns +
                ", supportsSubqueriesInQuantifieds=" + supportsSubqueriesInQuantifieds +
                ", supportsCorrelatedSubqueries=" + supportsCorrelatedSubqueries +
                ", supportsIntegrityEnhancementFacility=" + supportsIntegrityEnhancementFacility +
                ", supportsOuterJoins=" + supportsOuterJoins +
                ", supportsFullOuterJoins=" + supportsFullOuterJoins +
                ", supportsLimitedOuterJoins=" + supportsLimitedOuterJoins +
                ", supportsSchemasInDataManipulation=" + supportsSchemasInDataManipulation +
                ", supportsSchemasInProcedureCalls=" + supportsSchemasInProcedureCalls +
                ", supportsSchemasInTableDefinitions=" + supportsSchemasInTableDefinitions +
                ", supportsSchemasInIndexDefinitions=" + supportsSchemasInIndexDefinitions +
                ", supportsSchemasInPrivilegeDefinitions=" + supportsSchemasInPrivilegeDefinitions +
                ", supportsCatalogsInDataManipulation=" + supportsCatalogsInDataManipulation +
                ", supportsCatalogsInProcedureCalls=" + supportsCatalogsInProcedureCalls +
                ", supportsCatalogsInTableDefinitions=" + supportsCatalogsInTableDefinitions +
                ", supportsCatalogsInIndexDefinitions=" + supportsCatalogsInIndexDefinitions +
                ", supportsCatalogsInPrivilegeDefinitions=" + supportsCatalogsInPrivilegeDefinitions +
                ", supportsPositionedDelete=" + supportsPositionedDelete +
                ", supportsPositionedUpdate=" + supportsPositionedUpdate +
                ", supportsOpenCursorsAcrossCommit=" + supportsOpenCursorsAcrossCommit +
                ", supportsOpenCursorsAcrossRollback=" + supportsOpenCursorsAcrossRollback +
                ", supportsOpenStatementsAcrossCommit=" + supportsOpenStatementsAcrossCommit +
                ", supportsOpenStatementsAcrossRollback=" + supportsOpenStatementsAcrossRollback +
                ", locatorsUpdateCopy=" + locatorsUpdateCopy +
                ", supportsStatementPooling=" + supportsStatementPooling +
                ", autoCommitFailureClosesAllResultSets=" + autoCommitFailureClosesAllResultSets +
                ", supportsStoredFunctionsUsingCallSyntax=" + supportsStoredFunctionsUsingCallSyntax +
                ", generatedKeyAlwaysReturned=" + generatedKeyAlwaysReturned +
                ", supportsTransactionIsolationLevel=" + supportsTransactionIsolationLevel +
                ", nullCollation=" + nullCollation +
                ", sqlStateType=" + sqlStateType +
                ", defaultTransactionIsolation=" + defaultTransactionIsolation +
                ", resultSetHoldability=" + resultSetHoldability +
                ", rowIdLifetime=" + rowIdLifetime +
                ", maxTableNameLength=" + maxTableNameLength +
                ", maxSchemaNameLength=" + maxSchemaNameLength +
                ", maxCatalogNameLength=" + maxCatalogNameLength +
                ", maxColumnNameLength=" + maxColumnNameLength +
                ", maxColumnsInGroupBy=" + maxColumnsInGroupBy +
                ", maxColumnsInOrderBy=" + maxColumnsInOrderBy +
                ", maxColumnsInSelect=" + maxColumnsInSelect +
                ", maxColumnsInTable=" + maxColumnsInTable +
                ", maxColumnsInIndex=" + maxColumnsInIndex +
                ", maxStatementLength=" + maxStatementLength +
                ", maxStatements=" + maxStatements +
                ", maxTablesInSelect=" + maxTablesInSelect +
                ", maxUserNameLength=" + maxUserNameLength +
                ", maxRowSize=" + maxRowSize +
                ", databaseMajorVersion=" + databaseMajorVersion +
                ", databaseMinorVersion=" + databaseMinorVersion +
                ", driverMajorVersion=" + driverMajorVersion +
                ", driverMinorVersion=" + driverMinorVersion +
                '}';
    }
}
