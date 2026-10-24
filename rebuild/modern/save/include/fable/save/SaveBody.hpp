#pragma once

#include <fable/save/PersistStream.hpp>

#include <cstddef>
#include <cstdint>
#include <expected>
#include <optional>
#include <span>
#include <string>
#include <vector>

namespace fable::save {

// The body section, chunk1 of a `.sav`, as CWorld::SaveGameStateInternal (0x0049F4C0) writes it:
//
//   [game clock: Time f32, TimeStep f32, DayCount i32]        (written by 0x006BBB30)
//   ENTITIES  section                                         (CThingManager, via 0x00524290)
//   PLAYER    section                                         (CPlayerManager::SaveGameState 0x00449F90)
//   QUESTS    section                                         (0x004B64E0)
//   REGIONS   section                                         (0x004FDE80)
//   FACTIONS  section                                         (0x006B9560)
//
// In the writer's minimal-context branch (`this+0x102 != 0`, the `.qs.hs` files) only ENTITIES
// is written. Each section is `name\0`, an i32 length patched by EndSection (0x009BABF0), then its
// bytes. Section contents other than PLAYER stay opaque bytes at this layer.
struct SaveBody {
    struct GameClock {
        float time = 0.0F;
        float time_step = 0.0F;
        std::int32_t day_count = 0;

        friend bool operator==(const GameClock&, const GameClock&) = default;
    };

    struct Section {
        std::string name;
        std::vector<std::byte> bytes;

        friend bool operator==(const Section&, const Section&) = default;
    };

    std::optional<GameClock> clock;
    std::vector<Section> sections;

    friend bool operator==(const SaveBody&, const SaveBody&) = default;

    // Requires the sections to tile the stream exactly.
    [[nodiscard]] static std::expected<SaveBody, PersistError> decode(std::span<const std::byte> body);
    [[nodiscard]] std::vector<std::byte> encode() const;

    [[nodiscard]] const Section* find(std::string_view name) const noexcept;

    // True for the two shapes the retail writer produces: clock + ENTITIES, PLAYER, QUESTS,
    // REGIONS, FACTIONS, or no clock + ENTITIES alone (minimal context).
    [[nodiscard]] bool has_retail_shape() const noexcept;
};

// The PLAYER section (SAVE.md §3.5): exactly two tagged fields.
struct PlayerSection {
    std::uint64_t player_character_uid = 0;   // UID of the hero CThing in SAVED_ENTITIES
    std::string player_region_name;           // e.g. "Greatwood"

    friend bool operator==(const PlayerSection&, const PlayerSection&) = default;

    [[nodiscard]] static std::expected<PlayerSection, PersistError> decode(std::span<const std::byte> section);
    [[nodiscard]] std::vector<std::byte> encode() const;
};

} // namespace fable::save
