#pragma once
#include "core/database_metadata.hpp"

#ifdef __cplusplus
extern "C" {
#endif

    struct CDatabaseMetaData {
        // === Строковые значения ===
        const char16_t* databaseProductName = nullptr;
        const char16_t* databaseProductVersion = nullptr;
        const char16_t* driverName = nullptr;
        const char16_t* driverVersion = nullptr;
        const char16_t* identifierQuoteString = nullptr;
        const char16_t* schemaTerm = nullptr;
        const char16_t* procedureTerm = nullptr;
        const char16_t* catalogTerm = nullptr;
        const char16_t* catalogSeparator = nullptr;
        const char16_t* sqlKeywords = nullptr;
        const char16_t* numericFunctions = nullptr;
        const char16_t* stringFunctions = nullptr;
        const char16_t* systemFunctions = nullptr;
        const char16_t* timeDateFunctions = nullptr;
        const char16_t* searchStringEscape = nullptr;
        const char16_t* extraNameCharacters = nullptr;

        // === Булевы значения ===
        bool isReadOnly = false;
        bool supportsTransactions = false;
        bool supportsSavepoints = false;
        bool supportsNamedParameters = false;
        bool supportsBatchUpdates = false;
        bool supportsUnion = false;
        bool supportsUnionAll = false;
        bool supportsLikeEscapeClause = false;
        bool supportsGroupBy = false;
        bool supportsGroupByUnrelated = false;
        bool supportsGroupByBeyondSelect = false;
        bool supportsOrderByUnrelated = false;
        bool supportsAlterTableWithAddColumn = false;
        bool supportsColumnAliasing = false;
        bool nullPlusNonNullIsNull = false;
        bool supportsExpressionsInOrderBy = false;
        bool supportsSelectForUpdate = false;
        bool supportsStoredProcedures = false;
        bool supportsSubqueriesInComparisons = false;
        bool supportsSubqueriesInExists = false;
        bool supportsSubqueriesInIns = false;
        bool supportsSubqueriesInQuantifieds = false;
        bool supportsCorrelatedSubqueries = false;
        bool supportsIntegrityEnhancementFacility = false;
        bool supportsOuterJoins = false;
        bool supportsFullOuterJoins = false;
        bool supportsLimitedOuterJoins = false;
        bool supportsSchemasInDataManipulation = false;
        bool supportsSchemasInProcedureCalls = false;
        bool supportsSchemasInTableDefinitions = false;
        bool supportsSchemasInIndexDefinitions = false;
        bool supportsSchemasInPrivilegeDefinitions = false;
        bool supportsCatalogsInDataManipulation = false;
        bool supportsCatalogsInProcedureCalls = false;
        bool supportsCatalogsInTableDefinitions = false;
        bool supportsCatalogsInIndexDefinitions = false;
        bool supportsCatalogsInPrivilegeDefinitions = false;
        bool supportsPositionedDelete = false;
        bool supportsPositionedUpdate = false;
        bool supportsOpenCursorsAcrossCommit = false;
        bool supportsOpenCursorsAcrossRollback = false;
        bool supportsOpenStatementsAcrossCommit = false;
        bool supportsOpenStatementsAcrossRollback = false;
        bool locatorsUpdateCopy = false;
        bool supportsStatementPooling = false;
        bool autoCommitFailureClosesAllResultSets = false;
        bool supportsStoredFunctionsUsingCallSyntax = false;
        bool generatedKeyAlwaysReturned = false;

        // === Целочисленные значения ===
        int supportsTransactionIsolationLevel = 0;
        int nullCollation = 0;
        int sqlStateType = 0;
        int defaultTransactionIsolation = 0;
        int resultSetHoldability = 0;
        int rowIdLifetime = 0;
        int maxTableNameLength = 0;
        int maxSchemaNameLength = 0;
        int maxCatalogNameLength = 0;
        int maxColumnNameLength = 0;
        int maxColumnsInGroupBy = 0;
        int maxColumnsInOrderBy = 0;
        int maxColumnsInSelect = 0;
        int maxColumnsInTable = 0;
        int maxColumnsInIndex = 0;
        int maxStatementLength = 0;
        int maxStatements = 0;
        int maxTablesInSelect = 0;
        int maxUserNameLength = 0;
        int maxRowSize = 0;
        int databaseMajorVersion = 0;
        int databaseMinorVersion = 0;
        int driverMajorVersion = 0;
        int driverMinorVersion = 0;

        CDatabaseMetaData() = default;
        CDatabaseMetaData(const CDatabaseMetaData& other);
        CDatabaseMetaData(const DatabaseMetaData& other);
        ~CDatabaseMetaData();
    };

#ifdef __cplusplus
} // extern "C"
#endif