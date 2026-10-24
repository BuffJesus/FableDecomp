#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <expected>
#include <span>
#include <vector>

namespace fable::texture {

enum class TextureError {
    bad_info,             // the TOC info is not the 34-byte GBANK descriptor
    unsupported_format,   // a pixel format other than A8R8G8B8, DXT1 or DXT3
    bad_payload,          // mip 0 does not decode to FrameDataSize bytes
};

// The 34-byte GBANK texture descriptor stored as the TOC entry's info (TEXTURE.md §2).
struct TextureInfo {
    std::uint16_t alloc_width = 0;    // the power-of-two surface
    std::uint16_t alloc_height = 0;
    std::uint16_t depth = 0;
    std::uint16_t frame_width = 0;    // the authored image inside the surface
    std::uint16_t frame_height = 0;
    std::uint16_t frame_count = 0;
    std::uint32_t pixel_format = 0;   // 0x01 A8R8G8B8, 0x1F DXT1, 0x20 DXT3
    std::uint8_t transparency = 0;
    std::uint8_t mip_levels = 0;
    std::uint8_t flags = 0;
    std::uint32_t frame_data_size = 0;   // uncompressed mip-0 bytes
    std::uint32_t mip_size0 = 0;         // on-disk mip-0 region; 0 = the payload is all raw
    std::array<std::uint8_t, 6> format_class{};

    [[nodiscard]] static std::expected<TextureInfo, TextureError> parse(std::span<const std::byte> info);
};

enum PixelFormat : std::uint32_t {
    a8r8g8b8 = 0x01,
    dxt1 = 0x1F,
    dxt3 = 0x20,
};

// Mip 0 of the first frame as the raw surface bytes (alloc_width × alloc_height, the stored
// pixel format): chunks of LZO1X (u16 clen, 0xFFFF then u32; clen 0 = stored) covering all but
// the last 3 bytes, then those 3 bytes raw (TEXTURE.md §4).
[[nodiscard]] std::expected<std::vector<std::byte>, TextureError> decode_mip0(
    const TextureInfo& info, std::span<const std::byte> payload);

// RGBA8, row-major, top row first.
struct Image {
    std::uint32_t width = 0;
    std::uint32_t height = 0;
    std::vector<std::uint8_t> rgba;
};

// Converts a mip-0 surface to RGBA8, cropped to the authored frame size.
[[nodiscard]] std::expected<Image, TextureError> to_rgba(const TextureInfo& info, std::span<const std::byte> surface);

// Writes `image` as a PNG (8-bit RGBA, uncompressed deflate blocks, so no zlib dependency).
[[nodiscard]] std::vector<std::byte> encode_png(const Image& image);

} // namespace fable::texture
