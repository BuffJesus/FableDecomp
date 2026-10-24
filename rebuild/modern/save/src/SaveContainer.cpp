#include <fable/save/SaveContainer.hpp>

#include "Zlib.hpp"

#include <algorithm>
#include <array>
#include <limits>

namespace fable::save {
namespace {

constexpr std::array<std::byte, 12> magic{
    std::byte{'F'}, std::byte{'a'}, std::byte{'b'}, std::byte{'l'}, std::byte{'e'}, std::byte{'S'},
    std::byte{'a'}, std::byte{'v'}, std::byte{'e'}, std::byte{'!'}, std::byte{0}, std::byte{0},
};

constexpr std::array<std::uint32_t, 256> make_crc_table() noexcept
{
    std::array<std::uint32_t, 256> table{};
    for (std::uint32_t n = 0; n < table.size(); ++n) {
        std::uint32_t c = n;
        for (int bit = 0; bit < 8; ++bit) {
            c = (c & 1U) != 0 ? (c >> 1) ^ 0xEDB88320U : c >> 1;
        }
        table[n] = c;
    }
    return table;
}

constexpr auto crc_table = make_crc_table();

std::uint32_t read_u32(std::span<const std::byte> bytes, std::size_t offset) noexcept
{
    return static_cast<std::uint32_t>(bytes[offset])
        | static_cast<std::uint32_t>(bytes[offset + 1]) << 8
        | static_cast<std::uint32_t>(bytes[offset + 2]) << 16
        | static_cast<std::uint32_t>(bytes[offset + 3]) << 24;
}

void write_u32(std::vector<std::byte>& out, std::size_t offset, std::uint32_t value) noexcept
{
    for (int i = 0; i < 4; ++i) {
        out[offset + static_cast<std::size_t>(i)] = static_cast<std::byte>((value >> (8 * i)) & 0xFFU);
    }
}

SaveContainerError to_container_error(detail::ZlibError error) noexcept
{
    switch (error) {
    case detail::ZlibError::inflate_failed: return SaveContainerError::inflate_failed;
    case detail::ZlibError::size_mismatch: return SaveContainerError::inflated_size_mismatch;
    case detail::ZlibError::deflate_failed: return SaveContainerError::deflate_failed;
    case detail::ZlibError::too_large: return SaveContainerError::too_large;
    }
    return SaveContainerError::inflate_failed;
}

std::expected<std::vector<std::byte>, SaveContainerError> inflate_exact(
    std::span<const std::byte> compressed, std::uint32_t expected_size)
{
    return detail::inflate_exact(compressed, expected_size).transform_error(to_container_error);
}

// Container chunks are deflated at level 9 (SaveContainerCodec::encode).
std::expected<std::vector<std::byte>, SaveContainerError> deflate_retail(std::span<const std::byte> data)
{
    return detail::deflate_retail(data, 9).transform_error(to_container_error);
}

} // namespace

std::uint32_t ccrc(std::span<const std::byte> data, std::uint32_t seed) noexcept
{
    std::uint32_t crc = seed;
    for (const std::byte b : data) {
        crc = (crc >> 8) ^ crc_table[(crc ^ static_cast<std::uint32_t>(b)) & 0xFFU];
    }
    return crc;
}

std::expected<SaveContainer, SaveContainerError> SaveContainerCodec::decode(std::span<const std::byte> file)
{
    if (file.size() < header_size) {
        return std::unexpected(SaveContainerError::truncated_header);
    }
    if (!std::equal(magic.begin(), magic.end(), file.begin())) {
        return std::unexpected(SaveContainerError::bad_magic);
    }

    // VerifySignature order: bounds, CRC, then slab size and padding.
    const std::size_t total = read_u32(file, 0x0C);
    if (total < header_size || total > file.size() - 4) {
        return std::unexpected(SaveContainerError::data_length_out_of_range);
    }
    if (read_u32(file, total) != ccrc(file.first(total))) {
        return std::unexpected(SaveContainerError::bad_signature);
    }
    if (file.size() != slab_size) {
        return std::unexpected(SaveContainerError::bad_slab_size);
    }
    const auto padding = file.subspan(total + 4);
    if (std::any_of(padding.begin(), padding.end(), [](std::byte b) { return b != std::byte{0}; })) {
        return std::unexpected(SaveContainerError::nonzero_padding);
    }

    const std::uint32_t header_bytes = read_u32(file, 0x10);
    const std::uint32_t body_bytes = read_u32(file, 0x14);
    const std::size_t header_zlib = read_u32(file, 0x18);
    if (header_zlib > total - header_size || total - header_size - header_zlib < 4) {
        return std::unexpected(SaveContainerError::chunk_out_of_bounds);
    }
    const std::size_t body_zlib_at = header_size + header_zlib + 4;
    const std::size_t body_zlib = read_u32(file, header_size + header_zlib);
    if (body_zlib > total - body_zlib_at) {
        return std::unexpected(SaveContainerError::chunk_out_of_bounds);
    }
    if (body_zlib_at + body_zlib != total) {
        return std::unexpected(SaveContainerError::chunk_layout_mismatch);
    }

    auto header = inflate_exact(file.subspan(header_size, header_zlib), header_bytes);
    if (!header) {
        return std::unexpected(header.error());
    }
    auto body = inflate_exact(file.subspan(body_zlib_at, body_zlib), body_bytes);
    if (!body) {
        return std::unexpected(body.error());
    }
    return SaveContainer{std::move(*header), std::move(*body)};
}

std::expected<std::vector<std::byte>, SaveContainerError> SaveContainerCodec::encode(const SaveContainer& container)
{
    if (container.header_section.size() > std::numeric_limits<std::uint32_t>::max()
        || container.body_section.size() > std::numeric_limits<std::uint32_t>::max()) {
        return std::unexpected(SaveContainerError::too_large);
    }
    auto header = deflate_retail(container.header_section);
    if (!header) {
        return std::unexpected(header.error());
    }
    auto body = deflate_retail(container.body_section);
    if (!body) {
        return std::unexpected(body.error());
    }

    const std::size_t total = header_size + header->size() + 4 + body->size();
    if (total + 4 > slab_size) {
        return std::unexpected(SaveContainerError::too_large);
    }

    std::vector<std::byte> file(slab_size, std::byte{0});
    std::copy(magic.begin(), magic.end(), file.begin());
    write_u32(file, 0x0C, static_cast<std::uint32_t>(total));
    write_u32(file, 0x10, static_cast<std::uint32_t>(container.header_section.size()));
    write_u32(file, 0x14, static_cast<std::uint32_t>(container.body_section.size()));
    write_u32(file, 0x18, static_cast<std::uint32_t>(header->size()));
    std::copy(header->begin(), header->end(), file.begin() + header_size);
    const std::size_t body_len_at = header_size + header->size();
    write_u32(file, body_len_at, static_cast<std::uint32_t>(body->size()));
    std::copy(body->begin(), body->end(), file.begin() + static_cast<std::ptrdiff_t>(body_len_at + 4));
    write_u32(file, total, ccrc(std::span<const std::byte>(file).first(total)));
    return file;
}

} // namespace fable::save
