#pragma once
#include <nanodbc/nanodbc.h>

class ChunkedBinaryStream {
private:
    static constexpr size_t DEFAULT_CHUNK_SIZE = 8192; // 8KB

    nanodbc::result* rs_;
    int column_index_;
    size_t position_;
    bool eof_;
    std::vector<uint8_t> buffer_;

public:
    explicit ChunkedBinaryStream(nanodbc::result* rs, int column_index);

    int read(uint8_t* output_buffer, size_t offset, size_t length);

private:
    bool read_next_chunk();
};