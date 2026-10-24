#include <fable/save/SaveBody.hpp>

#include <algorithm>
#include <array>
#include <string_view>

namespace fable::save {
namespace {

constexpr std::size_t max_section_name = 64;

// A section name is printable ASCII, so a stray length or tag is never mistaken for one.
bool plausible_name(std::span<const std::byte> bytes, std::size_t at)
{
    std::size_t i = at;
    for (; i < bytes.size() && i - at <= max_section_name; ++i) {
        const auto c = std::to_integer<unsigned>(bytes[i]);
        if (c == 0) {
            return i > at;
        }
        if (c < 0x20 || c > 0x7E) {
            return false;
        }
    }
    return false;
}

} // namespace

std::expected<SaveBody, PersistError> SaveBody::decode(std::span<const std::byte> body)
{
    PersistReader in(body);
    SaveBody out;

    // The clock is the only content that is not a section; its first tag tells them apart.
    if (body.size() >= 4 && !plausible_name(body, 0)) {
        GameClock clock;
        for (auto [name, target] : {std::pair{"Time", &clock.time}, std::pair{"TimeStep", &clock.time_step}}) {
            if (auto t = in.tag(name); !t) {
                return std::unexpected(t.error());
            }
            const auto v = in.f32();
            if (!v) {
                return std::unexpected(v.error());
            }
            *target = *v;
        }
        if (auto t = in.tag("DayCount"); !t) {
            return std::unexpected(t.error());
        }
        const auto days = in.i32();
        if (!days) {
            return std::unexpected(days.error());
        }
        clock.day_count = *days;
        out.clock = clock;
    }

    while (in.remaining() != 0) {
        if (!plausible_name(body, in.position())) {
            return std::unexpected(PersistError::bad_section);
        }
        auto name = in.cstring();
        if (!name) {
            return std::unexpected(name.error());
        }
        const auto length = in.i32();
        if (!length) {
            return std::unexpected(length.error());
        }
        if (*length < 0) {
            return std::unexpected(PersistError::bad_section);
        }
        const auto bytes = in.raw(static_cast<std::size_t>(*length));
        if (!bytes) {
            return std::unexpected(PersistError::bad_section);
        }
        out.sections.push_back(Section{std::move(*name), {bytes->begin(), bytes->end()}});
    }
    return out;
}

std::vector<std::byte> SaveBody::encode() const
{
    PersistWriter out;
    if (clock) {
        out.tag("Time");
        out.f32(clock->time);
        out.tag("TimeStep");
        out.f32(clock->time_step);
        out.tag("DayCount");
        out.i32(clock->day_count);
    }
    for (const Section& s : sections) {
        out.section(s.name, s.bytes);
    }
    return out.take();
}

const SaveBody::Section* SaveBody::find(std::string_view name) const noexcept
{
    const auto it = std::find_if(sections.begin(), sections.end(), [&](const Section& s) { return s.name == name; });
    return it == sections.end() ? nullptr : &*it;
}

bool SaveBody::has_retail_shape() const noexcept
{
    constexpr std::array<std::string_view, 5> full{"ENTITIES", "PLAYER", "QUESTS", "REGIONS", "FACTIONS"};
    const auto names_are = [&](std::span<const std::string_view> expected) {
        return std::equal(sections.begin(), sections.end(), expected.begin(), expected.end(),
                          [](const Section& s, std::string_view n) { return s.name == n; });
    };
    return clock ? names_are(full) : names_are(std::span(full).first(1));
}

std::expected<PlayerSection, PersistError> PlayerSection::decode(std::span<const std::byte> section)
{
    PersistReader in(section);
    PlayerSection out;
    if (auto t = in.tag("PlayerCharacterUID"); !t) {
        return std::unexpected(t.error());
    }
    const auto uid = in.u64();
    if (!uid) {
        return std::unexpected(uid.error());
    }
    out.player_character_uid = *uid;
    if (auto t = in.tag("PlayerRegionName"); !t) {
        return std::unexpected(t.error());
    }
    auto region = in.cstring();
    if (!region) {
        return std::unexpected(region.error());
    }
    out.player_region_name = std::move(*region);
    if (in.remaining() != 0) {
        return std::unexpected(PersistError::bad_section);
    }
    return out;
}

std::vector<std::byte> PlayerSection::encode() const
{
    PersistWriter out;
    out.tag("PlayerCharacterUID");
    out.u64(player_character_uid);
    out.tag("PlayerRegionName");
    out.cstring(player_region_name);
    return out.take();
}

} // namespace fable::save
