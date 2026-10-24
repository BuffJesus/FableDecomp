#pragma once

// Internal: the two zlib operations the save format needs. Retail drives zlib 1.2.1 through
// CCompressorZlib with a 15-bit window and memLevel 8; only the level differs by layer
// (9 for the container chunks, 1 for SAVED_ENTITIES cells).

#include <cstddef>
#include <cstdint>
#include <expected>
#include <span>
#include <vector>

namespace fable::save::detail {

enum class ZlibError {
    inflate_failed,
    size_mismatch,   // the stream inflates to a different size than its header field says
    deflate_failed,
    too_large,
};

[[nodiscard]] std::expected<std::vector<std::byte>, ZlibError> inflate_exact(
    std::span<const std::byte> compressed, std::uint32_t expected_size);

[[nodiscard]] std::expected<std::vector<std::byte>, ZlibError> deflate_retail(
    std::span<const std::byte> data, int level);

} // namespace fable::save::detail
