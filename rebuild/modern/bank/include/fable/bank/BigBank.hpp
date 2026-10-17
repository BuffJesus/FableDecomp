#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <expected>
#include <memory>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace fable::bank {

enum class BigError {
    bad_magic,          // not "BIGB"
    truncated,          // a field or region runs past the end of the file
    footer_out_of_range,
    bad_toc,            // a TOC record does not parse, or its parsed length differs from the footer's size
    payload_out_of_range,
    overlap,            // two regions claim the same bytes (encode: a layout the writer cannot place)
    too_large,          // an offset or size no longer fits the 32-bit fields
};

// An entry's payload: a view into the bank file it was read from, or bytes that replace it.
// Keeping the view avoids copying the large banks (textures.big is 533 MB) to rewrite them.
class Payload final {
public:
    Payload() = default;
    Payload(std::shared_ptr<const std::vector<std::byte>> source, std::size_t offset, std::size_t size)
        : source_(std::move(source)), offset_(offset), size_(size) {}
    explicit Payload(std::vector<std::byte> bytes)
        : owned_(std::make_shared<const std::vector<std::byte>>(std::move(bytes))), size_(owned_->size()) {}

    [[nodiscard]] std::span<const std::byte> bytes() const noexcept
    {
        if (owned_) {
            return *owned_;
        }
        if (source_) {
            return std::span<const std::byte>(*source_).subspan(offset_, size_);
        }
        return {};
    }
    [[nodiscard]] std::size_t size() const noexcept { return size_; }

    friend bool operator==(const Payload& a, const Payload& b)
    {
        const auto x = a.bytes();
        const auto y = b.bytes();
        return x.size() == y.size() && std::equal(x.begin(), x.end(), y.begin());
    }

private:
    std::shared_ptr<const std::vector<std::byte>> source_;
    std::shared_ptr<const std::vector<std::byte>> owned_;
    std::size_t offset_ = 0;
    std::size_t size_ = 0;
};

// One TOC record. Strings are kept as their raw on-disk bytes: retail length prefixes include a
// trailing NUL, entries added by later tools may not, and the engine accepts both.
struct BankEntry {
    std::uint32_t magic = 42;
    std::uint32_t id = 0;
    std::int32_t type = 0;
    std::uint32_t crc = 0;          // a per-format class fingerprint, not a payload hash (BIG.md §8.1)
    std::vector<std::byte> name;    // raw, as stored after its u32 length
    std::uint32_t timestamp = 0;
    std::vector<std::vector<std::byte>> deps;
    std::vector<std::byte> info;
    Payload payload;
    // Where the payload sat in the file it was read from. The writer keeps regions in this
    // order and on the alignment boundary they had (BIG.md §9.1). For an empty payload this is
    // the stored offset field, written back unchanged.
    std::uint64_t original_offset = 0;

    [[nodiscard]] std::string_view name_text() const noexcept;
    friend bool operator==(const BankEntry&, const BankEntry&) = default;
};

struct SubBank {
    std::string name;
    std::uint32_t version = 0;
    std::uint32_t align = 1;
    // (type, count) pairs at the head of the TOC; nullopt when the TOC has none.
    std::optional<std::vector<std::pair<std::uint32_t, std::uint32_t>>> stats;
    std::vector<BankEntry> entries;
    std::uint64_t original_toc_offset = 0;

    friend bool operator==(const SubBank&, const SubBank&) = default;
};

// A `.big` bank archive (BIG.md §2): 16-byte header, payloads and per-sub-bank TOCs in any
// address order, then the sub-bank directory (footer).
struct BigBank {
    std::uint32_t version = 100;
    std::vector<SubBank> banks;
    std::uint64_t original_footer_offset = 0;

    friend bool operator==(const BigBank&, const BigBank&) = default;

    // `file` is kept alive by the payload views.
    [[nodiscard]] static std::expected<BigBank, BigError> decode(std::shared_ptr<const std::vector<std::byte>> file);

    // Re-lays every region in its original address order with the minimal-repack rule, so an
    // unmodified bank re-encodes byte for byte and a resized payload ripples only what follows it.
    [[nodiscard]] std::expected<std::vector<std::byte>, BigError> encode() const;
};

} // namespace fable::bank
