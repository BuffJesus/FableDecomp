#include <fable/texture/GbankTexture.hpp>

#include <array>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string_view>
#include <vector>

namespace {

using fable::texture::TextureInfo;

int failures = 0;

void expect(const bool condition, const std::string_view message)
{
    if (!condition) {
        std::cerr << "FAIL: " << message << '\n';
        ++failures;
    }
}

std::vector<std::byte> info_bytes(std::uint16_t w, std::uint16_t h, std::uint32_t format, std::uint32_t data_size,
                                  std::uint32_t mip0)
{
    std::vector<std::byte> b(34, std::byte{0});
    auto put16 = [&](std::size_t at, std::uint32_t v) {
        b[at] = static_cast<std::byte>(v & 0xFF);
        b[at + 1] = static_cast<std::byte>((v >> 8) & 0xFF);
    };
    auto put32 = [&](std::size_t at, std::uint32_t v) {
        put16(at, v & 0xFFFF);
        put16(at + 2, v >> 16);
    };
    put16(0, w);
    put16(2, h);
    put16(6, w);
    put16(8, h);
    put16(10, 1);
    put32(12, format);
    b[17] = std::byte{1};
    put32(20, data_size);
    put32(24, mip0);
    return b;
}

void info_parses()
{
    const auto info = TextureInfo::parse(info_bytes(256, 128, 0x01, 256 * 128 * 4, 0));
    expect(info && info->alloc_width == 256 && info->frame_height == 128 && info->pixel_format == 0x01, "34-byte info parses");
    expect(!TextureInfo::parse(std::vector<std::byte>(33)), "a short info is rejected");
}

void dxt1_block_decodes()
{
    // One 4x4 DXT1 block: c0 = pure red (0xF800), c1 = pure blue (0x001F), all texels index 0
    // except texel 15 = index 1.
    const auto info = TextureInfo::parse(info_bytes(4, 4, 0x1F, 8, 0));
    std::vector<std::byte> block{std::byte{0x00}, std::byte{0xF8}, std::byte{0x1F}, std::byte{0x00},
                                 std::byte{0x00}, std::byte{0x00}, std::byte{0x00}, std::byte{0x40}};
    const auto image = fable::texture::to_rgba(*info, block);
    expect(image && image->rgba[0] == 255 && image->rgba[1] == 0 && image->rgba[2] == 0 && image->rgba[3] == 255,
           "DXT1 texel 0 is red");
    expect(image && image->rgba[15 * 4 + 2] == 255 && image->rgba[15 * 4] == 0, "DXT1 texel 15 is blue");
}

void argb_is_bgra_on_disk()
{
    const auto info = TextureInfo::parse(info_bytes(1, 1, 0x01, 4, 0));
    const std::vector<std::byte> px{std::byte{10}, std::byte{20}, std::byte{30}, std::byte{40}};   // B G R A
    const auto image = fable::texture::to_rgba(*info, px);
    expect(image && image->rgba == std::vector<std::uint8_t>{30, 20, 10, 40}, "A8R8G8B8 is stored as BGRA");
}

void partial_dxt_blocks_cover_the_edges()
{
    for (const auto format : {fable::texture::dxt1, fable::texture::dxt3}) {
        const bool dxt1 = format == fable::texture::dxt1;
        const std::size_t block_bytes = dxt1 ? 8 : 16;
        std::vector<std::byte> surface(4 * block_bytes, std::byte{0});
        const std::array<std::uint16_t, 4> colors{0xf800, 0x07e0, 0x001f, 0xffff};
        for (std::size_t i = 0; i < colors.size(); ++i) {
            const std::size_t color_at = i * block_bytes + (dxt1 ? 0 : 8);
            surface[color_at] = static_cast<std::byte>(colors[i] & 0xff);
            surface[color_at + 1] = static_cast<std::byte>(colors[i] >> 8);
            if (!dxt1) {
                for (std::size_t a = 0; a < 8; ++a) {
                    surface[i * block_bytes + a] = std::byte{0xff};
                }
            }
        }
        const auto info = TextureInfo::parse(info_bytes(6, 5, format, static_cast<std::uint32_t>(surface.size()), 0));
        const auto image = fable::texture::to_rgba(*info, surface);
        expect(image && image->rgba[(0 * 6 + 5) * 4 + 1] == 255, "partial right DXT block is green");
        expect(image && image->rgba[(4 * 6 + 0) * 4 + 2] == 255, "partial bottom DXT block is blue");
        expect(image && image->rgba[(4 * 6 + 5) * 4] == 255 && image->rgba[(4 * 6 + 5) * 4 + 3] == 255,
               "partial corner DXT block is opaque white");
        const auto truncated = fable::texture::to_rgba(*info, std::span(surface).first(surface.size() - 1));
        expect(!truncated && truncated.error() == fable::texture::TextureError::bad_payload,
               "truncated partial DXT block is rejected");
    }
}

void mip0_chunks_decode()
{
    // 8 raw bytes: an LZO1X chunk for the first 5 ("abcde": initial run of 5, end of stream),
    // then the last 3 bytes raw.
    std::vector<std::byte> payload{std::byte{9}, std::byte{0}, std::byte{0x16}, std::byte{'a'}, std::byte{'b'},
                                   std::byte{'c'},  std::byte{'d'}, std::byte{'e'}, std::byte{0x11}, std::byte{0},
                                   std::byte{0},    std::byte{'f'}, std::byte{'g'}, std::byte{'h'}};
    const auto info = TextureInfo::parse(info_bytes(1, 2, 0x01, 8, static_cast<std::uint32_t>(payload.size())));
    const auto surface = fable::texture::decode_mip0(*info, payload);
    const std::vector<std::byte> want{std::byte{'a'}, std::byte{'b'}, std::byte{'c'}, std::byte{'d'},
                                      std::byte{'e'}, std::byte{'f'}, std::byte{'g'}, std::byte{'h'}};
    expect(surface && *surface == want, "mip 0 = LZO1X chunk + 3 raw tail bytes");
}

void png_has_signature()
{
    fable::texture::Image img{2, 1, {255, 0, 0, 255, 0, 255, 0, 255}};
    const auto png = fable::texture::encode_png(img);
    expect(png.size() > 33 && png[1] == std::byte{'P'} && png[12] == std::byte{'I'}, "PNG signature and IHDR");
}

} // namespace

int main()
{
    info_parses();
    dxt1_block_decodes();
    argb_is_bgra_on_disk();
    partial_dxt_blocks_cover_the_edges();
    mip0_chunks_decode();
    png_has_signature();
    if (failures == 0) {
        std::cout << "fable_texture: all tests passed\n";
    }
    return failures == 0 ? 0 : 1;
}
