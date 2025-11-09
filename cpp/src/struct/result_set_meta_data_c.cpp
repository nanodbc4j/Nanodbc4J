#include "struct/result_set_meta_data_c.h"
#include "utils/string_utils.hpp"
#include "utils/logger.hpp"

using namespace utils;

static ApiChar* convert(const ApiString& str) {
    LOG_TRACE("Converting string to char array: '{}'", to_string(str));
    ApiChar* result = duplicate_string(str.c_str(), str.length());
    LOG_TRACE("Converted string duplicated at {}", (void*)result);
    return result;
}

CResultSetMetaData::ColumnMetaData::ColumnMetaData(const ColumnMetaData& other) {
    LOG_TRACE("Copying ColumnMetaData from {}", reinterpret_cast<uintptr_t>(&other));
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
    columnLabel = duplicate_string(other.columnLabel);
    columnName = duplicate_string(other.columnName);
    schemaName = duplicate_string(other.schemaName);
    tableName = duplicate_string(other.tableName);
    catalogName = duplicate_string(other.catalogName);
    columnTypeName = duplicate_string(other.columnTypeName);
    columnClassName = duplicate_string(other.columnClassName);
}

CResultSetMetaData::ColumnMetaData::~ColumnMetaData() {
    auto str_free = [&](const auto* str) {
        if (str) free(const_cast<void*>(static_cast<const void*>(str)));
    };

    str_free(columnLabel);
    str_free(columnName);
    str_free(schemaName);
    str_free(tableName);
    str_free(catalogName);
    str_free(columnTypeName);
    str_free(columnClassName);
}

CResultSetMetaData::CResultSetMetaData(const CResultSetMetaData& other) {
    LOG_TRACE("Copying CResultSetMetaData from {}", reinterpret_cast<uintptr_t>(&other));
    columnCount = other.columnCount;

    if (columnCount) {
        column = new const ColumnMetaData* [columnCount];
        for (int i = 0; i < columnCount; ++i) {
            column[i] = new ColumnMetaData(*other.column[i]);
        }
    }
}

CResultSetMetaData::CResultSetMetaData(const ResultSetMetaData& other) {
    LOG_TRACE("Constructing CResultSetMetaData from ResultSetMetaData");
    columnCount = other.getColumnCount();
    LOG_TRACE("Detected {} columns", columnCount);

    if (columnCount) {
        column = new const ColumnMetaData * [columnCount];
        for (int i = 0; i < columnCount; ++i) {
            LOG_TRACE("Processing column [{}]", i);
            auto* data = new ColumnMetaData();

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

CResultSetMetaData::~CResultSetMetaData() {
    LOG_TRACE("Destroying CResultSetMetaData: columnCount={}", columnCount);

    if (columnCount) {
        for (int i = 0; i < columnCount; ++i) {
            LOG_TRACE("Deleting ColumnMetaData [{}]", i);
            delete column[i];
        }

        delete[] column;
    }
    LOG_TRACE("CResultSetMetaData destroyed");
}