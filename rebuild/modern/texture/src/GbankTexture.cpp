#include <fable/texture/GbankTexture.hpp>

#include <fable/codec/Lzo1x.hpp>

#include <algorithm>
#include <limits>
#include <string_view>

namespace fable::texture {
namespace {

constexpr std::size_t info_size = 34;
constexpr std::size_t raw_tail = 3;

std::uint32_t u16_at(std::span<const std::byte> b, std::size_t at)
{
    return std::to_integer<std::uint32_t>(b[at]) | std::to_integer<std::uint32_t>(b[at + 1]) << 8;
}

std::uint32_t u32_at(std::span<const std::byte> b, std::size_t at)
{
    return u16_at(b, at) | u16_at(b, at + 2) << 16;
}

// RGB565 to 8-bit channels.
std::array<std::uint8_t, 3> rgb565(std::uint32_t c)
{
    const auto r = (c >> 11) & 31U;
    const auto g = (c >> 5) & 63U;
    const auto b = c & 31U;
    return {static_cast<std::uint8_t>((r << 3) | (r >> 2)), static_cast<std::uint8_t>((g << 2) | (g >> 4)),
            static_cast<std::uint8_t>((b << 3) | (b >> 2))};
}

// One DXT colour block (8 bytes) into 16 RGBA texels; DXT1 uses the 3-colour + transparent mode
// when c0 <= c1, DXT3 always uses 4 colours (its alpha comes from the separate block).
void decode_colour_block(std::span<const std::byte> block, bool dxt1, std::array<std::array<std::uint8_t, 4>, 16>& out)
{
    const std::uint32_t c0 = u16_at(block, 0);
    const std::uint32_t c1 = u16_at(block, 2);
    const auto a = rgb565(c0);
    const auto b = rgb565(c1);
    std::array<std::array<std::uint8_t, 4>, 4> palette{};
    palette[0] = {a[0], a[1], a[2], 255};
    palette[1] = {b[0], b[1], b[2], 255};
    for (int i = 0; i < 3; ++i) {
        const auto ai = static_cast<std::uint32_t>(a[static_cast<std::size_t>(i)]);
        const auto bi = static_cast<std::uint32_t>(b[static_cast<std::size_t>(i)]);
        if (!dxt1 || c0 > c1) {
            palette[2][static_cast<std::size_t>(i)] = static_cast<std::uint8_t>((2 * ai + bi) / 3);
            palette[3][static_cast<std::size_t>(i)] = static_cast<std::uint8_t>((ai + 2 * bi) / 3);
        } else {
            palette[2][static_cast<std::size_t>(i)] = static_cast<std::uint8_t>((ai + bi) / 2);
            palette[3][static_cast<std::size_t>(i)] = 0;
        }
    }
    palette[2][3] = 255;
    palette[3][3] = (!dxt1 || c0 > c1) ? 255 : 0;
    const std::uint32_t bits = u32_at(block, 4);
    for (std::size_t t = 0; t < 16; ++t) {
        out[t] = palette[(bits >> (2 * t)) & 3U];
    }
}

void put_crc_chunk(std::vector<std::byte>& out, std::string_view type, std::span<const std::byte> data)
{
    auto be32 = [&](std::uint32_t v) {
        for (int s = 24; s >= 0; s -= 8) {
            out.push_back(static_cast<std::byte>((v >> s) & 0xFFU));
        }
    };
    be32(static_cast<std::uint32_t>(data.size()));
    const std::size_t crc_start = out.size();
    for (const char c : type) {
        out.push_back(static_cast<std::byte>(c));
    }
    out.insert(out.end(), data.begin(), data.end());
    std::uint32_t crc = 0xFFFFFFFFU;
    for (std::size_t i = crc_start; i < out.size(); ++i) {
        crc ^= std::to_integer<std::uint32_t>(out[i]);
        for (int k = 0; k < 8; ++k) {
            crc = (crc >> 1) ^ (0xEDB88320U & (0U - (crc & 1U)));
        }
    }
    be32(crc ^ 0xFFFFFFFFU);
}

} // namespace

std::expected<TextureInfo, TextureError> TextureInfo::parse(std::span<const std::byte> info)
{
    if (info.size() != info_size) {
        return std::unexpected(TextureError::bad_info);
    }
    TextureInfo t;
    t.alloc_width = static_cast<std::uint16_t>(u16_at(info, 0));
    t.alloc_height = static_cast<std::uint16_t>(u16_at(info, 2));
    t.depth = static_cast<std::uint16_t>(u16_at(info, 4));
    t.frame_width = static_cast<std::uint16_t>(u16_at(info, 6));
    t.frame_height = static_cast<std::uint16_t>(u16_at(info, 8));
    t.frame_count = static_cast<std::uint16_t>(u16_at(info, 10));
    t.pixel_format = u32_at(info, 12);
    t.transparency = std::to_integer<std::uint8_t>(info[16]);
    t.mip_levels = std::to_integer<std::uint8_t>(info[17]);
    t.flags = std::to_integer<std::uint8_t>(info[18]);
    t.frame_data_size = u32_at(info, 20);
    t.mip_size0 = u32_at(info, 24);
    for (std::size_t i = 0; i < t.format_class.size(); ++i) {
        t.format_class[i] = std::to_integer<std::uint8_t>(info[28 + i]);
    }
    if (t.frame_width > t.alloc_width || t.frame_height > t.alloc_height) {
        return std::unexpected(TextureError::bad_info);
    }
    return t;
}

std::expected<std::vector<std::byte>, TextureError> decode_mip0(const TextureInfo& info, std::span<const std::byte> payload)
{
    const std::size_t raw_size = info.frame_data_size;
    if (info.mip_size0 == 0) {   // all raw
        if (payload.size() < raw_size) {
            return std::unexpected(TextureError::bad_payload);
        }
        return std::vector<std::byte>(payload.begin(), payload.begin() + static_cast<std::ptrdiff_t>(raw_size));
    }
    if (raw_size <= raw_tail || info.mip_size0 > payload.size()) {
        return std::unexpected(TextureError::bad_payload);
    }
    const auto region = payload.first(info.mip_size0);
    const std::size_t body = raw_size - raw_tail;
    std::vector<std::byte> out;
    out.reserve(raw_size);
    std::size_t pos = 0;
    while (out.size() < body) {
        if (pos + 2 > region.size()) {
            return std::unexpected(TextureError::bad_payload);
        }
        std::size_t clen = u16_at(region, pos);
        pos += 2;
        if (clen == 0xFFFF) {
            if (pos + 4 > region.size()) {
                return std::unexpected(TextureError::bad_payload);
            }
            clen = u32_at(region, pos);
            pos += 4;
        }
        if (clen == 0) {   // stored: the rest of the body raw
            const std::size_t n = body - out.size();
            if (pos + n > region.size()) {
                return std::unexpected(TextureError::bad_payload);
            }
            out.insert(out.end(), region.begin() + static_cast<std::ptrdiff_t>(pos),
                       region.begin() + static_cast<std::ptrdiff_t>(pos + n));
            pos += n;
            break;
        }
        if (pos + clen > region.size()) {
            return std::unexpected(TextureError::bad_payload);
        }
        // Retail writes one chunk for the whole body; a chunk decodes to what is left.
        auto chunk = codec::lzo1x_decompress(region.subspan(pos, clen), body - out.size());
        if (!chunk) {
            return std::unexpected(TextureError::bad_payload);
        }
        out.insert(out.end(), chunk->begin(), chunk->end());
        pos += clen;
    }
    if (pos + raw_tail != region.size()) {
        return std::unexpected(TextureError::bad_payload);
    }
    out.insert(out.end(), region.begin() + static_cast<std::ptrdiff_t>(pos), region.end());
    return out;
}

std::expected<Image, TextureError> to_rgba(const TextureInfo& info, std::span<const std::byte> surface)
{
    const std::size_t aw = info.alloc_width;
    const std::size_t ah = info.alloc_height;
    if (info.frame_width > aw || info.frame_height > ah
        || aw * ah > std::numeric_limits<std::size_t>::max() / 4) {
        return std::unexpected(TextureError::bad_info);
    }
    Image img;
    img.width = info.frame_width;
    img.height = info.frame_height;
    auto put = [&](std::size_t x, std::size_t y, const std::array<std::uint8_t, 4>& px) {
        if (x < img.width && y < img.height) {
            std::copy(px.begin(), px.end(), img.rgba.begin() + static_cast<std::ptrdiff_t>((y * img.width + x) * 4));
        }
    };
    switch (info.pixel_format) {
    case a8r8g8b8: {
        if (surface.size() < aw * ah * 4) {
            return std::unexpected(TextureError::bad_payload);
        }
        img.rgba.assign(static_cast<std::size_t>(img.width) * img.height * 4, 0);
        for (std::size_t y = 0; y < ah; ++y) {
            for (std::size_t x = 0; x < aw; ++x) {
                const std::size_t i = (y * aw + x) * 4;   // stored B, G, R, A
                put(x, y, {std::to_integer<std::uint8_t>(surface[i + 2]), std::to_integer<std::uint8_t>(surface[i + 1]),
                           std::to_integer<std::uint8_t>(surface[i]), std::to_integer<std::uint8_t>(surface[i + 3])});
            }
        }
        return img;
    }
    case dxt1:
    case dxt3: {
        const bool dxt1_format = info.pixel_format == dxt1;
        const std::size_t block_bytes = dxt1_format ? 8 : 16;
        const std::size_t bw = std::max<std::size_t>(1, (aw + 3) / 4);
        const std::size_t bh = std::max<std::size_t>(1, (ah + 3) / 4);
        if (surface.size() < bw * bh * block_bytes) {
            return std::unexpected(TextureError::bad_payload);
        }
        img.rgba.assign(static_cast<std::size_t>(img.width) * img.height * 4, 0);
        std::array<std::array<std::uint8_t, 4>, 16> texels{};
        for (std::size_t by = 0; by < bh; ++by) {
            for (std::size_t bx = 0; bx < bw; ++bx) {
                const auto block = surface.subspan((by * bw + bx) * block_bytes, block_bytes);
                decode_colour_block(dxt1_format ? block : block.subspan(8), dxt1_format, texels);
                if (!dxt1_format) {   // DXT3: 16 explicit 4-bit alphas first
                    for (std::size_t t = 0; t < 16; ++t) {
                        const auto nibble = (std::to_integer<std::uint32_t>(block[t / 2]) >> (4 * (t % 2))) & 15U;
                        texels[t][3] = static_cast<std::uint8_t>(nibble * 17);
                    }
                }
                for (std::size_t t = 0; t < 16; ++t) {
                    put(bx * 4 + t % 4, by * 4 + t / 4, texels[t]);
                }
            }
        }
        return img;
    }
    default:
        return std::unexpected(TextureError::unsupported_format);
    }
}

std::vector<std::byte> encode_png(const Image& image)
{
    std::vector<std::byte> out;
    constexpr std::array<unsigned char, 8> signature{0x89, 'P', 'N', 'G', 0x0D, 0x0A, 0x1A, 0x0A};
    for (const unsigned char c : signature) {
        out.push_back(static_cast<std::byte>(c));
    }
    std::vector<std::byte> ihdr;
    auto be32 = [](std::vector<std::byte>& v, std::uint32_t x) {
        for (int s = 24; s >= 0; s -= 8) {
            v.push_back(static_cast<std::byte>((x >> s) & 0xFFU));
        }
    };
    be32(ihdr, image.width);
    be32(ihdr, image.height);
    constexpr std::array<unsigned char, 5> header_tail{8, 6, 0, 0, 0};   // 8-bit, RGBA, deflate, adaptive filter, no interlace
    for (const unsigned char c : header_tail) {
        ihdr.push_back(static_cast<std::byte>(c));
    }
    put_crc_chunk(out, "IHDR", ihdr);

    // Raw scanlines (filter byte 0) wrapped in a zlib stream of stored deflate blocks.
    std::vector<std::byte> raw;
    const std::size_t stride = static_cast<std::size_t>(image.width) * 4;
    for (std::size_t y = 0; y < image.height; ++y) {
        raw.push_back(std::byte{0});
        for (std::size_t i = 0; i < stride; ++i) {
            raw.push_back(static_cast<std::byte>(image.rgba[y * stride + i]));
        }
    }
    std::vector<std::byte> z{std::byte{0x78}, std::byte{0x01}};
    for (std::size_t pos = 0; pos < raw.size() || pos == 0;) {
        const std::size_t n = std::min<std::size_t>(65535, raw.size() - pos);
        const bool last = pos + n == raw.size();
        z.push_back(std::byte{static_cast<unsigned char>(last ? 1 : 0)});
        z.push_back(static_cast<std::byte>(n & 0xFF));
        z.push_back(static_cast<std::byte>(n >> 8));
        z.push_back(static_cast<std::byte>(~n & 0xFF));
        z.push_back(static_cast<std::byte>((~n >> 8) & 0xFF));
        z.insert(z.end(), raw.begin() + static_cast<std::ptrdiff_t>(pos), raw.begin() + static_cast<std::ptrdiff_t>(pos + n));
        pos += n;
        if (last) {
            break;
        }
    }
    std::uint32_t a = 1;
    std::uint32_t b = 0;
    for (const std::byte v : raw) {
        a = (a + std::to_integer<std::uint32_t>(v)) % 65521U;
        b = (b + a) % 65521U;
    }
    be32(z, (b << 16) | a);
    put_crc_chunk(out, "IDAT", z);
    put_crc_chunk(out, "IEND", {});
    return out;
}

} // namespace fable::texture
