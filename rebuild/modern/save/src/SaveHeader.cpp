#include <fable/save/SaveHeader.hpp>

namespace fable::save {
namespace {

// Reads `[tag(name)][value]` with the reader's `method` into `target`; returns the first error.
#define FABLE_READ_FIELD(reader, name, method, target)        \
    do {                                                       \
        if (auto t = (reader).tag(name); !t) {                 \
            return std::unexpected(t.error());                 \
        }                                                      \
        auto v = (reader).method();                            \
        if (!v) {                                              \
            return std::unexpected(v.error());                 \
        }                                                      \
        (target) = std::move(*v);                              \
    } while (false)

std::expected<SaveHeader::Details, PersistError> read_details(PersistReader& in)
{
    SaveHeader::Details d;
    FABLE_READ_FIELD(in, "TeleportingEnabled", boolean, d.teleporting_enabled);
    FABLE_READ_FIELD(in, "SavingEnabled", boolean, d.saving_enabled);
    FABLE_READ_FIELD(in, "ExperienceSpendingEnabled", boolean, d.experience_spending_enabled);
    FABLE_READ_FIELD(in, "CreatureGenerationEnabled", boolean, d.creature_generation_enabled);
    FABLE_READ_FIELD(in, "CreatureGenerationDisabledGroups", i32, d.creature_generation_disabled_groups);
    FABLE_READ_FIELD(in, "HeroSleepingEnabled", boolean, d.hero_sleeping_enabled);
    FABLE_READ_FIELD(in, "MapTableShowQuestCardsOnUsed", boolean, d.map_table_show_quest_cards_on_used);
    FABLE_READ_FIELD(in, "MiniMapEnabled", boolean, d.mini_map_enabled);
    FABLE_READ_FIELD(in, "MiniMapActiveBeforeDisabled", boolean, d.mini_map_active_before_disabled);
    FABLE_READ_FIELD(in, "GuildMasterMessagesEnabled", boolean, d.guild_master_messages_enabled);
    FABLE_READ_FIELD(in, "SummonerDeathExplosionAffectsHero", boolean, d.summoner_death_explosion_affects_hero);
    FABLE_READ_FIELD(in, "MostRecentSaveType", i32, d.most_recent_save_type);
    FABLE_READ_FIELD(in, "MostRecentSaveTypeBeforeManualSave", i32, d.most_recent_save_type_before_manual_save);
    FABLE_READ_FIELD(in, "MostRecentManualSaveName", wstring, d.most_recent_manual_save_name);
    FABLE_READ_FIELD(in, "SaveGameMarkerPos", vector3, d.save_game_marker_pos);
    FABLE_READ_FIELD(in, "SaveGameMarkerAngleXY", f32, d.save_game_marker_angle_xy);
    FABLE_READ_FIELD(in, "GuildSealRecallPos", vector3, d.guild_seal_recall_pos);
    FABLE_READ_FIELD(in, "GuildSealRecallAngleXY", f32, d.guild_seal_recall_angle_xy);
    FABLE_READ_FIELD(in, "CurrentRegionName", cstring, d.current_region_name);
    FABLE_READ_FIELD(in, "CurrentRegionMinimapGraphicName", cstring, d.current_region_minimap_graphic_name);
    FABLE_READ_FIELD(in, "TotalTimePlayed", f32, d.total_time_played);
    return d;
}

} // namespace

std::expected<SaveHeader, PersistError> SaveHeader::decode(std::span<const std::byte> section)
{
    PersistReader outer(section);
    const auto body = outer.section("HEADER");
    if (!body) {
        return std::unexpected(body.error());
    }
    if (outer.remaining() != 0) {
        return std::unexpected(PersistError::bad_section);
    }

    PersistReader in(*body);
    SaveHeader header;
    FABLE_READ_FIELD(in, "WorldName", cstring, header.world_name);
    FABLE_READ_FIELD(in, "WorldFrame", i32, header.world_frame);
    if (in.remaining() != 0) {
        auto details = read_details(in);
        if (!details) {
            return std::unexpected(details.error());
        }
        header.details = std::move(*details);
    }
    if (in.remaining() != 0) {
        return std::unexpected(PersistError::bad_section);
    }
    return header;
}

#undef FABLE_READ_FIELD

std::vector<std::byte> SaveHeader::encode() const
{
    PersistWriter out;
    out.tag("WorldName");
    out.cstring(world_name);
    out.tag("WorldFrame");
    out.i32(world_frame);
    if (details) {
        const Details& d = *details;
        out.tag("TeleportingEnabled");
        out.boolean(d.teleporting_enabled);
        out.tag("SavingEnabled");
        out.boolean(d.saving_enabled);
        out.tag("ExperienceSpendingEnabled");
        out.boolean(d.experience_spending_enabled);
        out.tag("CreatureGenerationEnabled");
        out.boolean(d.creature_generation_enabled);
        out.tag("CreatureGenerationDisabledGroups");
        out.i32(d.creature_generation_disabled_groups);
        out.tag("HeroSleepingEnabled");
        out.boolean(d.hero_sleeping_enabled);
        out.tag("MapTableShowQuestCardsOnUsed");
        out.boolean(d.map_table_show_quest_cards_on_used);
        out.tag("MiniMapEnabled");
        out.boolean(d.mini_map_enabled);
        out.tag("MiniMapActiveBeforeDisabled");
        out.boolean(d.mini_map_active_before_disabled);
        out.tag("GuildMasterMessagesEnabled");
        out.boolean(d.guild_master_messages_enabled);
        out.tag("SummonerDeathExplosionAffectsHero");
        out.boolean(d.summoner_death_explosion_affects_hero);
        out.tag("MostRecentSaveType");
        out.i32(d.most_recent_save_type);
        out.tag("MostRecentSaveTypeBeforeManualSave");
        out.i32(d.most_recent_save_type_before_manual_save);
        out.tag("MostRecentManualSaveName");
        out.wstring(d.most_recent_manual_save_name);
        out.tag("SaveGameMarkerPos");
        out.vector3(d.save_game_marker_pos);
        out.tag("SaveGameMarkerAngleXY");
        out.f32(d.save_game_marker_angle_xy);
        out.tag("GuildSealRecallPos");
        out.vector3(d.guild_seal_recall_pos);
        out.tag("GuildSealRecallAngleXY");
        out.f32(d.guild_seal_recall_angle_xy);
        out.tag("CurrentRegionName");
        out.cstring(d.current_region_name);
        out.tag("CurrentRegionMinimapGraphicName");
        out.cstring(d.current_region_minimap_graphic_name);
        out.tag("TotalTimePlayed");
        out.f32(d.total_time_played);
    }
    PersistWriter section;
    section.section("HEADER", out.bytes());
    return section.take();
}

} // namespace fable::save
