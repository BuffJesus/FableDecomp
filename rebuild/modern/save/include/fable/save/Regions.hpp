#pragma once

#include <fable/save/PersistStream.hpp>

#include <cstddef>
#include <expected>
#include <span>
#include <string>
#include <vector>

namespace fable::save {

// One CRegion's saved state (the region manager's vector<CRegion>, 0x58-byte elements in retail).
struct Region {
    std::string name;                         // CRegion::Name
    bool appear_on_world_map = false;         // +0x54
    bool creature_generation_enabled = false; // +0x55
    bool sound_themes_enabled = false;        // +0x56

    friend bool operator==(const Region&, const Region&) = default;
};

// REGIONS (SaveGameState 0x4FDE80, per region 0x6BC290). Every field is Transfer("", ...), so
// every tag is field_tag("") == 0:
//   [tag][u32 table size]
//   (table size - 1) × { [tag][name\0] [tag][bool] [tag][bool] [tag][bool] }
// Element 0 of the engine's table is never written; `regions` holds elements 1 onwards, so the
// stored table size is regions.size() + 1.
struct RegionsSection {
    std::vector<Region> regions;

    friend bool operator==(const RegionsSection&, const RegionsSection&) = default;

    [[nodiscard]] static std::expected<RegionsSection, PersistError> decode(std::span<const std::byte> section);
    [[nodiscard]] std::vector<std::byte> encode() const;
};

} // namespace fable::save
