#include "struct/meta_data.h"
#include "utils/string_utils.h"

MetaData::MetaData(const MetaData& other) {
    columnCount = other.columnCount;

    isAutoIncrement = other.isAutoIncrement;
    isCaseSensitive = other.isCaseSensitive;
    isSearchable = other.isSearchable;
    isCurrency = other.isCurrency;
    isNullable = other.isNullable;
    isSigned = other.isSigned;
    displaySize = other.displaySize;
    precision = other.precision;
    scale = other.scale;
    columnType = other.columnType;
    isReadOnly = other.isReadOnly;
    isWritable = other.isWritable;
    isDefinitelyWritable = other.isDefinitelyWritable;

    // Копируем строки через duplicate_string
    auto dup = [](const char16_t* src) -> const char16_t* {
        return utils::duplicate_string(src);
    };

    columnLabel = dup(other.columnLabel);
    columnName = dup(other.columnName);
    schemaName = dup(other.schemaName);
    tableName = dup(other.tableName);
    catalogName = dup(other.catalogName);
    columnTypeName = dup(other.columnTypeName);
}

MetaData::MetaData(const ResultSetMetaData& other) {
    columnCount = other.getColumnCount();

    isAutoIncrement = other.isAutoIncrement();
    isCaseSensitive = other.isCaseSensitive;
    isSearchable = other.isSearchable;
    isCurrency = other.isCurrency;
    isNullable = other.isNullable;
    isSigned = other.isSigned;
    displaySize = other.displaySize;
    precision = other.precision;
    scale = other.scale;
    columnType = other.columnType;
    isReadOnly = other.isReadOnly;
    isWritable = other.isWritable;
    isDefinitelyWritable = other.isDefinitelyWritable;
}

MetaData::~MetaData() {
    // Освобождаем строки, выделенные через malloc в utils::duplicate_string
    auto str_free = [](const char16_t* str) {
        if (str) free((void*) str);
    };

    str_free(columnLabel);
    str_free(columnName);
    str_free(schemaName);
    str_free(tableName);
    str_free(catalogName);
    str_free(columnTypeName);
}