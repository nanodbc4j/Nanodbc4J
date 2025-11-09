#pragma once
#include "core/result_set_meta_data.hpp"
#include "api/api.h"

#ifdef __cplusplus
extern "C" {
#endif

	struct CResultSetMetaData {

        struct ColumnMetaData {
            bool isAutoIncrement = false;
            bool isCaseSensitive = false;
            bool isSearchable = false;
            bool isCurrency = false;
            int isNullable = 0;
            bool isSigned = false;
            int displaySize = 0;
            int precision = 0;
            int scale = 0;
            int columnType = 0;
            bool isReadOnly = false;
            bool isWritable = false;
            bool isDefinitelyWritable = false;

            ApiChar* columnLabel = nullptr;
            ApiChar* columnName = nullptr;
            ApiChar* schemaName = nullptr;
            ApiChar* tableName = nullptr;
            ApiChar* catalogName = nullptr;
            ApiChar* columnTypeName = nullptr;
            ApiChar* columnClassName = nullptr;

            ColumnMetaData() = default;
            ColumnMetaData(const ColumnMetaData& other);
            ~ColumnMetaData();
        };

        int columnCount = 0;
        const ColumnMetaData** column = nullptr;

        CResultSetMetaData() = default;
        CResultSetMetaData(const CResultSetMetaData& other);
        explicit CResultSetMetaData(const ResultSetMetaData& other);
        ~CResultSetMetaData();
	};

#ifdef __cplusplus
} // extern "C"
#endif