#include "core/result_set.hpp"

#ifdef _WIN32
// needs to be included above sql.h for windows
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <sqlext.h>

using namespace std;

ResultSet::ResultSet(const result& rhs)
        : result(rhs) {
}

ResultSet::ResultSet(result&& rhs) noexcept
        : result(std::move(rhs)) {
}

ResultSet::ResultSet(nanodbc::statement&& statement, long rowset_size)
        : result(std::move(statement), rowset_size) {
}

void ResultSet::set_alias_column_name(nanodbc::string const &alias_column_name, short column) {
    if (column >= 0 && column < columns()) {
        aliases.insert(alias_column_name, column);
    }
}

nanodbc::string ResultSet::map_column_name(nanodbc::string const &column_name, short column) const {
    if (aliases.has_value(column)) {
        return aliases.get_key(column);
    }
    return column_name;
}

bool ResultSet::is_string_or_binary(short column) const {
    auto datatype = column_c_datatype(column);
    return datatype == SQL_C_CHAR || datatype == SQL_C_BINARY;
}

bool ResultSet::is_string_or_binary(const nanodbc::string& column_name) const {
    auto datatype = column_c_datatype(column_name);
    return datatype == SQL_C_CHAR || datatype == SQL_C_BINARY;
}