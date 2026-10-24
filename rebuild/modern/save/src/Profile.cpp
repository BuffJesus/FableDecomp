#include <fable/save/Profile.hpp>

#include <fable/save/SaveContainer.hpp>

#include <algorithm>
#include <array>
#include <cstdint>

namespace fable::save {
namespace {

constexpr std::array<std::byte, 12> magic{
    std::byte{'F'}, std::byte{'a'}, std::byte{'b'}, std::byte{'l'}, std::byte{'e'}, std::byte{'P'},
    std::byte{'r'}, std::byte{'o'}, std::byte{0}, std::byte{0}, std::byte{0}, std::byte{0},
};
constexpr std::size_t text_start = 0x10;

std::uint32_t read_u32(std::span<const std::byte> bytes, std::size_t offset) noexcept
{
    return static_cast<std::uint32_t>(bytes[offset]) | static_cast<std::uint32_t>(bytes[offset + 1]) << 8
        | static_cast<std::uint32_t>(bytes[offset + 2]) << 16 | static_cast<std::uint32_t>(bytes[offset + 3]) << 24;
}

void append_u32(std::vector<std::byte>& out, std::uint32_t value)
{
    for (int i = 0; i < 4; ++i) {
        out.push_back(static_cast<std::byte>((value >> (8 * i)) & 0xFFU));
    }
}

} // namespace

std::expected<Profile, ProfileError> Profile::decode(std::span<const std::byte> file)
{
    if (file.size() < text_start + 4) {
        return std::unexpected(ProfileError::truncated);
    }
    if (!std::equal(magic.begin(), magic.end(), file.begin())) {
        return std::unexpected(ProfileError::bad_magic);
    }
    const std::size_t total = read_u32(file, 0x0C);
    if (total != file.size() - 4 || total < text_start) {
        return std::unexpected(ProfileError::data_length_out_of_range);
    }
    if (read_u32(file, total) != ccrc(file.first(total))) {
        return std::unexpected(ProfileError::bad_signature);
    }

    const auto text = file.subspan(text_start, total - text_start);
    const std::string_view all(reinterpret_cast<const char*>(text.data()), text.size());
    Profile out;
    for (std::size_t at = 0; at < all.size();) {
        const auto end = all.find("\r\n", at);
        if (end == std::string_view::npos) {
            return std::unexpected(ProfileError::bad_line);
        }
        const std::string_view line = all.substr(at, end - at);
        const auto space = line.find(' ');
        if (space == std::string_view::npos || space == 0 || !line.ends_with(';')) {
            return std::unexpected(ProfileError::bad_line);
        }
        out.entries_.push_back(Entry{std::string(line.substr(0, space)),
                                     std::string(line.substr(space + 1, line.size() - space - 2))});
        at = end + 2;
    }
    return out;
}

std::vector<std::byte> Profile::encode() const
{
    std::vector<std::byte> out(magic.begin(), magic.end());
    append_u32(out, 0);   // total_data_len, patched below
    for (const Entry& e : entries_) {
        for (const char c : e.name + ' ' + e.value + ";\r\n") {
            out.push_back(static_cast<std::byte>(c));
        }
    }
    const auto total = static_cast<std::uint32_t>(out.size());
    for (int i = 0; i < 4; ++i) {
        out[0x0C + static_cast<std::size_t>(i)] = static_cast<std::byte>((total >> (8 * i)) & 0xFFU);
    }
    append_u32(out, ccrc(out));
    return out;
}

std::optional<std::string_view> Profile::value(std::string_view name) const noexcept
{
    const auto it = std::find_if(entries_.begin(), entries_.end(), [&](const Entry& e) { return e.name == name; });
    if (it == entries_.end()) {
        return std::nullopt;
    }
    return std::string_view(it->value);
}

bool Profile::set_value(std::string_view name, std::string value)
{
    const auto it = std::find_if(entries_.begin(), entries_.end(), [&](const Entry& e) { return e.name == name; });
    if (it == entries_.end()) {
        return false;
    }
    it->value = std::move(value);
    return true;
}

std::vector<std::string> Profile::save_game_names() const
{
    std::vector<std::string> names;
    for (std::size_t slot = 1; slot <= save_slot_count; ++slot) {
        const auto raw = value("SaveGameNames" + std::to_string(slot)).value_or("");
        const bool quoted = raw.size() >= 2 && raw.front() == '"' && raw.back() == '"';
        names.emplace_back(quoted ? raw.substr(1, raw.size() - 2) : raw);
    }
    return names;
}

} // namespace fable::save
