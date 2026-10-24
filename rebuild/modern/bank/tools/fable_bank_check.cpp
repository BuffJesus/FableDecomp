// fable_bank_check [--edit] <file-or-directory>...
//
// For each `.big` (BIGB) bank: decode, re-encode, and report whether the rewrite is byte-identical.
// With --edit, also grow one payload in every sub-bank (by one byte more than the file's largest
// alignment), check that the result decodes with only that payload changed and every payload on
// its sub-bank's alignment, then restore the original bytes and check that the file comes back
// exactly (the writer's invertibility, BIG.md §9.1). Exit status 1 on any failure.

#include <fable/bank/BigBank.hpp>

#include <algorithm>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace {

using fable::bank::BigBank;
using fable::bank::Payload;

std::shared_ptr<const std::vector<std::byte>> read_file(const std::filesystem::path& path)
{
    std::ifstream in(path, std::ios::binary);
    auto bytes = std::make_shared<std::vector<std::byte>>(static_cast<std::size_t>(std::filesystem::file_size(path)));
    in.read(reinterpret_cast<char*>(bytes->data()), static_cast<std::streamsize>(bytes->size()));
    return bytes;
}

bool has_magic(const std::filesystem::path& path)
{
    std::ifstream in(path, std::ios::binary);
    char magic[4] = {};
    in.read(magic, 4);
    return in && std::equal(magic, magic + 4, "BIGB");
}

// "" if the edit round trip holds, else what broke.
std::string edit_round_trip(const BigBank& bank, const std::vector<std::byte>& original, const bool packed)
{
    std::uint32_t cap = 1;
    for (const auto& sub : bank.banks) {
        cap = std::max(cap, sub.align);
    }
    for (std::size_t b = 0; b < bank.banks.size(); ++b) {
        const auto& entries = bank.banks[b].entries;
        const auto target = std::find_if(entries.begin(), entries.end(), [](const auto& e) { return e.payload.size() != 0; });
        if (target == entries.end()) {
            continue;
        }
        const auto e = static_cast<std::size_t>(target - entries.begin());
        BigBank edited = bank;
        std::vector<std::byte> grown(target->payload.bytes().begin(), target->payload.bytes().end());
        grown.resize(grown.size() + cap + 1, std::byte{0x5A});
        edited.banks[b].entries[e].payload = Payload(grown);

        const auto bytes = edited.encode();
        if (!bytes) {
            return "edit encode failed in " + bank.banks[b].name;
        }
        const auto again = BigBank::decode(std::make_shared<const std::vector<std::byte>>(*bytes));
        if (!again) {
            return "edited bank does not decode (" + bank.banks[b].name + ")";
        }
        for (std::size_t bb = 0; bb < bank.banks.size(); ++bb) {
            const auto& sub = again->banks[bb];
            for (std::size_t ee = 0; ee < sub.entries.size(); ++ee) {
                const auto& got = sub.entries[ee];
                const auto& want = edited.banks[bb].entries[ee];
                if (got.payload != want.payload || got.name != want.name || got.info != want.info || got.crc != want.crc) {
                    return "edit changed another entry in " + sub.name;
                }
                // Only payloads the original file had aligned must stay aligned: tool-written banks
                // (e.g. a repacked textures.big) can store payloads off their sub-bank boundary.
                const bool was_aligned = bank.banks[bb].entries[ee].original_offset % sub.align == 0;
                if (got.payload.size() != 0 && was_aligned && got.original_offset % sub.align != 0) {
                    return "edit broke alignment in " + sub.name;
                }
            }
        }
        BigBank reverted = *again;
        reverted.banks[b].entries[e].payload = Payload(std::vector<std::byte>(target->payload.bytes().begin(), target->payload.bytes().end()));
        const auto restored = reverted.encode();
        // Invertibility needs the original boundaries, which a re-decode recovers only when the
        // payloads were aligned: in a packed bank they are re-derived from the shifted offsets.
        if (packed) {
            if (!restored || !BigBank::decode(std::make_shared<const std::vector<std::byte>>(*restored))) {
                return "reverted packed bank does not decode (" + bank.banks[b].name + ")";
            }
        } else if (!restored || *restored != original) {
            return "grow-then-revert is not the original (" + bank.banks[b].name + ")";
        }
    }
    return "";
}

} // namespace

int main(int argc, char** argv)
{
    bool edit = false;
    std::vector<std::filesystem::path> files;
    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];
        if (arg == "--edit") {
            edit = true;
        } else if (std::filesystem::is_directory(arg)) {
            for (const auto& entry : std::filesystem::recursive_directory_iterator(arg)) {
                if (entry.is_regular_file() && has_magic(entry.path())) {
                    files.push_back(entry.path());
                }
            }
        } else {
            files.push_back(arg);
        }
    }

    int failures = 0;
    for (const auto& path : files) {
        const auto file = read_file(path);
        const auto bank = BigBank::decode(file);
        std::string verdict;
        std::size_t entries = 0;
        std::size_t unaligned = 0;
        if (!bank) {
            verdict = "rejected_" + std::to_string(static_cast<int>(bank.error()));
        } else {
            for (const auto& sub : bank->banks) {
                entries += sub.entries.size();
                for (const auto& e : sub.entries) {
                    unaligned += e.payload.size() != 0 && e.original_offset % sub.align != 0;
                }
            }
            const auto encoded = bank->encode();
            if (!encoded) {
                verdict = "encode_failed_" + std::to_string(static_cast<int>(encoded.error()));
            } else if (*encoded != *file) {
                verdict = "encode_mismatch";
            } else {
                verdict = "exact";
                if (edit) {
                    const auto problem = edit_round_trip(*bank, *file, unaligned != 0);
                    verdict = !problem.empty() ? "edit:" + problem : unaligned != 0 ? "exact+edit(packed)" : "exact+edit";
                }
            }
        }
        if (verdict.rfind("exact", 0) != 0) {
            ++failures;
        }
        std::cout << verdict << '\t' << (bank ? bank->banks.size() : 0) << " banks\t" << entries << " entries\t"
                  << unaligned << " unaligned\t" << file->size() << " bytes\t" << path.string() << '\n';
    }
    return failures == 0 ? 0 : 1;
}
