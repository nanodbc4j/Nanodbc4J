#pragma once
#include "core/result_set_meta_data.hpp"

#ifdef __cplusplus
extern "C" {
#endif

	struct MetaData {

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

            const char16_t* columnLabel = nullptr;
            const char16_t* columnName = nullptr;
            const char16_t* schemaName = nullptr;
            const char16_t* tableName = nullptr;
            const char16_t* catalogName = nullptr;
            const char16_t* columnTypeName = nullptr;
            const char16_t* columnClassName = nullptr;

            ColumnMetaData() = default;
            ColumnMetaData(const ColumnMetaData& other);
            ~ColumnMetaData();
        };

        int columnCount = 0;
        const ColumnMetaData** column = nullptr;

        MetaData() = default;
        MetaData(const MetaData& other);
        MetaData(const ResultSetMetaData& other);
        ~MetaData();
	};

#ifdef __cplusplus
} // extern "C"
#endif