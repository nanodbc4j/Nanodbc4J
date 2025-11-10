#pragma once
#include <nanodbc/nanodbc.h>

class ResultSetMetaData {
    const nanodbc::result& result_;

public:
    explicit ResultSetMetaData(const nanodbc::result& result);

    int getColumnCount() const;

    bool isAutoIncrement(int column) const;

    bool isCaseSensitive(int column) const;

    bool isSearchable(int column) const;

    bool isCurrency(int column) const;

    int isNullable(int column) const;

    bool isSigned(int column) const;

    int getColumnDisplaySize(int column) const;

    nanodbc::string getColumnLabel(int column) const;

    nanodbc::string getColumnName(int column) const;

    nanodbc::string getSchemaName(int column) const;

    int getPrecision(int column) const;

    int getScale(int column) const;

    nanodbc::string getTableName(int column) const;

    nanodbc::string getCatalogName(int column) const;

    int getColumnType(int column) const;

    nanodbc::string getColumnTypeName(int column) const;

    bool isReadOnly(int column) const;
    
    bool isWritable(int column) const;    

    bool isDefinitelyWritable(int column) const;

    nanodbc::string getColumnClassName(int column) const;
};