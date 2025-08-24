package io.github.michael1297.internal.handler;

import com.sun.jna.Pointer;
import io.github.michael1297.internal.pointer.OdbcDatabaseMetaDataPrt;
import io.github.michael1297.internal.struct.DatabaseMetaDataStruct;
import io.github.michael1297.jdbc.metadata.OdbcDatabaseMetaData;

/**
 * Converts native ODBC metadata (pointer) to Java OdbcDatabaseMetaData object.
 */
public class OdbcDatabaseMetaDataHandler {
    // Static methods only
    private OdbcDatabaseMetaDataHandler(){
    }

    public static OdbcDatabaseMetaData processerMetaData(OdbcDatabaseMetaDataPrt metaDataPtr){
        if (metaDataPtr == null || metaDataPtr.getPointer() == null) {
            throw new IllegalArgumentException("MetaDataPtr is null");
        }

        OdbcDatabaseMetaData metaData = new OdbcDatabaseMetaData();
        DatabaseMetaDataStruct metaDataStruct = new DatabaseMetaDataStruct(metaDataPtr.getPointer());

        // === Строковые значения ===
        metaData.databaseProductName = getWideString(metaDataStruct.databaseProductName);
        metaData.databaseProductVersion = getWideString(metaDataStruct.databaseProductVersion);
        metaData.driverName = getWideString(metaDataStruct.driverName);
        metaData.driverVersion = getWideString(metaDataStruct.driverVersion);
        metaData.identifierQuoteString = getWideString(metaDataStruct.identifierQuoteString);
        metaData.schemaTerm = getWideString(metaDataStruct.schemaTerm);
        metaData.procedureTerm = getWideString(metaDataStruct.procedureTerm);
        metaData.catalogTerm = getWideString(metaDataStruct.catalogTerm);
        metaData.catalogSeparator = getWideString(metaDataStruct.catalogSeparator);
        metaData.sqlKeywords = getWideString(metaDataStruct.sqlKeywords);
        metaData.numericFunctions = getWideString(metaDataStruct.numericFunctions);
        metaData.stringFunctions = getWideString(metaDataStruct.stringFunctions);
        metaData.systemFunctions = getWideString(metaDataStruct.systemFunctions);
        metaData.timeDateFunctions = getWideString(metaDataStruct.timeDateFunctions);
        metaData.searchStringEscape = getWideString(metaDataStruct.searchStringEscape);
        metaData.extraNameCharacters = getWideString(metaDataStruct.extraNameCharacters);

        // === Булевы значения ===
        metaData.isReadOnly = metaDataStruct.isReadOnly != 0;
        metaData.supportsTransactions = metaDataStruct.supportsTransactions != 0;
        metaData.supportsSavepoints = metaDataStruct.supportsSavepoints != 0;
        metaData.supportsNamedParameters = metaDataStruct.supportsNamedParameters != 0;
        metaData.supportsBatchUpdates = metaDataStruct.supportsBatchUpdates != 0;
        metaData.supportsUnion = metaDataStruct.supportsUnion != 0;
        metaData.supportsUnionAll = metaDataStruct.supportsUnionAll != 0;
        metaData.supportsLikeEscapeClause = metaDataStruct.supportsLikeEscapeClause != 0;
        metaData.supportsGroupBy = metaDataStruct.supportsGroupBy != 0;
        metaData.supportsGroupByUnrelated = metaDataStruct.supportsGroupByUnrelated != 0;
        metaData.supportsGroupByBeyondSelect = metaDataStruct.supportsGroupByBeyondSelect != 0;
        metaData.supportsOrderByUnrelated = metaDataStruct.supportsOrderByUnrelated != 0;
        metaData.supportsAlterTableWithAddColumn = metaDataStruct.supportsAlterTableWithAddColumn != 0;
        metaData.supportsColumnAliasing = metaDataStruct.supportsColumnAliasing != 0;
        metaData.nullPlusNonNullIsNull = metaDataStruct.nullPlusNonNullIsNull != 0;
        metaData.supportsExpressionsInOrderBy = metaDataStruct.supportsExpressionsInOrderBy != 0;
        metaData.supportsSelectForUpdate = metaDataStruct.supportsSelectForUpdate != 0;
        metaData.supportsStoredProcedures = metaDataStruct.supportsStoredProcedures != 0;
        metaData.supportsSubqueriesInComparisons = metaDataStruct.supportsSubqueriesInComparisons != 0;
        metaData.supportsSubqueriesInExists = metaDataStruct.supportsSubqueriesInExists != 0;
        metaData.supportsSubqueriesInIns = metaDataStruct.supportsSubqueriesInIns != 0;
        metaData.supportsSubqueriesInQuantifieds = metaDataStruct.supportsSubqueriesInQuantifieds != 0;
        metaData.supportsCorrelatedSubqueries = metaDataStruct.supportsCorrelatedSubqueries != 0;
        metaData.supportsIntegrityEnhancementFacility = metaDataStruct.supportsIntegrityEnhancementFacility != 0;
        metaData.supportsOuterJoins = metaDataStruct.supportsOuterJoins != 0;
        metaData.supportsFullOuterJoins = metaDataStruct.supportsFullOuterJoins != 0;
        metaData.supportsLimitedOuterJoins = metaDataStruct.supportsLimitedOuterJoins != 0;
        metaData.supportsSchemasInDataManipulation = metaDataStruct.supportsSchemasInDataManipulation != 0;
        metaData.supportsSchemasInProcedureCalls = metaDataStruct.supportsSchemasInProcedureCalls != 0;
        metaData.supportsSchemasInTableDefinitions = metaDataStruct.supportsSchemasInTableDefinitions != 0;
        metaData.supportsSchemasInIndexDefinitions = metaDataStruct.supportsSchemasInIndexDefinitions != 0;
        metaData.supportsSchemasInPrivilegeDefinitions = metaDataStruct.supportsSchemasInPrivilegeDefinitions != 0;
        metaData.supportsCatalogsInDataManipulation = metaDataStruct.supportsCatalogsInDataManipulation != 0;
        metaData.supportsCatalogsInProcedureCalls = metaDataStruct.supportsCatalogsInProcedureCalls != 0;
        metaData.supportsCatalogsInTableDefinitions = metaDataStruct.supportsCatalogsInTableDefinitions != 0;
        metaData.supportsCatalogsInIndexDefinitions = metaDataStruct.supportsCatalogsInIndexDefinitions != 0;
        metaData.supportsCatalogsInPrivilegeDefinitions = metaDataStruct.supportsCatalogsInPrivilegeDefinitions != 0;
        metaData.supportsPositionedDelete = metaDataStruct.supportsPositionedDelete != 0;
        metaData.supportsPositionedUpdate = metaDataStruct.supportsPositionedUpdate != 0;
        metaData.supportsOpenCursorsAcrossCommit = metaDataStruct.supportsOpenCursorsAcrossCommit != 0;
        metaData.supportsOpenCursorsAcrossRollback = metaDataStruct.supportsOpenCursorsAcrossRollback != 0;
        metaData.supportsOpenStatementsAcrossCommit = metaDataStruct.supportsOpenStatementsAcrossCommit != 0;
        metaData.supportsOpenStatementsAcrossRollback = metaDataStruct.supportsOpenStatementsAcrossRollback != 0;
        metaData.locatorsUpdateCopy = metaDataStruct.locatorsUpdateCopy != 0;
        metaData.supportsStatementPooling = metaDataStruct.supportsStatementPooling != 0;
        metaData.autoCommitFailureClosesAllResultSets = metaDataStruct.autoCommitFailureClosesAllResultSets != 0;
        metaData.supportsStoredFunctionsUsingCallSyntax = metaDataStruct.supportsStoredFunctionsUsingCallSyntax != 0;
        metaData.generatedKeyAlwaysReturned = metaDataStruct.generatedKeyAlwaysReturned != 0;

        // === Целочисленные значения ===
        metaData.supportsTransactionIsolationLevel =  metaDataStruct.supportsTransactionIsolationLevel;
        metaData.nullCollation =  metaDataStruct.nullCollation;
        metaData.sqlStateType =  metaDataStruct.sqlStateType;
        metaData.defaultTransactionIsolation =  metaDataStruct.defaultTransactionIsolation;
        metaData.resultSetHoldability =  metaDataStruct.resultSetHoldability;
        metaData.rowIdLifetime =  metaDataStruct.rowIdLifetime;
        metaData.maxTableNameLength =  metaDataStruct.maxTableNameLength;
        metaData.maxSchemaNameLength =  metaDataStruct.maxSchemaNameLength;
        metaData.maxCatalogNameLength =  metaDataStruct.maxCatalogNameLength;
        metaData.maxColumnNameLength =  metaDataStruct.maxColumnNameLength;
        metaData.maxColumnsInGroupBy =  metaDataStruct.maxColumnsInGroupBy;
        metaData.maxColumnsInOrderBy =  metaDataStruct.maxColumnsInOrderBy;
        metaData.maxColumnsInSelect =  metaDataStruct.maxColumnsInSelect;
        metaData.maxColumnsInTable =  metaDataStruct.maxColumnsInTable;
        metaData.maxColumnsInIndex =  metaDataStruct.maxColumnsInIndex;
        metaData.maxStatementLength =  metaDataStruct.maxStatementLength;
        metaData.maxStatements =  metaDataStruct.maxStatements;
        metaData.maxTablesInSelect =  metaDataStruct.maxTablesInSelect;
        metaData.maxUserNameLength =  metaDataStruct.maxUserNameLength;
        metaData.maxRowSize =  metaDataStruct.maxRowSize;
        metaData.databaseMajorVersion =  metaDataStruct.databaseMajorVersion;
        metaData.databaseMinorVersion =  metaDataStruct.databaseMinorVersion;
        metaData.driverMajorVersion =  metaDataStruct.driverMajorVersion;
        metaData.driverMinorVersion =  metaDataStruct.driverMinorVersion;

        return metaData;
    }

    private static String getWideString(Pointer p){
        if(p == null || p.equals(Pointer.NULL)) {
            return null;
        }
        return p.getWideString(0);
    }
}
