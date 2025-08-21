#include "struct/meta_data_c.h"
#include "utils/string_utils.hpp"

// Копируем строки через duplicate_string
static auto& dup = utils::duplicate_string<char16_t>;

// Освобождаем строки, выделенные через malloc в utils::duplicate_string
static auto str_free = [](const char16_t* str) {
    if (str) free((void*) str);
};

inline static const char16_t* convert(const std::wstring& str) {
    auto u16str = utils::to_u16string(str);
    return dup(u16str.c_str());
}

MetaData::ColumnMetaData::ColumnMetaData(const ColumnMetaData& other) {
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
    columnLabel = dup(other.columnLabel);
    columnName = dup(other.columnName);
    schemaName = dup(other.schemaName);
    tableName = dup(other.tableName);
    catalogName = dup(other.catalogName);
    columnTypeName = dup(other.columnTypeName);
    columnClassName = dup(other.columnClassName);
}

MetaData::ColumnMetaData::~ColumnMetaData() {
    str_free(columnLabel);
    str_free(columnName);
    str_free(schemaName);
    str_free(tableName);
    str_free(catalogName);
    str_free(columnTypeName);
    str_free(columnClassName);

    isAutoIncrement = false;
    isCaseSensitive = false;
    isSearchable = false;
    isCurrency = false;
    isNullable = 0;
    isSigned = false;
    displaySize = 0;
    precision = 0;
    scale = 0;
    columnType = 0;
    isReadOnly = false;
    isWritable = false;
    isDefinitelyWritable = false;
    columnLabel = nullptr;
    columnName = nullptr;
    schemaName = nullptr;
    tableName = nullptr;
    catalogName = nullptr;
    columnTypeName = nullptr;
    columnClassName = nullptr;
}

MetaData::MetaData(const MetaData& other) {
    columnCount = other.columnCount;

    if (columnCount) {
        column = new const ColumnMetaData* [columnCount];
        for (int i = 0; i < columnCount; ++i) {
            column[i] = new ColumnMetaData(*other.column[i]);
        }
    }
}

MetaData::MetaData(const ResultSetMetaData& other) {
    columnCount = other.getColumnCount();

    if (columnCount) {
        column = new const ColumnMetaData * [columnCount];
        for (int i = 0; i < columnCount; ++i) {
            ColumnMetaData* data = new ColumnMetaData();

            // Отсчет начинается с 1
            data->isAutoIncrement = other.isAutoIncrement(i + 1);
            data->isCaseSensitive = other.isCaseSensitive(i + 1);
            data->isSearchable = other.isSearchable(i + 1);
            data->isCurrency = other.isCurrency(i + 1);
            data->isNullable = other.isNullable(i + 1);
            data->isSigned = other.isSigned(i + 1);
            data->displaySize = other.getColumnDisplaySize(i + 1);
            data->precision = other.getPrecision(i + 1);
            data->scale = other.getScale(i + 1);
            data->columnType = other.getColumnType(i + 1);
            data->isReadOnly = other.isReadOnly(i + 1);
            data->isWritable = other.isWritable(i + 1);
            data->isDefinitelyWritable = other.isDefinitelyWritable(i + 1);
            data->columnLabel = convert(other.getColumnLabel(i + 1));
            data->columnName = convert(other.getColumnName(i + 1));
            data->schemaName = convert(other.getSchemaName(i + 1));
            data->tableName = convert(other.getTableName(i + 1));
            data->catalogName = convert(other.getCatalogName(i + 1));
            data->columnTypeName = convert(other.getColumnTypeName(i + 1));
            data->columnClassName = convert(other.getColumnClassName(i + 1));

            column[i] = data;
        }
    }    
}

MetaData::~MetaData() {
    if (columnCount) {
        for (int i = 0; i < columnCount; ++i) {
            delete column[i];
        }

        delete[] column;
        column = nullptr;
    }
    columnCount = 0;
}