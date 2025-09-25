package io.github.nanodbc4j.internal.handler;

import io.github.nanodbc4j.internal.NativeDB;
import io.github.nanodbc4j.internal.cstruct.NativeError;
import io.github.nanodbc4j.internal.dto.DatabaseMetaDataDto;
import io.github.nanodbc4j.internal.cstruct.DatabaseMetaDataStruct;
import io.github.nanodbc4j.internal.pointer.ConnectionPtr;
import io.github.nanodbc4j.internal.pointer.ResultSetPtr;
import lombok.NonNull;
import lombok.experimental.UtilityClass;

import static io.github.nanodbc4j.internal.handler.Handler.*;

/**
 * Converts native ODBC metadata (pointer) to Java OdbcDatabaseMetaData object.
 */
@UtilityClass
public class DatabaseMetaDataHandler {

    public static DatabaseMetaDataDto processerMetaData(@NonNull DatabaseMetaDataStruct metaDataStruct) {
        DatabaseMetaDataDto metaData = new DatabaseMetaDataDto();

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
        metaData.allProceduresAreCallable = metaDataStruct.allProceduresAreCallable != 0;
        metaData.allTablesAreSelectable = metaDataStruct.allTablesAreSelectable != 0;
        metaData.supportsMultipleResultSets = metaDataStruct.supportsMultipleResultSets != 0;
        metaData.supportsMultipleTransactions = metaDataStruct.supportsMultipleTransactions != 0;
        metaData.autoCommitFailureClosesAllResultSets = metaDataStruct.autoCommitFailureClosesAllResultSets != 0;
        metaData.supportsStoredFunctionsUsingCallSyntax = metaDataStruct.supportsStoredFunctionsUsingCallSyntax != 0;
        metaData.generatedKeyAlwaysReturned = metaDataStruct.generatedKeyAlwaysReturned != 0;

        // === Целочисленные значения ===
        metaData.supportsTransactionIsolationLevel = metaDataStruct.supportsTransactionIsolationLevel;
        metaData.nullCollation = metaDataStruct.nullCollation;
        metaData.sqlStateType = metaDataStruct.sqlStateType;
        metaData.defaultTransactionIsolation = metaDataStruct.defaultTransactionIsolation;
        metaData.resultSetHoldability = metaDataStruct.resultSetHoldability;
        metaData.rowIdLifetime = metaDataStruct.rowIdLifetime;
        metaData.maxTableNameLength = metaDataStruct.maxTableNameLength;
        metaData.maxSchemaNameLength = metaDataStruct.maxSchemaNameLength;
        metaData.maxCatalogNameLength = metaDataStruct.maxCatalogNameLength;
        metaData.maxColumnNameLength = metaDataStruct.maxColumnNameLength;
        metaData.maxColumnsInGroupBy = metaDataStruct.maxColumnsInGroupBy;
        metaData.maxColumnsInOrderBy = metaDataStruct.maxColumnsInOrderBy;
        metaData.maxColumnsInSelect = metaDataStruct.maxColumnsInSelect;
        metaData.maxColumnsInTable = metaDataStruct.maxColumnsInTable;
        metaData.maxColumnsInIndex = metaDataStruct.maxColumnsInIndex;
        metaData.maxStatementLength = metaDataStruct.maxStatementLength;
        metaData.maxStatements = metaDataStruct.maxStatements;
        metaData.maxTablesInSelect = metaDataStruct.maxTablesInSelect;
        metaData.maxUserNameLength = metaDataStruct.maxUserNameLength;
        metaData.maxRowSize = metaDataStruct.maxRowSize;
        metaData.databaseMajorVersion = metaDataStruct.databaseMajorVersion;
        metaData.databaseMinorVersion = metaDataStruct.databaseMinorVersion;
        metaData.driverMajorVersion = metaDataStruct.driverMajorVersion;
        metaData.driverMinorVersion = metaDataStruct.driverMinorVersion;

        return metaData;
    }

    public ResultSetPtr getTables(ConnectionPtr conn, String catalog, String schema, String table, String type) {
        NativeError nativeError = new NativeError();
        try {
            ResultSetPtr resultSetPtr =
                    NativeDB.INSTANCE.get_database_meta_data_tables(conn,
                            catalog + NUL_CHAR, schema + NUL_CHAR,
                            table + NUL_CHAR,
                            type + NUL_CHAR,
                            nativeError);
            throwIfNativeError(nativeError);
            return resultSetPtr;
        } finally {
            NativeDB.INSTANCE.clear_native_error(nativeError);
        }
    }

    public ResultSetPtr getSchemas(ConnectionPtr conn) {
        NativeError nativeError = new NativeError();
        try {
            ResultSetPtr resultSetPtr = NativeDB.INSTANCE.get_database_meta_data_schemas(conn, nativeError);
            throwIfNativeError(nativeError);
            return resultSetPtr;
        } finally {
            NativeDB.INSTANCE.clear_native_error(nativeError);
        }
    }

    public ResultSetPtr getSchemas(ConnectionPtr conn, String catalog, String schemaPattern) {
        NativeError nativeError = new NativeError();
        try {
            // используем get_database_meta_data_tables
            ResultSetPtr resultSetPtr =
                    NativeDB.INSTANCE.get_database_meta_data_tables(conn,
                            catalog + NUL_CHAR,
                            schemaPattern + NUL_CHAR,
                            NUL_TERMINATOR,
                            NUL_TERMINATOR,
                            nativeError);
            throwIfNativeError(nativeError);
            return resultSetPtr;
        } finally {
            NativeDB.INSTANCE.clear_native_error(nativeError);
        }
    }

    public ResultSetPtr getCatalogs(ConnectionPtr conn) {
        NativeError nativeError = new NativeError();
        try {
            ResultSetPtr resultSetPtr = NativeDB.INSTANCE.get_database_meta_data_catalogs(conn, nativeError);
            throwIfNativeError(nativeError);
            return resultSetPtr;
        } finally {
            NativeDB.INSTANCE.clear_native_error(nativeError);
        }
    }

    public ResultSetPtr getTableTypes(ConnectionPtr conn) {
        NativeError nativeError = new NativeError();
        try {
            ResultSetPtr resultSetPtr = NativeDB.INSTANCE.get_database_meta_data_table_types(conn, nativeError);
            throwIfNativeError(nativeError);
            return resultSetPtr;
        } finally {
            NativeDB.INSTANCE.clear_native_error(nativeError);
        }
    }

    public ResultSetPtr getColumns(ConnectionPtr conn, String catalog, String schema, String table, String column) {
        NativeError nativeError = new NativeError();
        try {
            ResultSetPtr resultSetPtr =
                    NativeDB.INSTANCE.get_database_meta_data_columns(conn,
                            catalog + NUL_CHAR,
                            schema + NUL_CHAR,
                            table + NUL_CHAR,
                            column + NUL_CHAR,
                            nativeError);
            throwIfNativeError(nativeError);
            return resultSetPtr;
        } finally {
            NativeDB.INSTANCE.clear_native_error(nativeError);
        }
    }

    public ResultSetPtr getPrimaryKeys(ConnectionPtr conn, String catalog, String schema, String table) {
        NativeError nativeError = new NativeError();
        try {
            ResultSetPtr resultSetPtr =
                    NativeDB.INSTANCE.get_database_meta_data_primary_keys(conn,
                            catalog + NUL_CHAR,
                            schema + NUL_CHAR,
                            table + NUL_CHAR,
                            nativeError);
            throwIfNativeError(nativeError);
            return resultSetPtr;
        } finally {
            NativeDB.INSTANCE.clear_native_error(nativeError);
        }
    }

    public ResultSetPtr getImportedKeys(ConnectionPtr conn, String catalog, String schema, String table) {
        NativeError nativeError = new NativeError();
        try {
            ResultSetPtr resultSetPtr =
                    NativeDB.INSTANCE.get_database_meta_data_imported_keys(conn,
                            catalog + NUL_CHAR,
                            schema + NUL_CHAR,
                            table + NUL_CHAR,
                            nativeError);
            throwIfNativeError(nativeError);
            return resultSetPtr;
        } finally {
            NativeDB.INSTANCE.clear_native_error(nativeError);
        }
    }

    public ResultSetPtr getExportedKeys(ConnectionPtr conn, String catalog, String schema, String table) {
        NativeError nativeError = new NativeError();
        try {
            ResultSetPtr resultSetPtr =
                    NativeDB.INSTANCE.get_database_meta_data_exported_keys(conn,
                            catalog + NUL_CHAR,
                            schema + NUL_CHAR,
                            table + NUL_CHAR,
                            nativeError);
            throwIfNativeError(nativeError);
            return resultSetPtr;
        } finally {
            NativeDB.INSTANCE.clear_native_error(nativeError);
        }
    }

    public ResultSetPtr getTypeInfo(ConnectionPtr conn) {
        NativeError nativeError = new NativeError();
        try {
            ResultSetPtr resultSetPtr =
                    NativeDB.INSTANCE.get_database_meta_data_type_info(conn, nativeError);
            throwIfNativeError(nativeError);
            return resultSetPtr;
        } finally {
            NativeDB.INSTANCE.clear_native_error(nativeError);
        }
    }

    public ResultSetPtr getProcedures(ConnectionPtr conn, String catalog, String schemaPattern, String procedureNamePattern) {
        NativeError nativeError = new NativeError();
        try {
            ResultSetPtr resultSetPtr =
                    NativeDB.INSTANCE.get_database_meta_data_procedures(conn,
                            catalog + NUL_CHAR,
                            schemaPattern + NUL_CHAR,
                            procedureNamePattern + NUL_CHAR,
                            nativeError);
            throwIfNativeError(nativeError);
            return resultSetPtr;
        } finally {
            NativeDB.INSTANCE.clear_native_error(nativeError);
        }
    }

    public ResultSetPtr getProcedureColumns(ConnectionPtr conn, String catalog, String schemaPattern, String procedureNamePattern, String columnNamePattern) {
        NativeError nativeError = new NativeError();
        try {
            ResultSetPtr resultSetPtr =
                    NativeDB.INSTANCE.get_database_meta_data_procedure_columns(conn,
                            catalog + NUL_CHAR,
                            schemaPattern + NUL_CHAR,
                            procedureNamePattern + NUL_CHAR,
                            columnNamePattern + NUL_CHAR,
                            nativeError);
            throwIfNativeError(nativeError);
            return resultSetPtr;
        } finally {
            NativeDB.INSTANCE.clear_native_error(nativeError);
        }
    }
}
