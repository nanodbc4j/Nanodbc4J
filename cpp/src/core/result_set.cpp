#include "core/result_set.hpp"

ResultSet::ResultSet(){
}

ResultSet::ResultSet(const result &rhs)
    : result(rhs){
}

ResultSet::ResultSet(result &&rhs) noexcept
    : result(std::move(rhs)){
}

ResultSet::ResultSet(nanodbc::statement&& statement, long rowset_size)
    : result(std::move(statement), rowset_size) {
};