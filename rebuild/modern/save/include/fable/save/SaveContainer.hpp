#pragma once

#include <cstddef>
#include <cstdint>
#include <expected>
#include <span>
#include <vector>

namespace fable::save {

// Retail CCRC::Calc: reflected CRC-32 (poly 0xEDB88320, the zlib table), caller seed, no final
// inversion. Every on-disk hash in Fable uses seed 0 (save trailer, field tags, def tags).
[[nodiscard]] std::uint32_t ccrc(std::span<const std::byte> data, std::uint32_t seed = 0) noexcept;

enum class SaveContainerError {
    bad_magic,              // not "FableSave!" + 00 00
    truncated_header,       // shorter than the 0x1C-byte header
    data_length_out_of_range, // total_data_len + 4 > file size
    bad_signature,          // trailer != ccrc(file[0 : total_data_len])
    bad_slab_size,          // retail requires exactly 0x4B000 bytes
    nonzero_padding,        // bytes after the trailer must all be 0x00
    chunk_out_of_bounds,    // a chunk length runs past total_data_len
    chunk_layout_mismatch,  // chunk1 does not end exactly at total_data_len
    inflate_failed,
    inflated_size_mismatch, // a section's size differs from its header field
    deflate_failed,
    too_large,              // the rewritten data does not fit the slab
};

// The two persist-stream sections of a retail `.sav` (`FableSave!` container), decompressed.
//
// On disk (docs/formats/SAVE.md "Container and header layout"):
//   0x00 "FableSave!" 00 00
//   0x0C u32 total_data_len   (= trailer offset)
//   0x10 u32 header_size      (inflated HEADER section)
//   0x14 u32 body_size        (inflated main section)
//   0x18 u32 header_zlib_size
//   0x1C header zlib stream, u32 body_zlib_size, body zlib stream
//   total_data_len: u32 ccrc(file[0 : total_data_len]), then zeros to 0x4B000.
//
// All fields are explicit little-endian 32-bit values, so the layout is the same on every pointer
// width; nothing here depends on the retail 32-bit struct images.
struct SaveContainer {
    std::vector<std::byte> header_section;
    std::vector<std::byte> body_section;

    friend bool operator==(const SaveContainer&, const SaveContainer&) = default;
};

class SaveContainerCodec final {
public:
    static constexpr std::size_t slab_size = 0x4B000;
    static constexpr std::size_t header_size = 0x1C;

    // Accepts exactly what retail CUserProfileManager::VerifySignature (0x00409730) accepts, plus
    // the chunk framing CWorld::LoadGameStateInternal (0x004A21F0) then reads.
    [[nodiscard]] static std::expected<SaveContainer, SaveContainerError>
    decode(std::span<const std::byte> file);

    // Deflates each section the way retail CDataOutputStream::WriteZlibCompressed (0x009967E0)
    // does: zlib level 9, 15-bit window, memLevel 8, default strategy. With classic zlib (1.2.1
    // through 1.3.x; not zlib-ng) this reproduces retail-written saves byte for byte.
    [[nodiscard]] static std::expected<std::vector<std::byte>, SaveContainerError>
    encode(const SaveContainer& container);
};

} // namespace fable::save
