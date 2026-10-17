#pragma once

// Internal: the LZO1X operations the QUESTS and FACTIONS text blobs need.
//
// The decoder is this module's own implementation of the LZO1X stream format (MIT, like the
// rest of the tree). The compressor is upstream LZO's lzo1x_999_compress, which retail calls
// through 0xC08130 (level-8 parameters, 0x70000 bytes of work memory). LZO is GPL-2.0-or-later,
// so it is linked only when the build defines FABLE_SAVE_HAVE_LZO (CMake FABLE_SAVE_WITH_LZO).

#include <cstddef>
#include <expected>
#include <optional>
#include <span>
#include <vector>

namespace fable::save::detail {

enum class LzoError {
    truncated,       // the stream ends inside a token, a literal run or a match
    bad_distance,    // a match reaches back before the start of the output
    size_mismatch,   // the stream decodes to a different size than expected
    trailing_bytes,  // bytes follow the end-of-stream marker
};

[[nodiscard]] std::expected<std::vector<std::byte>, LzoError> lzo1x_decompress(
    std::span<const std::byte> stream, std::size_t expected_size);

[[nodiscard]] bool lzo_compressor_available() noexcept;

// lzo1x_999_compress(data); nullopt when the build has no LZO or compression fails.
[[nodiscard]] std::optional<std::vector<std::byte>> lzo1x_999_compress(std::span<const std::byte> data);

} // namespace fable::save::detail
