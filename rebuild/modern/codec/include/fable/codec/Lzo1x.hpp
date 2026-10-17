#pragma once

// LZO1X stream decoder (this tree's own implementation of the format, MIT). Used by save text
// blobs, texture mip 0 and the other bank payloads that retail compresses with lzo1x_999.

#include <cstddef>
#include <expected>
#include <span>
#include <vector>

namespace fable::codec {

enum class Lzo1xError {
    truncated,       // the stream ends inside a token, a literal run or a match
    bad_distance,    // a match reaches back before the start of the output
    size_mismatch,   // the stream decodes to a different size than expected
    trailing_bytes,  // bytes follow the end-of-stream marker
};

[[nodiscard]] std::expected<std::vector<std::byte>, Lzo1xError> lzo1x_decompress(
    std::span<const std::byte> stream, std::size_t expected_size);

} // namespace fable::codec
