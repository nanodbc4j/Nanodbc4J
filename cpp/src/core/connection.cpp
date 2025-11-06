#include "core/connection.hpp"

#ifdef _WIN32
// needs to be included above sql.h for windows
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <sqlext.h>
#include "core/nanodbc_defs.h"

void Connection::set_catalog(const nanodbc::string& catalog) {
    if (!connected()) {
        throw std::runtime_error("Cannot set isolation level: connection is not active");
    }

    if (catalog.empty()) {
        throw std::runtime_error("Catalog name cannot be empty.");
    }

    SQLRETURN ret = ::SQLSetConnectAttr(
        this->native_dbc_handle(),
        SQL_ATTR_CURRENT_CATALOG,
        (NANODBC_SQLCHAR*) catalog.c_str(),
        SQL_NTS
    );

    if (!SQL_SUCCEEDED(ret)) {
        throw std::runtime_error("ODBC error in SQLSetConnectAttr(SQL_ATTR_CURRENT_CATALOG) - Driver may not support changing catalog at runtime.");
    }
}

void Connection::set_isolation_level(IsolationLevel level) {
    if (!connected()) {
        throw std::runtime_error("Cannot set isolation level: connection is not active");
    }

    SQLUINTEGER odbc_level = level.to_odbc();
    SQLRETURN rc = SQLSetConnectAttr(
        native_dbc_handle(),
        SQL_ATTR_TXN_ISOLATION,
        (SQLPOINTER)(std::intptr_t)odbc_level,
        0
    );

    if (!SQL_SUCCEEDED(rc)) {
        throw std::runtime_error("ODBC error in SQLSetConnectAttr(SQL_ATTR_TXN_ISOLATION)");
    }
}

IsolationLevel Connection::get_isolation_level() const {
    if (!connected()) {
        throw std::runtime_error("Cannot get isolation level: connection is not active");
    }

    SQLUINTEGER current_level = 0;
    SQLRETURN rc = SQLGetConnectAttr(
        native_dbc_handle(),
        SQL_ATTR_TXN_ISOLATION,
        &current_level,
        0,
        nullptr
    );

    if (!SQL_SUCCEEDED(rc)) {
        throw std::runtime_error("ODBC error in SQLGetConnectAttr(SQL_ATTR_TXN_ISOLATION)");
    }

    return IsolationLevel::from_odbc(current_level);
}

void Connection::set_auto_commit(bool auto_commit) {
    if (!this->connected()) {
        throw std::runtime_error("Cannot set auto-commit: connection is not active");
    }

    if (auto_commit) {
        // If enabling auto-commit, finalize current transaction (if any)
        if (transaction_) {
            transaction_->commit(); // Commit pending changes
            transaction_.reset();   // Clear transaction — auto-commit now active
        }
        // transaction_ == nullptr → auto-commit is ON
    } else {
        // If disabling auto-commit, start transaction (if not already started)
        if (!transaction_) {
            transaction_ = std::make_unique<nanodbc::transaction>(*this);
        }
        // transaction_ != nullptr → auto-commit is OFF
    }
}

void Connection::commit() {
    if (!this->connected()) {
        throw std::runtime_error("Cannot commit: connection is not active");
    } if (!transaction_) {
        throw std::runtime_error("Cannot commit: auto-commit is enabled or no active transaction");
    }

    transaction_->commit();
    transaction_.reset(); // Transaction complete → auto-commit implicitly restored
}

void Connection::rollback() {
    if (!this->connected()) {
        throw std::runtime_error("Cannot rollback: connection is not active");
    } if (!transaction_) {
        throw std::runtime_error("Cannot rollback: auto-commit is enabled or no active transaction");
    }

    transaction_->rollback();
    transaction_.reset(); // Transaction complete → auto-commit implicitly restored
}

bool Connection::get_auto_commit() const {
    return transaction_ == nullptr;
}