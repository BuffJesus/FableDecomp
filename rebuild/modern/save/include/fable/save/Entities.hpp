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

// One SAVED_ENTITIES record: a map cell's persisted entities, stored zlib-compressed at level 1.
// The data is the inflated entity stream (SAVE.md "Entity graph"); it stays opaque bytes here.
struct EntityCell {
    std::uint64_t cell_id = 0;
    std::vector<std::byte> data;            // empty = a record with no stream (clen 0)
    std::array<std::byte, 9> trailer{};     // all zero in every observed save

    friend bool operator==(const EntityCell&, const EntityCell&) = default;
};

// The SAVED_ENTITIES section:
//   u32 0, u32 slot_count, u32 0
//   slot_count slots, each either u64 0 (empty) or
//     u64 29+clen, u64 cell_id, u64 clen, u32 ulen, clen bytes zlib (level 1), 9 trailer bytes
//   [tag][u32 dead_count], dead_count × { [tag][u64 uid] [tag][u32 section_name_crc] }
// The tail is CEntityRuntimePersistence::PersistedDeadEntities, filtered to the game-persistent
// entries (writer 0x521620). Every tag is field_tag("") == 0.
struct PersistedDeadEntity {
    std::uint64_t uid = 0;
    // Seed-0 CRC of the owning section's name: the quest script that created the entity
    // (e.g. "Q_GuildTraining", "V_SickChild_Activate"), or "NULL" (0x321ACEE7) for none.
    std::uint32_t section_name_crc = 0;

    friend bool operator==(const PersistedDeadEntity&, const PersistedDeadEntity&) = default;
};

struct SavedEntities {
    std::vector<std::optional<EntityCell>> slots;
    std::vector<PersistedDeadEntity> dead_entities;

    friend bool operator==(const SavedEntities&, const SavedEntities&) = default;

    [[nodiscard]] static std::expected<SavedEntities, PersistError> decode(std::span<const std::byte> section);
    [[nodiscard]] std::expected<std::vector<std::byte>, PersistError> encode() const;
};

// SAVED_NPC_NAMES: CThingManager::NPCNameMap, map<UID, pair<CCharString, CWideString>>, written
// by 0x524440 in key order. Binary saves write u32 count, then per entry u64 uid, the group as a
// counted string and the name as NUL-terminated UTF-16. An empty group is written as "UNATTACHED"
// and read back as empty (loader 0x525A62), so `group` holds the in-memory value. A context
// with the +0x102 flag set (the minimal no-clock `.qs.hs` form) writes an empty section
// (`entries` = nullopt).
struct NpcName {
    std::uint64_t uid = 0;
    std::string group;
    std::u16string name;

    friend bool operator==(const NpcName&, const NpcName&) = default;
};

struct SavedNpcNames {
    std::optional<std::vector<NpcName>> entries;

    friend bool operator==(const SavedNpcNames&, const SavedNpcNames&) = default;

    [[nodiscard]] static std::expected<SavedNpcNames, PersistError> decode(std::span<const std::byte> section);
    [[nodiscard]] std::vector<std::byte> encode() const;
};

// The ENTITIES head of a full save (writer 0x524390): [tag][u32 format version] [tag][u64
// CThingManager::CurrentUniqueID], the next free global UID. Every tag is field_tag("") == 0.
// The minimal `.qs.hs` form has no head.
struct EntitiesHead {
    std::uint32_t format_version = 2;   // 2 in every observed save
    std::uint64_t current_unique_id = 0;

    friend bool operator==(const EntitiesHead&, const EntitiesHead&) = default;
};

// The ENTITIES section: the optional head, then the SAVED_ENTITIES and SAVED_NPC_NAMES sections.
struct EntitiesSection {
    std::optional<EntitiesHead> head;
    SavedEntities saved_entities;
    SavedNpcNames saved_npc_names;

    friend bool operator==(const EntitiesSection&, const EntitiesSection&) = default;

    [[nodiscard]] static std::expected<EntitiesSection, PersistError> decode(std::span<const std::byte> section);
    [[nodiscard]] std::expected<std::vector<std::byte>, PersistError> encode() const;
};

} // namespace fable::save
