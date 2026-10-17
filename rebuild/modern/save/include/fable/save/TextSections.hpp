#pragma once

#include <fable/save/PersistStream.hpp>

#include <cstddef>
#include <cstdint>
#include <expected>
#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace fable::save {

// A compressed persist-text blob, as QUESTS and FACTIONS store them:
//   u32 size
//   size == 0:  nothing more
//   otherwise:  i16 clen; clen == -1: u32 clen follows
//               clen <= 0: `size` raw bytes (retail's stored form)
//               clen > 0:  clen bytes LZO1X decoding to the first size - 3 bytes, then the
//                          last 3 bytes raw
// Writer 0x996610 / reader 0x995FE0. The reader takes clen as signed, so the writer stores at
// most 0x7FFF in 16 bits and uses -1 + u32 above that. It writes the stored form for size <= 3
// and when the compressed stream would not be shorter than size - 3.
[[nodiscard]] std::expected<std::string, PersistError> read_lz_text(PersistReader& in);

// Byte-exact with retail when the build links upstream LZO (lzo_compressor_available());
// otherwise compressed with the tree's MIT LZO1X encoder: a valid stream that decodes to the same
// text, but not retail's exact bytes. Falls back to the stored form if that is not smaller.
void write_lz_text(PersistWriter& out, std::string_view text);

[[nodiscard]] bool lzo_compressor_available() noexcept;

// One accepted boast, as the QUESTS section's tagged tail stores it.
struct AcceptedBoast {
    std::string script_name;   // BoastScriptName<i>: the quest the boast belongs to
    std::uint32_t id = 0;      // BoastId<i>
    bool failed = false;       // BoastFailed<i>
    bool completed = false;    // BoastCompleted<i>

    friend bool operator==(const AcceptedBoast&, const AcceptedBoast&) = default;
};

// QUESTS: the quest manager's persist text (START_QUEST_CARDS ... END_SAVED_QUESTS), then the
// accepted boasts as tagged binary fields: NumAcceptedBoasts, then per boast i the fields
// BoastScriptName<i>, BoastId<i>, BoastFailed<i>, BoastCompleted<i> (the index is part of the
// field name, so of its tag).
struct QuestsSection {
    std::string text;
    std::vector<AcceptedBoast> accepted_boasts;

    friend bool operator==(const QuestsSection&, const QuestsSection&) = default;

    [[nodiscard]] static std::expected<QuestsSection, PersistError> decode(std::span<const std::byte> section);
    [[nodiscard]] std::vector<std::byte> encode() const;
};

// FACTIONS: tagged NumberOfFactions (u32), then per faction i the tagged Faction<i> LZ text
// blob. Each text is one faction's `Name "FACTION_X"; NumberOfAllies n; Ally "..."; ...
// EndFaction`. The tags follow from the names, so only the texts are kept.
struct FactionsSection {
    std::vector<std::string> factions;

    friend bool operator==(const FactionsSection&, const FactionsSection&) = default;

    [[nodiscard]] static std::expected<FactionsSection, PersistError> decode(std::span<const std::byte> section);
    [[nodiscard]] std::vector<std::byte> encode() const;
};

} // namespace fable::save
