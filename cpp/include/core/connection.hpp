#pragma once

#include <nanodbc/nanodbc.h>
#include "core/isolation_level.hpp"

class Connection : public nanodbc::connection {
    std::unique_ptr<nanodbc::transaction> transaction_;

public:
    using nanodbc::connection::connection; // Inherit base constructors

    /// \brief Sets the transaction isolation level using ODBC SQLSetConnectAttr.
    /// Must be called before starting a transaction.
    /// \param level The desired isolation level.
    /// \throws std::runtime_error if connection is inactive or ODBC call fails.
    void set_isolation_level(IsolationLevel level);

    /// \brief Retrieves the current transaction isolation level using ODBC SQLGetConnectAttr.
    /// \return The current isolation level.
    /// \throws std::runtime_error if connection is inactive or ODBC call fails or value is unknown.
    IsolationLevel get_isolation_level() const;

    /// \brief Enables or disables auto-commit mode
    /// \param autoCommit - true to enable auto-commit, false to start a transaction
    /// \throws std::runtime_error if connection is not active
    void set_auto_commit(bool auto_commit);

    /// \brief Commits the current transaction
    /// \throws std::runtime_error if no active transaction or connection is not active
    void commit();

    /// \brief Rolls back the current transaction
    /// \throws std::runtime_error if no active transaction or connection is not active
    void rollback();

    /// \brief Returns current auto-commit state
    /// \return true if auto-commit is enabled, false if inside a transaction
    bool get_auto_commit() const;

    ~Connection() noexcept = default;
};