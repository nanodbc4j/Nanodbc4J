#include "core/connection.hpp"

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