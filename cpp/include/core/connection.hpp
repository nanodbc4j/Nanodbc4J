#pragma once

#include <nanodbc/nanodbc.h>

class Connection : public nanodbc::connection {
    std::unique_ptr<nanodbc::transaction> transaction_;

public:
    using nanodbc::connection::connection; // Inherit base constructors

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