#include <fable/save/PersistStream.hpp>

#include <fable/save/SaveContainer.hpp>

#include <bit>

namespace fable::save {
namespace {

std::uint32_t le32(std::span<const std::byte> b) noexcept
{
    return static_cast<std::uint32_t>(b[0]) | static_cast<std::uint32_t>(b[1]) << 8
        | static_cast<std::uint32_t>(b[2]) << 16 | static_cast<std::uint32_t>(b[3]) << 24;
}

std::span<const std::byte> as_bytes(std::string_view text) noexcept
{
    return {reinterpret_cast<const std::byte*>(text.data()), text.size()};
}

} // namespace

std::uint32_t field_tag(std::string_view name) noexcept
{
    return ccrc(as_bytes(name));
}

std::expected<std::span<const std::byte>, PersistError> PersistReader::take(std::size_t count)
{
    if (count > remaining()) {
        return std::unexpected(PersistError::truncated);
    }
    const auto out = bytes_.subspan(pos_, count);
    pos_ += count;
    return out;
}

std::expected<void, PersistError> PersistReader::tag(std::string_view name)
{
    const auto value = u32();
    if (!value) {
        return std::unexpected(value.error());
    }
    if (*value != field_tag(name)) {
        return std::unexpected(PersistError::tag_mismatch);
    }
    return {};
}

std::expected<std::uint32_t, PersistError> PersistReader::u32()
{
    return take(4).transform(le32);
}

std::expected<std::int32_t, PersistError> PersistReader::i32()
{
    return u32().transform([](std::uint32_t v) { return static_cast<std::int32_t>(v); });
}

std::expected<std::uint64_t, PersistError> PersistReader::u64()
{
    return take(8).transform([](std::span<const std::byte> b) {
        return static_cast<std::uint64_t>(le32(b.first(4))) | static_cast<std::uint64_t>(le32(b.subspan(4))) << 32;
    });
}

std::expected<float, PersistError> PersistReader::f32()
{
    return u32().transform([](std::uint32_t v) { return std::bit_cast<float>(v); });
}

std::expected<bool, PersistError> PersistReader::boolean()
{
    const auto b = take(1);
    if (!b) {
        return std::unexpected(b.error());
    }
    const auto v = std::to_integer<unsigned>((*b)[0]);
    if (v > 1) {
        return std::unexpected(PersistError::invalid_bool);
    }
    return v == 1;
}

std::expected<std::string, PersistError> PersistReader::cstring()
{
    std::string out;
    for (std::size_t i = pos_; i < bytes_.size(); ++i) {
        if (bytes_[i] == std::byte{0}) {
            pos_ = i + 1;
            return out;
        }
        out.push_back(static_cast<char>(bytes_[i]));
    }
    return std::unexpected(PersistError::unterminated);
}

std::expected<std::string, PersistError> PersistReader::counted_string()
{
    const auto length = u32();
    if (!length) {
        return std::unexpected(length.error());
    }
    const auto raw = take(*length);
    if (!raw) {
        return std::unexpected(raw.error());
    }
    std::string out;
    out.reserve(raw->size());
    for (const std::byte b : *raw) {
        out.push_back(static_cast<char>(b));
    }
    return out;
}

std::expected<std::u16string, PersistError> PersistReader::wstring()
{
    std::u16string out;
    for (std::size_t i = pos_; i + 1 < bytes_.size(); i += 2) {
        const auto unit = static_cast<char16_t>(std::to_integer<unsigned>(bytes_[i])
                                                | std::to_integer<unsigned>(bytes_[i + 1]) << 8);
        if (unit == 0) {
            pos_ = i + 2;
            return out;
        }
        out.push_back(unit);
    }
    return std::unexpected(PersistError::unterminated);
}

std::expected<std::array<float, 3>, PersistError> PersistReader::vector3()
{
    std::array<float, 3> out{};
    for (float& v : out) {
        const auto f = f32();
        if (!f) {
            return std::unexpected(f.error());
        }
        v = *f;
    }
    return out;
}

std::expected<std::span<const std::byte>, PersistError> PersistReader::raw(std::size_t count)
{
    return take(count);
}

std::expected<std::span<const std::byte>, PersistError> PersistReader::section(std::string_view name)
{
    const auto got = cstring();
    if (!got) {
        return std::unexpected(got.error());
    }
    if (*got != name) {
        return std::unexpected(PersistError::bad_section);
    }
    const auto length = u32();
    if (!length) {
        return std::unexpected(length.error());
    }
    return take(*length);
}

void PersistWriter::tag(std::string_view name)
{
    u32(field_tag(name));
}

void PersistWriter::u32(std::uint32_t value)
{
    for (int i = 0; i < 4; ++i) {
        bytes_.push_back(static_cast<std::byte>((value >> (8 * i)) & 0xFFU));
    }
}

void PersistWriter::i32(std::int32_t value)
{
    u32(static_cast<std::uint32_t>(value));
}

void PersistWriter::u64(std::uint64_t value)
{
    u32(static_cast<std::uint32_t>(value & 0xFFFFFFFFU));
    u32(static_cast<std::uint32_t>(value >> 32));
}

void PersistWriter::f32(float value)
{
    u32(std::bit_cast<std::uint32_t>(value));
}

void PersistWriter::boolean(bool value)
{
    bytes_.push_back(value ? std::byte{1} : std::byte{0});
}

void PersistWriter::cstring(std::string_view value)
{
    const auto raw = as_bytes(value);
    bytes_.insert(bytes_.end(), raw.begin(), raw.end());
    bytes_.push_back(std::byte{0});
}

void PersistWriter::counted_string(std::string_view value)
{
    u32(static_cast<std::uint32_t>(value.size()));
    const auto raw = as_bytes(value);
    bytes_.insert(bytes_.end(), raw.begin(), raw.end());
}

void PersistWriter::wstring(std::u16string_view value)
{
    for (const char16_t unit : value) {
        bytes_.push_back(static_cast<std::byte>(unit & 0xFFU));
        bytes_.push_back(static_cast<std::byte>(unit >> 8));
    }
    bytes_.push_back(std::byte{0});
    bytes_.push_back(std::byte{0});
}

void PersistWriter::vector3(const std::array<float, 3>& value)
{
    for (const float v : value) {
        f32(v);
    }
}

void PersistWriter::section(std::string_view name, std::span<const std::byte> body)
{
    cstring(name);
    u32(static_cast<std::uint32_t>(body.size()));
    bytes_raw(body);
}

void PersistWriter::bytes_raw(std::span<const std::byte> raw)
{
    bytes_.insert(bytes_.end(), raw.begin(), raw.end());
}

} // namespace fable::save
