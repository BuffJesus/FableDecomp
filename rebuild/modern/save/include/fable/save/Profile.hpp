#pragma once

#include <cstddef>
#include <expected>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace fable::save {

enum class ProfileError {
    bad_magic,                 // not "FablePro" + u32 0
    truncated,
    data_length_out_of_range,  // total_data_len must equal file size - 4
    bad_signature,             // trailer != ccrc(file[0 : total_data_len])
    bad_line,                  // a line that is not `Name value;` + CRLF
};

// `Profile.bin`: the `FablePro` container around a text-mode CPersistContext script
// (docs/formats/SAVE.md "Profile index"):
//   0x00 "FablePro", u32 0, u32 total_data_len (= file size - 4), latin1 text, u32 ccrc trailer.
// The text is one `Name value;\r\n` line per field, in writer order, so entries keep their order
// and raw value spelling and re-encode byte for byte.
//
// Retail VerifySignature (0x00409730) also has a 0x4000-size / 0x20-padding branch for this magic,
// but every real profile is variable-length and unpadded (153/153 here). The decoder applies the
// CRC rule the files actually satisfy.
class Profile final {
public:
    struct Entry {
        std::string name;    // e.g. "SaveGameNames1", "AssignedInputsPassive[3]"
        std::string value;   // raw text between the space and the ';', e.g. "\"Manual - Save1\""

        friend bool operator==(const Entry&, const Entry&) = default;
    };

    static constexpr std::size_t save_slot_count = 50;

    [[nodiscard]] static std::expected<Profile, ProfileError> decode(std::span<const std::byte> file);
    [[nodiscard]] std::vector<std::byte> encode() const;

    [[nodiscard]] const std::vector<Entry>& entries() const noexcept { return entries_; }
    [[nodiscard]] std::optional<std::string_view> value(std::string_view name) const noexcept;
    // Replaces an existing entry's raw value; returns false if there is no such entry.
    bool set_value(std::string_view name, std::string value);

    // The manual-save registry `SaveGameNames1..50`, unquoted. An empty string is a free slot.
    [[nodiscard]] std::vector<std::string> save_game_names() const;

    friend bool operator==(const Profile&, const Profile&) = default;

private:
    std::vector<Entry> entries_;
};

} // namespace fable::save
