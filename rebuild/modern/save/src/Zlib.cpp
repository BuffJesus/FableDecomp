#include "Zlib.hpp"

#include <zlib.h>

#include <array>
#include <limits>

namespace fable::save::detail {

std::expected<std::vector<std::byte>, ZlibError> inflate_exact(
    std::span<const std::byte> compressed, std::uint32_t expected_size)
{
    if (compressed.size() > std::numeric_limits<uInt>::max()) {
        return std::unexpected(ZlibError::too_large);
    }
    z_stream stream{};
    if (inflateInit(&stream) != Z_OK) {
        return std::unexpected(ZlibError::inflate_failed);
    }
    struct Cleanup {
        z_stream* state;
        ~Cleanup() { inflateEnd(state); }
    } cleanup{&stream};
    stream.next_in = reinterpret_cast<Bytef*>(const_cast<std::byte*>(compressed.data()));
    stream.avail_in = static_cast<uInt>(compressed.size());
    std::vector<std::byte> out;
    std::array<std::byte, 16384> chunk;
    int result = Z_OK;
    while (result == Z_OK) {
        stream.next_out = reinterpret_cast<Bytef*>(chunk.data());
        stream.avail_out = static_cast<uInt>(chunk.size());
        result = inflate(&stream, Z_NO_FLUSH);
        const auto produced = chunk.size() - stream.avail_out;
        if (produced > expected_size - out.size()) {
            return std::unexpected(ZlibError::size_mismatch);
        }
        out.insert(out.end(), chunk.begin(), chunk.begin() + static_cast<std::ptrdiff_t>(produced));
    }
    if (result != Z_STREAM_END || stream.avail_in != 0) {
        return std::unexpected(ZlibError::inflate_failed);
    }
    if (out.size() != expected_size) {
        return std::unexpected(ZlibError::size_mismatch);
    }
    return out;
}

std::expected<std::vector<std::byte>, ZlibError> deflate_retail(std::span<const std::byte> data, int level)
{
    if (data.size() > std::numeric_limits<uInt>::max()) {
        return std::unexpected(ZlibError::too_large);
    }
    z_stream stream{};
    if (deflateInit2(&stream, level, Z_DEFLATED, 15, 8, Z_DEFAULT_STRATEGY) != Z_OK) {
        return std::unexpected(ZlibError::deflate_failed);
    }
    std::vector<std::byte> out(deflateBound(&stream, static_cast<uLong>(data.size())));
    stream.next_in = reinterpret_cast<Bytef*>(const_cast<std::byte*>(data.data()));
    stream.avail_in = static_cast<uInt>(data.size());
    stream.next_out = reinterpret_cast<Bytef*>(out.data());
    stream.avail_out = static_cast<uInt>(out.size());
    const int result = deflate(&stream, Z_FINISH);
    out.resize(stream.total_out);
    deflateEnd(&stream);
    if (result != Z_STREAM_END) {
        return std::unexpected(ZlibError::deflate_failed);
    }
    return out;
}

} // namespace fable::save::detail
