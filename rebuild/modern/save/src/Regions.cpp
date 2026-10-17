#include <fable/save/Regions.hpp>

namespace fable::save {

std::expected<RegionsSection, PersistError> RegionsSection::decode(std::span<const std::byte> section)
{
    PersistReader in(section);
    if (const auto tag = in.tag(""); !tag) {
        return std::unexpected(tag.error());
    }
    const auto table_size = in.u32();
    if (!table_size) {
        return std::unexpected(table_size.error());
    }
    // A world always has its unwritten element 0; each record is at least 4 tags, a NUL and 3 bools.
    if (*table_size == 0 || *table_size - 1 > in.remaining() / 20) {
        return std::unexpected(PersistError::bad_section);
    }
    RegionsSection out;
    out.regions.reserve(*table_size - 1);
    for (std::uint32_t i = 1; i < *table_size; ++i) {
        Region region;
        if (const auto tag = in.tag(""); !tag) {
            return std::unexpected(tag.error());
        }
        auto name = in.cstring();
        if (!name) {
            return std::unexpected(name.error());
        }
        region.name = std::move(*name);
        for (bool* flag : {&region.appear_on_world_map, &region.creature_generation_enabled,
                           &region.sound_themes_enabled}) {
            if (const auto tag = in.tag(""); !tag) {
                return std::unexpected(tag.error());
            }
            const auto value = in.boolean();
            if (!value) {
                return std::unexpected(value.error());
            }
            *flag = *value;
        }
        out.regions.push_back(std::move(region));
    }
    if (in.remaining() != 0) {
        return std::unexpected(PersistError::bad_section);
    }
    return out;
}

std::vector<std::byte> RegionsSection::encode() const
{
    PersistWriter out;
    out.tag("");
    out.u32(static_cast<std::uint32_t>(regions.size() + 1));
    for (const Region& region : regions) {
        out.tag("");
        out.cstring(region.name);
        for (const bool flag : {region.appear_on_world_map, region.creature_generation_enabled,
                                region.sound_themes_enabled}) {
            out.tag("");
            out.boolean(flag);
        }
    }
    return out.take();
}

} // namespace fable::save
