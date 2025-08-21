#pragma once
#include <wtypes.h>
#include <sqlext.h>
#include <nanodbc/nanodbc.h>

class ResultSetMetaData {
private:
    nanodbc::result result_;
    SQLHSTMT hStmt_;

public:
    ResultSetMetaData(const nanodbc::result& result);    

    int getColumnCount() const;

    bool isAutoIncrement(int column) const;

    bool isCaseSensitive(int column) const;

    bool isSearchable(int column) const;

    bool isCurrency(int column) const;

    int isNullable(int column) const;

    bool isSigned(int column) const;

    int getColumnDisplaySize(int column) const;

    std::wstring getColumnLabel(int column) const;

    std::wstring getColumnName(int column) const;

    std::wstring getSchemaName(int column) const;

    int getPrecision(int column) const;

    int getScale(int column) const;

    std::wstring getTableName(int column) const;

    std::wstring getCatalogName(int column) const;

    int getColumnType(int column) const;

    std::wstring getColumnTypeName(int column) const;

    bool isReadOnly(int column) const;
    
    bool isWritable(int column) const;    

    bool isDefinitelyWritable(int column) const;

    std::wstring getColumnClassName(int column) const;
};