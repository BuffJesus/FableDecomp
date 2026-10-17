// fable_save_check <file-or-directory>...
//
// For each `FableSave!` file: decode with the retail gates, re-encode, and report whether the
// rewrite is byte-identical to the original. `FablePro` profiles get the same check
// (`profile:exact`). Directories are walked recursively; files with other magics are skipped.
// Exit status 1 if any file fails to decode or re-encode.
//
//   exact      decode passed and encode(decode(file)) == file
//   sections   decode passed, sections round-trip, but the compressed bytes differ (the file was
//              written by a different deflate, e.g. zlib-ng in Python tools)
//   rejected   decode failed (retail would refuse to load it)
//
// The HEADER section (chunk0), the body's section list (chunk1), PLAYER and ENTITIES (down to
// the inflated SAVED_ENTITIES cells, which are re-deflated) are also decoded and re-encoded; a
// save where one of them does not reproduce byte for byte gets a `header:`, `body:`, `player:`
// or `entities:` verdict instead.
//
// REGIONS is decoded and must re-encode exactly. QUESTS and FACTIONS (LZO text blobs) are decoded too. A build with FABLE_SAVE_WITH_LZO must
// re-encode them byte for byte; without it they re-encode with the MIT encoder, and the check
// is that the result decodes back to the same text (`quests:` / `factions:` verdicts).

#include <fable/save/Entities.hpp>
#include <fable/save/Profile.hpp>
#include <fable/save/Regions.hpp>
#include <fable/save/SaveBody.hpp>
#include <fable/save/SaveContainer.hpp>
#include <fable/save/SaveHeader.hpp>
#include <fable/save/TextSections.hpp>

#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <string_view>
#include <vector>

namespace {

using fable::save::SaveContainerCodec;

// "" when the section is absent or re-encodes byte for byte, else the verdict.
template <typename Section>
std::string exact_section_verdict(const fable::save::SaveBody& body, std::string_view name, std::string_view prefix)
{
    const auto* raw = body.find(name);
    if (raw == nullptr) {
        return "";
    }
    const auto decoded = Section::decode(raw->bytes);
    if (!decoded) {
        return std::string(prefix) + "decode_error_" + std::to_string(static_cast<int>(decoded.error()));
    }
    return decoded->encode() == raw->bytes ? "" : std::string(prefix) + "encode_mismatch";
}

// "" when the text section is absent or re-encodes as this build should, else the verdict.
template <typename Section>
std::string text_section_verdict(const fable::save::SaveBody& body, std::string_view name, std::string_view prefix)
{
    const auto* raw = body.find(name);
    if (raw == nullptr) {
        return "";
    }
    const auto decoded = Section::decode(raw->bytes);
    if (!decoded) {
        return std::string(prefix) + "decode_error_" + std::to_string(static_cast<int>(decoded.error()));
    }
    const auto encoded = decoded->encode();
    if (fable::save::lzo_compressor_available() && encoded == raw->bytes) {
        return "";
    }
    const auto again = Section::decode(encoded);
    if (!again || *again != *decoded) {
        return std::string(prefix) + "round_trip_mismatch";
    }
    // Same text, different compressed bytes: not written by retail's lzo1x_999 (e.g. the Python
    // tools' store-mode streams), like a whole-file `sections` verdict.
    return fable::save::lzo_compressor_available() ? "sections" : "";
}

using fable::save::SaveContainerError;

std::string_view error_name(SaveContainerError error)
{
    switch (error) {
    case SaveContainerError::bad_magic: return "bad_magic";
    case SaveContainerError::truncated_header: return "truncated_header";
    case SaveContainerError::data_length_out_of_range: return "data_length_out_of_range";
    case SaveContainerError::bad_signature: return "bad_signature";
    case SaveContainerError::bad_slab_size: return "bad_slab_size";
    case SaveContainerError::nonzero_padding: return "nonzero_padding";
    case SaveContainerError::chunk_out_of_bounds: return "chunk_out_of_bounds";
    case SaveContainerError::chunk_layout_mismatch: return "chunk_layout_mismatch";
    case SaveContainerError::inflate_failed: return "inflate_failed";
    case SaveContainerError::inflated_size_mismatch: return "inflated_size_mismatch";
    case SaveContainerError::deflate_failed: return "deflate_failed";
    case SaveContainerError::too_large: return "too_large";
    }
    return "unknown";
}

std::vector<std::byte> read_file(const std::filesystem::path& path)
{
    std::ifstream in(path, std::ios::binary);
    const std::vector<char> raw{std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>()};
    std::vector<std::byte> out(raw.size());
    for (std::size_t i = 0; i < raw.size(); ++i) {
        out[i] = static_cast<std::byte>(raw[i]);
    }
    return out;
}

bool has_magic(const std::vector<std::byte>& file, std::string_view magic)
{
    if (file.size() < magic.size()) {
        return false;
    }
    for (std::size_t i = 0; i < magic.size(); ++i) {
        if (file[i] != static_cast<std::byte>(magic[i])) {
            return false;
        }
    }
    return true;
}

} // namespace

int main(int argc, char** argv)
{
    std::vector<std::filesystem::path> files;
    for (int i = 1; i < argc; ++i) {
        const std::filesystem::path arg = argv[i];
        if (std::filesystem::is_directory(arg)) {
            for (const auto& entry : std::filesystem::recursive_directory_iterator(arg)) {
                if (entry.is_regular_file()) {
                    files.push_back(entry.path());
                }
            }
        } else {
            files.push_back(arg);
        }
    }

    std::map<std::string, int> totals;
    for (const auto& path : files) {
        const auto file = read_file(path);
        if (has_magic(file, "FablePro")) {
            const auto profile = fable::save::Profile::decode(file);
            const std::string verdict = !profile ? "profile:rejected_" + std::to_string(static_cast<int>(profile.error()))
                : profile->encode() == file      ? "profile:exact"
                                                 : "profile:encode_mismatch";
            ++totals[verdict];
            if (verdict != "profile:exact") {
                std::cout << verdict << '\t' << path.string() << '\n';
            }
            continue;
        }
        if (!has_magic(file, "FableSave!")) {
            continue;
        }
        const auto decoded = SaveContainerCodec::decode(file);
        std::string verdict;
        if (!decoded) {
            verdict = "rejected:" + std::string(error_name(decoded.error()));
        } else if (const auto header = fable::save::SaveHeader::decode(decoded->header_section); !header) {
            verdict = "header:decode_error_" + std::to_string(static_cast<int>(header.error()));
        } else if (header->encode() != decoded->header_section) {
            verdict = "header:encode_mismatch";
        } else if (const auto body = fable::save::SaveBody::decode(decoded->body_section); !body) {
            verdict = "body:decode_error_" + std::to_string(static_cast<int>(body.error()));
        } else if (body->encode() != decoded->body_section) {
            verdict = "body:encode_mismatch";
        } else if (!body->has_retail_shape()) {
            verdict = "body:unexpected_sections";
        } else if (const auto* player = body->find("PLAYER");
                   player != nullptr && !fable::save::PlayerSection::decode(player->bytes)) {
            verdict = "player:decode_error";
        } else if (player != nullptr && fable::save::PlayerSection::decode(player->bytes)->encode() != player->bytes) {
            verdict = "player:encode_mismatch";
        } else if (const auto entities = fable::save::EntitiesSection::decode(body->find("ENTITIES")->bytes);
                   !entities) {
            verdict = "entities:decode_error_" + std::to_string(static_cast<int>(entities.error()));
        } else if (const auto again = entities->encode(); !again || *again != body->find("ENTITIES")->bytes) {
            verdict = "entities:encode_mismatch";
        } else if (verdict = exact_section_verdict<fable::save::RegionsSection>(*body, "REGIONS", "regions:");
                   !verdict.empty()) {
        } else if (verdict = text_section_verdict<fable::save::QuestsSection>(*body, "QUESTS", "quests:");
                   !verdict.empty()) {
        } else if (verdict = text_section_verdict<fable::save::FactionsSection>(*body, "FACTIONS", "factions:");
                   !verdict.empty()) {
        } else {
            const auto encoded = SaveContainerCodec::encode(*decoded);
            if (!encoded) {
                verdict = "encode_failed:" + std::string(error_name(encoded.error()));
            } else if (*encoded == file) {
                verdict = "exact";
            } else {
                const auto again = SaveContainerCodec::decode(*encoded);
                verdict = again && *again == *decoded ? "sections" : "encode_mismatch";
            }
        }
        ++totals[verdict];
        if (verdict != "exact") {
            std::cout << verdict << '\t' << path.string() << '\n';
        }
    }
    std::cout << "text blobs: " << (fable::save::lzo_compressor_available() ? "LZO byte-exact" : "MIT LZO1X encoder (valid, not byte-exact)")
              << '\n';
    int rejected = 0;
    for (const auto& [verdict, count] : totals) {
        std::cout << count << '\t' << verdict << '\n';
        if (verdict != "exact" && verdict != "sections" && verdict != "profile:exact") {
            rejected += count;
        }
    }
    return rejected == 0 ? 0 : 1;
}
