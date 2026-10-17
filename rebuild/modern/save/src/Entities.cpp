#include <fable/save/Entities.hpp>

#include "Zlib.hpp"

#include <algorithm>
#include <string_view>

namespace fable::save {
namespace {

constexpr std::uint64_t record_overhead = 29;   // cell_id 8 + clen 8 + ulen 4 + trailer 9
constexpr int cell_zlib_level = 1;
constexpr std::string_view unattached_group = "UNATTACHED";

// The position of the first `name\0` in `bytes`, as a section start.
std::optional<std::size_t> find_section(std::span<const std::byte> bytes, std::string_view name)
{
    const auto it = std::search(bytes.begin(), bytes.end(), name.begin(), name.end(),
                                [](std::byte b, char c) { return b == static_cast<std::byte>(c); });
    if (it == bytes.end()) {
        return std::nullopt;
    }
    const auto at = static_cast<std::size_t>(it - bytes.begin());
    if (at + name.size() >= bytes.size() || bytes[at + name.size()] != std::byte{0}) {
        return std::nullopt;
    }
    return at;
}

} // namespace

std::expected<SavedEntities, PersistError> SavedEntities::decode(std::span<const std::byte> section)
{
    PersistReader in(section);
    const auto lead = in.u32();
    const auto slot_count = in.u32();
    const auto pad = in.u32();
    if (!lead || !slot_count || !pad) {
        return std::unexpected(PersistError::truncated);
    }
    if (*lead != 0 || *pad != 0 || *slot_count > in.remaining() / 8) {
        return std::unexpected(PersistError::bad_section);
    }

    SavedEntities out;
    out.slots.reserve(*slot_count);
    for (std::uint32_t i = 0; i < *slot_count; ++i) {
        const auto record_length = in.u64();
        if (!record_length) {
            return std::unexpected(record_length.error());
        }
        if (*record_length == 0) {
            out.slots.emplace_back(std::nullopt);
            continue;
        }
        const auto cell_id = in.u64();
        const auto clen = in.u64();
        const auto ulen = in.u32();
        if (!cell_id || !clen || !ulen) {
            return std::unexpected(PersistError::truncated);
        }
        if (*record_length != record_overhead + *clen || *clen > in.remaining()) {
            return std::unexpected(PersistError::bad_section);
        }
        const auto compressed = in.raw(static_cast<std::size_t>(*clen));
        const auto trailer = in.raw(9);
        if (!compressed || !trailer) {
            return std::unexpected(PersistError::truncated);
        }
        EntityCell cell;
        cell.cell_id = *cell_id;
        std::copy(trailer->begin(), trailer->end(), cell.trailer.begin());
        if (*clen != 0) {
            auto data = detail::inflate_exact(*compressed, *ulen);
            if (!data) {
                return std::unexpected(PersistError::bad_section);
            }
            cell.data = std::move(*data);
        } else if (*ulen != 0) {
            return std::unexpected(PersistError::bad_section);
        }
        out.slots.emplace_back(std::move(cell));
    }

    const auto dead_count = in.u32();
    if (!dead_count) {
        return std::unexpected(dead_count.error());
    }
    if (in.remaining() != static_cast<std::size_t>(*dead_count) * 20) {
        return std::unexpected(PersistError::bad_section);
    }
    out.dead_entities.resize(*dead_count);
    for (PersistedDeadEntity& dead : out.dead_entities) {
        if (const auto tag = in.tag(""); !tag) {
            return std::unexpected(tag.error());
        }
        const auto uid = in.u64();
        if (const auto tag = in.tag(""); !tag) {
            return std::unexpected(tag.error());
        }
        const auto crc = in.u32();
        dead = PersistedDeadEntity{*uid, *crc};   // the size check above covers both reads
    }
    return out;
}

std::expected<std::vector<std::byte>, PersistError> SavedEntities::encode() const
{
    PersistWriter out;
    out.u32(0);
    out.u32(static_cast<std::uint32_t>(slots.size()));
    out.u32(0);
    for (const auto& slot : slots) {
        if (!slot) {
            out.u64(0);
            continue;
        }
        std::vector<std::byte> compressed;
        if (!slot->data.empty()) {
            auto deflated = detail::deflate_retail(slot->data, cell_zlib_level);
            if (!deflated) {
                return std::unexpected(PersistError::bad_section);
            }
            compressed = std::move(*deflated);
        }
        out.u64(record_overhead + compressed.size());
        out.u64(slot->cell_id);
        out.u64(compressed.size());
        out.u32(static_cast<std::uint32_t>(slot->data.size()));
        out.bytes_raw(compressed);
        out.bytes_raw(slot->trailer);
    }
    out.u32(static_cast<std::uint32_t>(dead_entities.size()));
    for (const PersistedDeadEntity& dead : dead_entities) {
        out.tag("");
        out.u64(dead.uid);
        out.tag("");
        out.u32(dead.section_name_crc);
    }
    return out.take();
}

std::expected<SavedNpcNames, PersistError> SavedNpcNames::decode(std::span<const std::byte> section)
{
    SavedNpcNames out;
    if (section.empty()) {
        return out;
    }
    PersistReader in(section);
    const auto count = in.u32();
    if (!count) {
        return std::unexpected(count.error());
    }
    // Each entry is at least u64 + u32 + a UTF-16 NUL.
    if (*count > in.remaining() / 14) {
        return std::unexpected(PersistError::bad_section);
    }
    out.entries.emplace();
    out.entries->reserve(*count);
    for (std::uint32_t i = 0; i < *count; ++i) {
        const auto uid = in.u64();
        if (!uid) {
            return std::unexpected(uid.error());
        }
        auto group = in.counted_string();
        if (!group) {
            return std::unexpected(group.error());
        }
        auto name = in.wstring();
        if (!name) {
            return std::unexpected(name.error());
        }
        if (*group == unattached_group) {
            group->clear();
        }
        out.entries->push_back(NpcName{*uid, std::move(*group), std::move(*name)});
    }
    if (in.remaining() != 0) {
        return std::unexpected(PersistError::bad_section);
    }
    return out;
}

std::vector<std::byte> SavedNpcNames::encode() const
{
    PersistWriter out;
    if (!entries) {
        return out.take();
    }
    out.u32(static_cast<std::uint32_t>(entries->size()));
    for (const NpcName& entry : *entries) {
        out.u64(entry.uid);
        out.counted_string(entry.group.empty() ? unattached_group : std::string_view(entry.group));
        out.wstring(entry.name);
    }
    return out.take();
}

std::expected<EntitiesSection, PersistError> EntitiesSection::decode(std::span<const std::byte> section)
{
    const auto at = find_section(section, "SAVED_ENTITIES");
    if (!at) {
        return std::unexpected(PersistError::bad_section);
    }
    EntitiesSection out;
    if (*at != 0) {
        PersistReader head(section.first(*at));
        const auto version_tag = head.tag("");
        const auto version = head.u32();
        const auto uid_tag = head.tag("");
        const auto uid = head.u64();
        if (!version_tag || !version || !uid_tag || !uid || head.remaining() != 0) {
            return std::unexpected(PersistError::bad_section);
        }
        out.head = EntitiesHead{*version, *uid};
    }

    PersistReader in(section.subspan(*at));
    const auto saved = in.section("SAVED_ENTITIES");
    if (!saved) {
        return std::unexpected(saved.error());
    }
    auto entities = SavedEntities::decode(*saved);
    if (!entities) {
        return std::unexpected(entities.error());
    }
    out.saved_entities = std::move(*entities);
    const auto names = in.section("SAVED_NPC_NAMES");
    if (!names) {
        return std::unexpected(names.error());
    }
    auto npc_names = SavedNpcNames::decode(*names);
    if (!npc_names) {
        return std::unexpected(npc_names.error());
    }
    out.saved_npc_names = std::move(*npc_names);
    if (in.remaining() != 0) {
        return std::unexpected(PersistError::bad_section);
    }
    return out;
}

std::expected<std::vector<std::byte>, PersistError> EntitiesSection::encode() const
{
    auto saved = saved_entities.encode();
    if (!saved) {
        return std::unexpected(saved.error());
    }
    PersistWriter out;
    if (head) {
        out.tag("");
        out.u32(head->format_version);
        out.tag("");
        out.u64(head->current_unique_id);
    }
    out.section("SAVED_ENTITIES", *saved);
    out.section("SAVED_NPC_NAMES", saved_npc_names.encode());
    return out.take();
}

} // namespace fable::save
