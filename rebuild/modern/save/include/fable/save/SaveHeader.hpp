#pragma once

#include <fable/save/PersistStream.hpp>

#include <array>
#include <cstddef>
#include <cstdint>
#include <expected>
#include <optional>
#include <span>
#include <string>
#include <vector>

namespace fable::save {

// The HEADER section, chunk0 of a `.sav`. Written by CWorld::SaveGameStateInternal (0x0049F4C0);
// field order is its Transfer call order (docs/formats/SAVE.md §3.4, 23/23 fields confirmed).
struct SaveHeader {
    // Fields 2-22. The writer skips them in its minimal-context branch (`this+0x102 != 0`).
    struct Details {
        bool teleporting_enabled = false;
        bool saving_enabled = false;
        bool experience_spending_enabled = false;
        bool creature_generation_enabled = false;
        std::int32_t creature_generation_disabled_groups = 0;   // bitmask
        bool hero_sleeping_enabled = false;
        bool map_table_show_quest_cards_on_used = false;
        bool mini_map_enabled = false;
        bool mini_map_active_before_disabled = false;
        bool guild_master_messages_enabled = false;
        bool summoner_death_explosion_affects_hero = false;
        std::int32_t most_recent_save_type = 0;                 // CWorld::ESaveType
        std::int32_t most_recent_save_type_before_manual_save = 0;
        std::u16string most_recent_manual_save_name;            // empty in autosaves
        std::array<float, 3> save_game_marker_pos{};
        float save_game_marker_angle_xy = 0.0F;
        std::array<float, 3> guild_seal_recall_pos{};
        float guild_seal_recall_angle_xy = 0.0F;
        std::string current_region_name;                        // e.g. TXT_REGION_OAKVALE
        std::string current_region_minimap_graphic_name;        // e.g. MINIMAP_GREATWOOD
        float total_time_played = 0.0F;                         // 4 bytes on disk

        friend bool operator==(const Details&, const Details&) = default;
    };

    std::string world_name;        // "FinalAlbion"
    std::int32_t world_frame = 0;  // world frames (15 per second) at save time
    std::optional<Details> details;

    friend bool operator==(const SaveHeader&, const SaveHeader&) = default;

    // `section` is the whole chunk0 stream: "HEADER\0", u32 length, tagged fields.
    [[nodiscard]] static std::expected<SaveHeader, PersistError> decode(std::span<const std::byte> section);
    [[nodiscard]] std::vector<std::byte> encode() const;
};

} // namespace fable::save
