#pragma once
#include <nanodbc/nanodbc.h>

class ResultSet : public nanodbc::result {
public:
    /// \brief Empty result set.
    ResultSet();

    /// \brief Copy constructor.
    explicit ResultSet(const result& rhs);

    /// \brief Move constructor.
    explicit ResultSet(result&& rhs) noexcept;

    ResultSet(nanodbc::statement&& statement, long rowset_size);

};