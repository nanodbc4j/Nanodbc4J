#pragma once
#include "core/database_metadata.hpp"

#ifdef __cplusplus
extern "C" {
#endif

    struct CDatabaseMetaData {
        // === Строковые значения ===
        const wchar_t* databaseProductName = nullptr;
        const wchar_t* databaseProductVersion = nullptr;
        const wchar_t* driverName = nullptr;
        const wchar_t* driverVersion = nullptr;
        const wchar_t* identifierQuoteString = nullptr;
        const wchar_t* schemaTerm = nullptr;
        const wchar_t* procedureTerm = nullptr;
        const wchar_t* catalogTerm = nullptr;
        const wchar_t* catalogSeparator = nullptr;
        const wchar_t* sqlKeywords = nullptr;
        const wchar_t* numericFunctions = nullptr;
        const wchar_t* stringFunctions = nullptr;
        const wchar_t* systemFunctions = nullptr;
        const wchar_t* timeDateFunctions = nullptr;
        const wchar_t* searchStringEscape = nullptr;
        const wchar_t* extraNameCharacters = nullptr;
        const wchar_t* userName = nullptr;

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
        bool allProceduresAreCallable = false;
        bool allTablesAreSelectable = false;
        bool supportsMultipleResultSets = false;
        bool supportsMultipleTransactions = false;
        bool autoCommitFailureClosesAllResultSets = false;
        bool supportsStoredFunctionsUsingCallSyntax = false;
        bool generatedKeyAlwaysReturned = false;
        bool nullsAreSortedHigh = false;
        bool nullsAreSortedLow = false;
        bool nullsAreSortedAtStart = false;
        bool nullsAreSortedAtEnd = false;
        bool usesLocalFiles = false;
        bool usesLocalFilePerTable = false;
        bool supportsMixedCaseIdentifiers = false;
        bool storesUpperCaseIdentifiers = false;
        bool storesLowerCaseIdentifiers = false;
        bool storesMixedCaseIdentifiers = false;
        bool supportsMixedCaseQuotedIdentifiers = false;
        bool storesUpperCaseQuotedIdentifiers = false;
        bool storesLowerCaseQuotedIdentifiers = false;
        bool storesMixedCaseQuotedIdentifiers = false;
        bool supportsAlterTableWithDropColumn = false;
        bool supportsConvert = false;
        bool supportsTableCorrelationNames = false;
        bool supportsDifferentTableCorrelationNames = false;
        bool supportsNonNullableColumns = false;
        bool supportsMinimumSQLGrammar = false;
        bool supportsCoreSQLGrammar = false;
        bool supportsExtendedSQLGrammar = false;
        bool supportsANSI92EntryLevelSQL = false;
        bool supportsANSI92IntermediateSQL = false;
        bool supportsANSI92FullSQL = false;
        bool supportsDataDefinitionAndDataManipulationTransactions = false;
        bool supportsDataManipulationTransactionsOnly = false;
        bool dataDefinitionCausesTransactionCommit = false;
        bool dataDefinitionIgnoredInTransactions = false;
        bool supportsMultipleOpenResults = false;
        bool supportsGetGeneratedKeys = false;
        bool doesMaxRowSizeIncludeBlobs = false;
        bool isCatalogAtStart = false;

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
        int getMaxBinaryLiteralLength = 0;
        int getMaxCharLiteralLength = 0;
        int getMaxConnections = 0;
        int getMaxCursorNameLength = 0;
        int getMaxIndexLength = 0;
        int getMaxProcedureNameLength = 0;

        CDatabaseMetaData() = default;
        CDatabaseMetaData(const CDatabaseMetaData& other);
        explicit CDatabaseMetaData(const DatabaseMetaData& other);
        ~CDatabaseMetaData();
    };

#ifdef __cplusplus
} // extern "C"
#endif