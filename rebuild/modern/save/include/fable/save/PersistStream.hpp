#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <expected>
#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace fable::save {

enum class PersistError {
    truncated,      // the stream ends inside a tag or value
    tag_mismatch,   // retail CheckCRC asserts stream_crc == CCharString(name).GetCRC()
    invalid_bool,   // a bool byte other than 0 or 1
    unterminated,   // a string runs to the end of the stream without its NUL
    bad_section,    // section name or length framing does not match
};

// The on-disk tag of a field: seed-0 CCRC::Calc over the field name's bytes, no terminator.
[[nodiscard]] std::uint32_t field_tag(std::string_view name) noexcept;

// Reader for the binary CPersistContext stream (mode 2). Every Transfer<T>(name, value) is
// `[u32 field_tag(name)][value]`, and value widths are fixed by T, not by the host: long and
// unsigned long are 4 bytes, CWideString is UTF-16LE, UIDs are 8 bytes (docs/modern/PERSISTED_TYPES.md).
class PersistReader final {
public:
    explicit PersistReader(std::span<const std::byte> bytes) noexcept : bytes_(bytes) {}

    [[nodiscard]] std::size_t position() const noexcept { return pos_; }
    [[nodiscard]] std::size_t remaining() const noexcept { return bytes_.size() - pos_; }

    [[nodiscard]] std::expected<void, PersistError> tag(std::string_view name);

    [[nodiscard]] std::expected<std::uint32_t, PersistError> u32();
    [[nodiscard]] std::expected<std::int32_t, PersistError> i32();
    [[nodiscard]] std::expected<std::uint64_t, PersistError> u64();
    [[nodiscard]] std::expected<float, PersistError> f32();
    [[nodiscard]] std::expected<bool, PersistError> boolean();
    [[nodiscard]] std::expected<std::string, PersistError> cstring();      // CCharString: bytes + NUL
    [[nodiscard]] std::expected<std::u16string, PersistError> wstring();   // CWideString: UTF-16LE + NUL
    [[nodiscard]] std::expected<std::string, PersistError> counted_string();   // u32 length, then bytes (0x995A80)
    [[nodiscard]] std::expected<std::array<float, 3>, PersistError> vector3();
    [[nodiscard]] std::expected<std::span<const std::byte>, PersistError> raw(std::size_t count);

    // `name\0` then u32 length; returns the section's bytes and advances past them.
    [[nodiscard]] std::expected<std::span<const std::byte>, PersistError> section(std::string_view name);

private:
    [[nodiscard]] std::expected<std::span<const std::byte>, PersistError> take(std::size_t count);

    std::span<const std::byte> bytes_;
    std::size_t pos_ = 0;
};

// Writer for the binary CPersistContext stream (mode 3); the exact inverse of PersistReader.
class PersistWriter final {
public:
    void tag(std::string_view name);
    void u32(std::uint32_t value);
    void i32(std::int32_t value);
    void u64(std::uint64_t value);
    void f32(float value);
    void boolean(bool value);
    void cstring(std::string_view value);
    void wstring(std::u16string_view value);
    void counted_string(std::string_view value);
    void vector3(const std::array<float, 3>& value);

    // Wraps `body` as a named section: `name\0`, u32 body length, body.
    void section(std::string_view name, std::span<const std::byte> body);
    void bytes_raw(std::span<const std::byte> raw);

    [[nodiscard]] const std::vector<std::byte>& bytes() const noexcept { return bytes_; }
    [[nodiscard]] std::vector<std::byte> take() noexcept { return std::move(bytes_); }

private:
    std::vector<std::byte> bytes_;
};

} // namespace fable::save
