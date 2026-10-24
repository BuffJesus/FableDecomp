#include <fable/save/Entities.hpp>
#include <fable/save/Profile.hpp>
#include <fable/save/Regions.hpp>
#include <fable/save/SaveBody.hpp>
#include <fable/save/SaveContainer.hpp>
#include <fable/save/SaveHeader.hpp>
#include <fable/save/TextSections.hpp>

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace {

using fable::save::ccrc;
using fable::save::SaveContainer;
using fable::save::SaveContainerCodec;
using fable::save::SaveContainerError;

int failures = 0;

void expect(const bool condition, const std::string_view message)
{
    if (!condition) {
        std::cerr << "FAIL: " << message << '\n';
        ++failures;
    }
}

void expect_error(const auto& result, const SaveContainerError error, const std::string_view message)
{
    expect(!result && result.error() == error, message);
}

std::vector<std::byte> bytes(const std::string_view text)
{
    std::vector<std::byte> out;
    for (const char c : text) {
        out.push_back(static_cast<std::byte>(c));
    }
    return out;
}

SaveContainer sample()
{
    // Shaped like a retail stream: repetitive tagged fields, so deflate has matches to find.
    std::vector<std::byte> body;
    for (std::uint32_t i = 0; i < 4000; ++i) {
        body.push_back(static_cast<std::byte>(i & 0xFFU));
        body.push_back(static_cast<std::byte>((i * 7U) & 0x3FU));
        body.push_back(std::byte{0x2A});
    }
    return SaveContainer{bytes("HEADER section"), body};
}

std::uint32_t u32_at(std::span<const std::byte> file, std::size_t offset)
{
    return static_cast<std::uint32_t>(file[offset]) | static_cast<std::uint32_t>(file[offset + 1]) << 8
        | static_cast<std::uint32_t>(file[offset + 2]) << 16 | static_cast<std::uint32_t>(file[offset + 3]) << 24;
}

void set_u32(std::vector<std::byte>& file, std::size_t offset, std::uint32_t value)
{
    for (int i = 0; i < 4; ++i) {
        file[offset + static_cast<std::size_t>(i)] = static_cast<std::byte>((value >> (8 * i)) & 0xffU);
    }
}

void empty_chunks_round_trip()
{
    const SaveContainer empty{{}, {}};
    const auto file = SaveContainerCodec::encode(empty);
    expect(file.has_value(), "empty chunks encode");
    if (file) {
        const auto decoded = SaveContainerCodec::decode(*file);
        expect(decoded && *decoded == empty, "empty zlib chunks round-trip");
    }
}

void extra_compressed_bytes_are_rejected()
{
    auto file = *SaveContainerCodec::encode(sample());
    const auto header_size = u32_at(file, 0x18);
    const auto total = u32_at(file, 0x0c);
    const auto insert_at = SaveContainerCodec::header_size + header_size;
    file.insert(file.begin() + static_cast<std::ptrdiff_t>(insert_at), std::byte{0x7f});
    file.resize(SaveContainerCodec::slab_size);
    set_u32(file, 0x18, header_size + 1);
    set_u32(file, 0x0c, total + 1);
    set_u32(file, total + 1, ccrc(std::span<const std::byte>(file).first(total + 1)));
    expect_error(SaveContainerCodec::decode(file), SaveContainerError::inflate_failed,
                 "a chunk must consume all of its declared compressed bytes");
}

void crc_matches_retail_ccrc()
{
    // Seed 0, no inversion: "123456789" is the classic check input; zlib's crc32 gives
    // 0xCBF43926 with pre/post inversion, so the raw seed-0 form differs from it.
    expect(ccrc(bytes("123456789")) == 0x2DFD2D88U, "seed-0 CRC of the check string");
    expect(ccrc(bytes("")) == 0U, "empty input keeps the seed");
    expect(ccrc(bytes("a"), 0xFFFFFFFFU) != ccrc(bytes("a")), "seed is honoured");
}

void encode_writes_retail_layout()
{
    const auto file = SaveContainerCodec::encode(sample());
    expect(file.has_value(), "sample encodes");
    if (!file) {
        return;
    }
    expect(file->size() == SaveContainerCodec::slab_size, "file is the 0x4B000 slab");
    expect(std::string_view(reinterpret_cast<const char*>(file->data()), 10) == "FableSave!", "magic");
    const std::uint32_t total = u32_at(*file, 0x0C);
    expect(u32_at(*file, 0x10) == sample().header_section.size(), "header size field");
    expect(u32_at(*file, 0x14) == sample().body_section.size(), "body size field");
    expect(std::to_integer<int>((*file)[0x1C]) == 0x78 && std::to_integer<int>((*file)[0x1D]) == 0xDA,
           "level-9 zlib stream header 78 DA");
    expect(u32_at(*file, total) == ccrc(std::span<const std::byte>(*file).first(total)), "trailer CRC");
}

void round_trip_is_exact()
{
    const auto file = SaveContainerCodec::encode(sample());
    const auto decoded = file ? SaveContainerCodec::decode(*file) : std::unexpected(SaveContainerError::deflate_failed);
    expect(decoded.has_value() && *decoded == sample(), "decode(encode(x)) == x");
    if (decoded) {
        const auto again = SaveContainerCodec::encode(*decoded);
        expect(again.has_value() && *again == *file, "encode is deterministic");
    }
}

void retail_gates_are_enforced()
{
    auto file = *SaveContainerCodec::encode(sample());
    const std::uint32_t total = u32_at(file, 0x0C);

    auto bad = file;
    bad[3] = std::byte{'X'};
    expect_error(SaveContainerCodec::decode(bad), SaveContainerError::bad_magic, "magic is checked");

    bad = file;
    bad[0x40] ^= std::byte{1};
    expect_error(SaveContainerCodec::decode(bad), SaveContainerError::bad_signature, "CRC covers the data");

    bad = file;
    bad.push_back(std::byte{0});
    expect_error(SaveContainerCodec::decode(bad), SaveContainerError::bad_slab_size, "exact slab size");

    bad = file;
    bad[total + 8] = std::byte{1};
    expect_error(SaveContainerCodec::decode(bad), SaveContainerError::nonzero_padding, "zero padding");

    bad = file;
    bad[0x0C] = std::byte{0xFF};
    bad[0x0D] = std::byte{0xFF};
    bad[0x0E] = std::byte{0xFF};
    expect_error(SaveContainerCodec::decode(bad), SaveContainerError::data_length_out_of_range,
                 "total_data_len bounds");

    expect_error(SaveContainerCodec::decode(std::span(file).first(8)), SaveContainerError::truncated_header,
                 "short input");
}

void inflated_sizes_are_checked()
{
    auto file = *SaveContainerCodec::encode(sample());
    const std::uint32_t total = u32_at(file, 0x0C);
    // Claim a larger body, then re-sign so only the size check can catch it.
    file[0x14] = static_cast<std::byte>(std::to_integer<unsigned>(file[0x14]) + 1);
    const std::uint32_t crc = ccrc(std::span<const std::byte>(file).first(total));
    for (int i = 0; i < 4; ++i) {
        file[total + static_cast<std::size_t>(i)] = static_cast<std::byte>((crc >> (8 * i)) & 0xFFU);
    }
    expect_error(SaveContainerCodec::decode(file), SaveContainerError::inflated_size_mismatch,
                 "header size fields must match the streams");

    // A tiny valid stream cannot justify allocating a header's maximum u32 size.
    file = *SaveContainerCodec::encode(sample());
    set_u32(file, 0x14, 0xffffffffU);
    set_u32(file, total, ccrc(std::span<const std::byte>(file).first(total)));
    expect_error(SaveContainerCodec::decode(file), SaveContainerError::inflated_size_mismatch,
                 "oversized output claims fail without upfront allocation");

    file = *SaveContainerCodec::encode(sample());
    set_u32(file, 0x14, 0);
    set_u32(file, total, ccrc(std::span<const std::byte>(file).first(total)));
    expect_error(SaveContainerCodec::decode(file), SaveContainerError::inflated_size_mismatch,
                 "nonempty streams cannot overflow a zero-byte output declaration");
}

void oversized_saves_are_refused()
{
    // Incompressible data larger than the slab cannot be written as a retail save.
    std::vector<std::byte> noise(SaveContainerCodec::slab_size);
    std::uint32_t x = 0x12345678U;
    for (auto& b : noise) {
        x = x * 1664525U + 1013904223U;
        b = static_cast<std::byte>(x >> 24);
    }
    expect_error(SaveContainerCodec::encode(SaveContainer{{}, noise}), SaveContainerError::too_large,
                 "slab overflow is an error, not a truncated file");
}

void header_round_trips()
{
    using fable::save::PersistError;
    using fable::save::SaveHeader;

    SaveHeader full;
    full.world_name = "FinalAlbion";
    full.world_frame = 138230;
    SaveHeader::Details d;
    d.saving_enabled = true;
    d.mini_map_enabled = true;
    d.most_recent_save_type = 1;
    d.most_recent_manual_save_name = u"Manual - Save1";
    d.save_game_marker_pos = {3114.79F, 3000.5F, -12.25F};
    d.current_region_name = "TXT_REGION_GREATWOOD_TELEPORT";
    d.current_region_minimap_graphic_name = "MINIMAP_GREATWOOD";
    full.details = d;

    const auto bytes = full.encode();
    expect(std::string_view(reinterpret_cast<const char*>(bytes.data()), 7) == std::string_view("HEADER\0", 7),
           "section name");
    // First field tag right after the 4-byte section length: WorldName = a8de4f22 (SAVE.md §3.4).
    expect(u32_at(bytes, 11) == 0xA8DE4F22U, "WorldName tag");
    const auto back = SaveHeader::decode(bytes);
    expect(back.has_value() && *back == full, "full header round-trips");

    SaveHeader minimal{"FinalAlbion", 740, std::nullopt};
    const auto small = SaveHeader::decode(minimal.encode());
    expect(small.has_value() && *small == minimal, "minimal-context header (fields 0-1 only)");

    auto broken = bytes;
    broken[11] ^= std::byte{1};
    const auto bad = SaveHeader::decode(broken);
    expect(!bad && bad.error() == PersistError::tag_mismatch, "tags are checked like retail CheckCRC");
}

void body_round_trips()
{
    using fable::save::PersistError;
    using fable::save::PlayerSection;
    using fable::save::SaveBody;

    const PlayerSection player{0xFFFFFF0000003AE3ULL, "Greatwood"};
    const auto player_bytes = player.encode();
    expect(u32_at(player_bytes, 0) == 0x96ADC827U, "PlayerCharacterUID tag (SAVE.md §3.5)");
    const auto player_back = PlayerSection::decode(player_bytes);
    expect(player_back.has_value() && *player_back == player, "PLAYER round-trips");

    SaveBody full;
    full.clock = SaveBody::GameClock{0.393F, 0.000666F, 5};
    full.sections = {{"ENTITIES", bytes("entity stream")}, {"PLAYER", player_bytes},
                     {"QUESTS", bytes("q")}, {"REGIONS", {}}, {"FACTIONS", bytes("f")}};
    const auto encoded = full.encode();
    expect(u32_at(encoded, 0) == 0xEEE2E867U, "body opens with the Time tag");
    const auto back = SaveBody::decode(encoded);
    expect(back.has_value() && *back == full, "full body round-trips");
    expect(back && back->find("PLAYER") != nullptr && back->find("PLAYER")->bytes == player_bytes, "find");

    SaveBody minimal;
    minimal.sections = {{"ENTITIES", bytes("only")}};
    const auto small = SaveBody::decode(minimal.encode());
    expect(small.has_value() && *small == minimal && !small->clock, "minimal body has no clock");
    expect(full.has_retail_shape() && minimal.has_retail_shape(), "both writer shapes are retail");
    SaveBody renamed = full;
    renamed.sections[1].name = "LAYER";   // what a one-byte-short ENTITIES length looks like
    expect(!renamed.has_retail_shape(), "unexpected section names are flagged");

    auto truncated = encoded;
    truncated.pop_back();
    const auto bad = SaveBody::decode(truncated);
    expect(!bad && bad.error() == PersistError::bad_section, "sections must tile the stream");
}

void entities_round_trip()
{
    using fable::save::EntitiesSection;
    using fable::save::EntityCell;
    using fable::save::SavedEntities;

    SavedEntities saved;
    saved.slots.emplace_back(std::nullopt);                          // empty slot: u64 0
    saved.slots.emplace_back(EntityCell{0x27A89, sample().body_section, {}});
    saved.slots.emplace_back(EntityCell{0x1832A3, {}, {}});           // record with no stream
    saved.dead_entities.push_back({0x027A89000000045CULL, 0x321ACEE7U});

    const auto bytes_out = saved.encode();
    expect(bytes_out.has_value(), "SAVED_ENTITIES encodes");
    if (!bytes_out) {
        return;
    }
    expect(u32_at(*bytes_out, 4) == 3, "slot count");
    expect(u32_at(*bytes_out, 12) == 0 && u32_at(*bytes_out, 16) == 0, "empty slot is u64 0");
    const std::uint32_t clen = u32_at(*bytes_out, 36);
    expect(u32_at(*bytes_out, 20) == 29 + clen, "record length = 29 + compressed length");
    expect(std::to_integer<int>((*bytes_out)[48]) == 0x78 && std::to_integer<int>((*bytes_out)[49]) == 0x01,
           "cells are zlib level 1 (78 01)");
    const auto back = SavedEntities::decode(*bytes_out);
    expect(back.has_value() && *back == saved, "SAVED_ENTITIES round-trips");

    fable::save::SavedNpcNames names;
    names.entries.emplace();
    names.entries->push_back({0x3020DF0000002903ULL, "", u"Brom"});
    names.entries->push_back({0x3020DF000000290BULL, "GROUP", u"Theresa"});
    const auto names_bytes = names.encode();
    expect(u32_at(names_bytes, 0) == 2 && u32_at(names_bytes, 12) == 10,
           "an empty NPC group is written as the 10-byte UNATTACHED");
    const auto names_back = fable::save::SavedNpcNames::decode(names_bytes);
    expect(names_back.has_value() && *names_back == names, "SAVED_NPC_NAMES round-trips");
    expect(fable::save::SavedNpcNames{}.encode().empty(), "a minimal-context NPC name section is empty");

    EntitiesSection entities{fable::save::EntitiesHead{2, 0xDE}, saved, names};
    const auto section = entities.encode();
    const auto entities_back = section ? EntitiesSection::decode(*section) : std::unexpected(fable::save::PersistError::truncated);
    expect(entities_back.has_value() && *entities_back == entities, "ENTITIES round-trips with its head");
}

void text_sections_round_trip()
{
    using fable::save::PersistReader;
    using fable::save::PersistWriter;

    // A hand-assembled LZO1X stream: initial run "abc" (0x14), an M2 match of 8 at distance 3
    // (0xE8 0x00), end of stream (0x11 0x00 0x00). Retail frames it as size, i16 clen, stream,
    // then the last 3 bytes raw.
    const auto blob = bytes(std::string_view("\x0E\x00\x00\x00\x09\x00\x14" "abc\xE8\x00\x11\x00\x00" "XYZ", 18));
    PersistReader in(blob);
    const auto text = fable::save::read_lz_text(in);
    expect(text.has_value() && *text == "abcabcabcabXYZ" && in.remaining() == 0, "LZO1X blob decodes");

    std::string noise;
    std::uint32_t seed = 12345;
    for (int i = 0; i < 70000; ++i) {
        seed = seed * 1103515245U + 12345U;
        noise.push_back(static_cast<char>('a' + (seed >> 16) % 26));
    }
    for (const std::string& sample : {std::string(), std::string("ab"), std::string("abc"),
                                      std::string("Name \"FACTION_HERO\";\nNumberOfAllies 0;\nEndFaction\n"),
                                      std::string(5000, 'q'), noise}) {
        PersistWriter out;
        fable::save::write_lz_text(out, sample);
        const auto written = out.take();
        PersistReader back(written);
        const auto again = fable::save::read_lz_text(back);
        expect(again.has_value() && *again == sample && back.remaining() == 0, "LZ text round-trips");
    }

    fable::save::FactionsSection factions{{"Name \"FACTION_HERO\";\nEndFaction\n", ""}};
    expect(fable::save::field_tag("NumberOfFactions") == 0x4F0E45C3U, "FACTIONS head tag is NumberOfFactions");
    const auto factions_back = fable::save::FactionsSection::decode(factions.encode());
    expect(factions_back.has_value() && *factions_back == factions, "FACTIONS round-trips");
    fable::save::QuestsSection quests{"START_QUEST_CARDS\nEND_QUEST_CARDS\n",
                                      {{"Q_TraderEscort", 3, false, false}, {"Q_TraderEscort", 1, true, false}}};
    const auto quests_bytes = quests.encode();
    const auto quests_back = fable::save::QuestsSection::decode(quests_bytes);
    expect(quests_back.has_value() && *quests_back == quests, "QUESTS round-trips with its boasts");
    // Retail Manual-Save1 starts its tail with the NumAcceptedBoasts tag 0xA4D9A09F.
    expect(fable::save::field_tag("NumAcceptedBoasts") == 0xA4D9A09FU, "boast tags are seed-0 field tags");
}

void regions_round_trip()
{
    fable::save::RegionsSection regions{{{"LookoutPoint", false, true, true}, {"PicnicArea", true, false, true}}};
    const auto out = regions.encode();
    expect(u32_at(out, 0) == 0 && u32_at(out, 4) == 3, "REGIONS: tag(\"\") is 0 and the table size counts element 0");
    expect(out.size() == 8 + 2 * 20 + 12 + 10, "REGIONS: 20 bytes per record beyond the name");
    const auto back = fable::save::RegionsSection::decode(out);
    expect(back.has_value() && *back == regions, "REGIONS round-trips");
}

void profile_round_trips()
{
    using fable::save::Profile;
    using fable::save::ProfileError;

    std::string text = "ProfileName \"Cornelio\";\r\nSubtitlingEnabled TRUE;\r\n";
    for (int slot = 1; slot <= 50; ++slot) {
        text += "SaveGameNames" + std::to_string(slot) + (slot == 1 ? " \"Manual - Save1\";\r\n" : " \"\";\r\n");
    }
    std::vector<std::byte> file = bytes("FablePro");
    file.resize(16, std::byte{0});
    const auto body = bytes(text);
    file.insert(file.end(), body.begin(), body.end());
    const auto total = static_cast<std::uint32_t>(file.size());
    for (int i = 0; i < 4; ++i) {
        file[12 + static_cast<std::size_t>(i)] = static_cast<std::byte>((total >> (8 * i)) & 0xFFU);
    }
    const std::uint32_t crc = ccrc(file);
    for (int i = 0; i < 4; ++i) {
        file.push_back(static_cast<std::byte>((crc >> (8 * i)) & 0xFFU));
    }

    auto profile = Profile::decode(file);
    expect(profile.has_value(), "profile decodes");
    if (!profile) {
        return;
    }
    expect(profile->encode() == file, "profile re-encodes byte for byte");
    expect(profile->value("SubtitlingEnabled") == "TRUE", "raw values");
    const auto names = profile->save_game_names();
    expect(names.size() == 50 && names[0] == "Manual - Save1" && names[1].empty(), "save registry");

    expect(profile->set_value("SaveGameNames2", "\"Manual - Save2\""), "registry edit");
    const auto edited = Profile::decode(profile->encode());
    expect(edited && edited->save_game_names()[1] == "Manual - Save2", "edited profile re-signs and reloads");

    auto broken = file;
    broken[20] ^= std::byte{1};
    const auto bad = Profile::decode(broken);
    expect(!bad && bad.error() == ProfileError::bad_signature, "profile trailer is enforced");
}

} // namespace

int main()
{
    empty_chunks_round_trip();
    extra_compressed_bytes_are_rejected();
    profile_round_trips();
    header_round_trips();
    body_round_trips();
    entities_round_trip();
    text_sections_round_trip();
    regions_round_trip();
    crc_matches_retail_ccrc();
    encode_writes_retail_layout();
    round_trip_is_exact();
    retail_gates_are_enforced();
    inflated_sizes_are_checked();
    oversized_saves_are_refused();
    if (failures == 0) {
        std::cout << "fable_save: all tests passed\n";
    }
    return failures == 0 ? 0 : 1;
}
