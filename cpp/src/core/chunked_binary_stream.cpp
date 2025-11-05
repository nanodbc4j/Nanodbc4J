#include "core/chunked_binary_stream.hpp"

#ifdef _WIN32
// needs to be included above sql.h for windows
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <sqlext.h>
#include <sql.h>
#include "core/nanodbc_defs.h"

ChunkedBinaryStream::ChunkedBinaryStream(nanodbc::result* rs, int column_index)
    : rs_(rs)
    , column_index_(column_index)
    , position_(0)
    , eof_(false) {
}

int ChunkedBinaryStream::read(uint8_t* output_buffer, size_t offset, size_t length) {
    if (eof_) return -1;

    size_t total_read = 0;

    while (total_read < length && !eof_) {
        if (position_ >= buffer_.size() && !read_next_chunk()) {
            if (!read_next_chunk()) {
                eof_ = true;
                break;
            }
        }

        size_t available = buffer_.size() - position_;
        size_t to_copy = std::min(length - total_read, available);

        if (to_copy > 0) {
            std::memcpy(output_buffer + offset + total_read,
                buffer_.data() + position_,
                to_copy);

            position_ += to_copy;
            total_read += to_copy;
        }
    }

    return total_read > 0 ? static_cast<int>(total_read) : -1;
}

bool ChunkedBinaryStream::read_next_chunk() {
    SQLLEN indicator = 0;
    buffer_.resize(DEFAULT_CHUNK_SIZE);

    SQLRETURN rc = SQLGetData(
        rs_->native_statement_handle(),
        static_cast<SQLUSMALLINT>(column_index_ + 1), // ODBC uses 1-based indexing
        SQL_C_BINARY,
        buffer_.data(),
        buffer_.size(),
        &indicator
    );

    if (SQL_SUCCEEDED(rc)) {
        if (indicator == SQL_NULL_DATA) {
            buffer_.clear();
            return false;
        } else if (indicator == SQL_NO_TOTAL || indicator > buffer_.size()) {
            // Data didn't fit completely
            buffer_.resize(buffer_.size());
        } else {
            // All data received
            buffer_.resize(indicator);
            eof_ = true;
        }
        position_ = 0;
        return true;
    } else if (rc == SQL_NO_DATA) {
        buffer_.clear();
        return false;
    } else {
        throw nanodbc::database_error(rs_->native_statement_handle(), SQL_HANDLE_STMT);
    }
}