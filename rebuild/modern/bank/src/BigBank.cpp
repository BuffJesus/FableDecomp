#include <fable/bank/BigBank.hpp>

#include <algorithm>
#include <array>
#include <limits>

namespace fable::bank {
namespace {

constexpr std::size_t header_size = 16;
constexpr std::uint32_t max_stats_count = 1000;   // EgoCore's rewind rule (BIG.md §2)

class Reader final {
public:
    Reader(std::span<const std::byte> bytes, std::size_t pos) : bytes_(bytes), pos_(pos) {}

    [[nodiscard]] std::size_t position() const noexcept { return pos_; }
    void seek(std::size_t pos) noexcept { pos_ = pos; }

    bool u32(std::uint32_t& value)
    {
        if (bytes_.size() < 4 || pos_ > bytes_.size() - 4) {
            return false;
        }
        value = 0;
        for (int i = 0; i < 4; ++i) {
            value |= std::to_integer<std::uint32_t>(bytes_[pos_ + static_cast<std::size_t>(i)]) << (8 * i);
        }
        pos_ += 4;
        return true;
    }

    bool raw(std::size_t count, std::vector<std::byte>& out)
    {
        if (count > bytes_.size() - pos_) {
            return false;
        }
        out.assign(bytes_.begin() + static_cast<std::ptrdiff_t>(pos_),
                   bytes_.begin() + static_cast<std::ptrdiff_t>(pos_ + count));
        pos_ += count;
        return true;
    }

    // u32 length, then that many bytes.
    bool counted(std::vector<std::byte>& out)
    {
        std::uint32_t length = 0;
        return u32(length) && raw(length, out);
    }

    bool cstring(std::string& out)
    {
        for (std::size_t i = pos_; i < bytes_.size(); ++i) {
            if (bytes_[i] == std::byte{0}) {
                out.assign(reinterpret_cast<const char*>(bytes_.data()) + pos_, i - pos_);
                pos_ = i + 1;
                return true;
            }
        }
        return false;
    }

private:
    std::span<const std::byte> bytes_;
    std::size_t pos_ = 0;
};

class Writer final {
public:
    void u32(std::uint32_t value)
    {
        for (int i = 0; i < 4; ++i) {
            bytes_.push_back(static_cast<std::byte>((value >> (8 * i)) & 0xFFU));
        }
    }
    void raw(std::span<const std::byte> bytes) { bytes_.insert(bytes_.end(), bytes.begin(), bytes.end()); }
    void counted(std::span<const std::byte> bytes)
    {
        u32(static_cast<std::uint32_t>(bytes.size()));
        raw(bytes);
    }
    [[nodiscard]] std::vector<std::byte> take() noexcept { return std::move(bytes_); }

private:
    std::vector<std::byte> bytes_;
};

struct FooterRecord {
    std::string name;
    std::uint32_t version = 0;
    std::uint32_t entry_count = 0;
    std::uint32_t toc_offset = 0;
    std::uint32_t toc_size = 0;
    std::uint32_t align = 0;
};

// One TOC, with each payload's offset taken from `offsets` (index-parallel to the entries).
std::vector<std::byte> encode_toc(const SubBank& bank, const std::vector<std::uint64_t>& offsets)
{
    Writer out;
    if (bank.stats) {
        out.u32(static_cast<std::uint32_t>(bank.stats->size()));
        for (const auto& [type, count] : *bank.stats) {
            out.u32(type);
            out.u32(count);
        }
    }
    for (std::size_t i = 0; i < bank.entries.size(); ++i) {
        const BankEntry& entry = bank.entries[i];
        out.u32(entry.magic);
        out.u32(entry.id);
        out.u32(static_cast<std::uint32_t>(entry.type));
        out.u32(static_cast<std::uint32_t>(entry.payload.size()));
        out.u32(static_cast<std::uint32_t>(offsets[i]));
        out.u32(entry.crc);
        out.counted(entry.name);
        out.u32(entry.timestamp);
        out.u32(static_cast<std::uint32_t>(entry.deps.size()));
        for (const auto& dep : entry.deps) {
            out.counted(dep);
        }
        out.counted(entry.info);
    }
    return out.take();
}

// The largest power of two <= cap that divides x: the boundary the bank builder placed a region on.
std::uint64_t original_boundary(std::uint64_t x, std::uint64_t cap)
{
    std::uint64_t b = 1;
    while (b * 2 <= cap && x % (b * 2) == 0) {
        b *= 2;
    }
    return b;
}

std::uint64_t snap_up(std::uint64_t x, std::uint64_t boundary)
{
    return (x + boundary - 1) / boundary * boundary;
}

} // namespace

std::string_view BankEntry::name_text() const noexcept
{
    std::string_view text(reinterpret_cast<const char*>(name.data()), name.size());
    while (!text.empty() && text.back() == '\0') {
        text.remove_suffix(1);
    }
    return text;
}

std::expected<BigBank, BigError> BigBank::decode(std::shared_ptr<const std::vector<std::byte>> file)
{
    const std::span<const std::byte> bytes(*file);
    if (bytes.size() < header_size) {
        return std::unexpected(BigError::truncated);
    }
    if (bytes[0] != std::byte{'B'} || bytes[1] != std::byte{'I'} || bytes[2] != std::byte{'G'} || bytes[3] != std::byte{'B'}) {
        return std::unexpected(BigError::bad_magic);
    }
    Reader head(bytes, 4);
    BigBank out;
    std::uint32_t footer_offset = 0;
    std::uint32_t footer_size = 0;
    head.u32(out.version);
    head.u32(footer_offset);
    head.u32(footer_size);
    if (footer_offset < header_size || footer_offset > bytes.size() || footer_size > bytes.size() - footer_offset) {
        return std::unexpected(BigError::footer_out_of_range);
    }
    out.original_footer_offset = footer_offset;

    Reader footer(bytes.first(static_cast<std::size_t>(footer_offset) + footer_size), footer_offset);
    std::uint32_t bank_count = 0;
    if (!footer.u32(bank_count)) {
        return std::unexpected(BigError::truncated);
    }
    std::vector<FooterRecord> records;
    for (std::uint32_t i = 0; i < bank_count; ++i) {
        FooterRecord r;
        if (!footer.cstring(r.name) || !footer.u32(r.version) || !footer.u32(r.entry_count) || !footer.u32(r.toc_offset)
            || !footer.u32(r.toc_size) || !footer.u32(r.align)) {
            return std::unexpected(BigError::truncated);
        }
        records.push_back(std::move(r));
    }
    if (footer.position() != static_cast<std::size_t>(footer_offset) + footer_size) {
        return std::unexpected(BigError::footer_out_of_range);
    }

    for (const FooterRecord& r : records) {
        if (r.toc_offset > bytes.size() || r.toc_size > bytes.size() - r.toc_offset) {
            return std::unexpected(BigError::bad_toc);
        }
        const auto toc_end = static_cast<std::size_t>(r.toc_offset) + r.toc_size;
        Reader toc(bytes.first(toc_end), r.toc_offset);
        SubBank bank;
        bank.name = r.name;
        bank.version = r.version;
        bank.align = r.align;
        bank.original_toc_offset = r.toc_offset;

        std::uint32_t stats_count = 0;
        if (!toc.u32(stats_count)) {
            return std::unexpected(BigError::bad_toc);
        }
        if (stats_count < max_stats_count) {
            bank.stats.emplace();
            for (std::uint32_t i = 0; i < stats_count; ++i) {
                std::uint32_t type = 0;
                std::uint32_t count = 0;
                if (!toc.u32(type) || !toc.u32(count)) {
                    return std::unexpected(BigError::bad_toc);
                }
                bank.stats->emplace_back(type, count);
            }
        } else {
            toc.seek(r.toc_offset);
        }

        bank.entries.reserve(r.entry_count);
        for (std::uint32_t i = 0; i < r.entry_count; ++i) {
            BankEntry entry;
            std::uint32_t type = 0;
            std::uint32_t size = 0;
            std::uint32_t offset = 0;
            std::uint32_t dep_count = 0;
            if (!toc.u32(entry.magic) || !toc.u32(entry.id) || !toc.u32(type) || !toc.u32(size) || !toc.u32(offset)
                || !toc.u32(entry.crc) || !toc.counted(entry.name) || !toc.u32(entry.timestamp) || !toc.u32(dep_count)) {
                return std::unexpected(BigError::bad_toc);
            }
            if (dep_count > r.toc_size / 4) {
                return std::unexpected(BigError::bad_toc);
            }
            entry.deps.resize(dep_count);
            for (auto& dep : entry.deps) {
                if (!toc.counted(dep)) {
                    return std::unexpected(BigError::bad_toc);
                }
            }
            if (!toc.counted(entry.info)) {
                return std::unexpected(BigError::bad_toc);
            }
            entry.type = static_cast<std::int32_t>(type);
            entry.original_offset = offset;
            if (size != 0) {
                if (offset < header_size || offset > bytes.size() || size > bytes.size() - offset) {
                    return std::unexpected(BigError::payload_out_of_range);
                }
                entry.payload = Payload(file, offset, size);
            }
            bank.entries.push_back(std::move(entry));
        }
        if (toc.position() != toc_end) {
            return std::unexpected(BigError::bad_toc);
        }
        out.banks.push_back(std::move(bank));
    }
    return out;
}

std::expected<std::vector<std::byte>, BigError> BigBank::encode() const
{
    // Regions in original address order: each TOC, each non-empty payload, then the footer.
    struct Region {
        std::uint64_t original_start;
        std::size_t bank;
        std::size_t entry;   // npos for a TOC
        std::uint64_t size;
        std::uint64_t start = 0;
    };
    constexpr std::size_t toc_region = static_cast<std::size_t>(-1);

    std::uint64_t cap = 1;
    for (const SubBank& bank : banks) {
        cap = std::max<std::uint64_t>(cap, bank.align);
    }

    // TOC lengths do not depend on the offset values, so they can be sized before layout.
    std::vector<std::vector<std::uint64_t>> offsets(banks.size());
    std::vector<Region> regions;
    for (std::size_t b = 0; b < banks.size(); ++b) {
        offsets[b].resize(banks[b].entries.size());
        const auto toc_size = encode_toc(banks[b], offsets[b]).size();
        regions.push_back(Region{banks[b].original_toc_offset, b, toc_region, toc_size});
        for (std::size_t e = 0; e < banks[b].entries.size(); ++e) {
            const BankEntry& entry = banks[b].entries[e];
            offsets[b][e] = entry.original_offset;
            if (entry.payload.size() != 0) {
                regions.push_back(Region{entry.original_offset, b, e, entry.payload.size()});
            }
        }
    }
    std::stable_sort(regions.begin(), regions.end(),
                     [](const Region& x, const Region& y) { return x.original_start < y.original_start; });

    std::uint64_t cursor = header_size;
    for (Region& region : regions) {
        region.start = snap_up(cursor, original_boundary(region.original_start, cap));
        cursor = region.start + region.size;
        if (region.entry != toc_region) {
            offsets[region.bank][region.entry] = region.start;
        }
    }

    // The footer follows the last region (BIG.md §5: it follows the last TOC contiguously).
    Writer footer;
    footer.u32(static_cast<std::uint32_t>(banks.size()));
    std::vector<std::vector<std::byte>> tocs(banks.size());
    for (std::size_t b = 0; b < banks.size(); ++b) {
        tocs[b] = encode_toc(banks[b], offsets[b]);
    }
    for (std::size_t b = 0; b < banks.size(); ++b) {
        const auto toc = std::find_if(regions.begin(), regions.end(),
                                      [&](const Region& r) { return r.bank == b && r.entry == toc_region; });
        const SubBank& bank = banks[b];
        footer.raw(std::span(reinterpret_cast<const std::byte*>(bank.name.data()), bank.name.size()));
        footer.raw(std::array{std::byte{0}});
        footer.u32(bank.version);
        footer.u32(static_cast<std::uint32_t>(bank.entries.size()));
        footer.u32(static_cast<std::uint32_t>(toc->start));
        footer.u32(static_cast<std::uint32_t>(tocs[b].size()));
        footer.u32(bank.align);
    }
    const auto footer_bytes = footer.take();
    const std::uint64_t footer_offset = cursor;
    const std::uint64_t total = footer_offset + footer_bytes.size();
    if (total > std::numeric_limits<std::uint32_t>::max()) {
        return std::unexpected(BigError::too_large);
    }

    std::vector<std::byte> out(static_cast<std::size_t>(total), std::byte{0});
    Writer header;
    header.raw(std::array{std::byte{'B'}, std::byte{'I'}, std::byte{'G'}, std::byte{'B'}});
    header.u32(version);
    header.u32(static_cast<std::uint32_t>(footer_offset));
    header.u32(static_cast<std::uint32_t>(footer_bytes.size()));
    const auto header_bytes = header.take();
    std::copy(header_bytes.begin(), header_bytes.end(), out.begin());
    for (const Region& region : regions) {
        const auto bytes = region.entry == toc_region ? std::span<const std::byte>(tocs[region.bank])
                                                      : banks[region.bank].entries[region.entry].payload.bytes();
        std::copy(bytes.begin(), bytes.end(), out.begin() + static_cast<std::ptrdiff_t>(region.start));
    }
    std::copy(footer_bytes.begin(), footer_bytes.end(), out.begin() + static_cast<std::ptrdiff_t>(footer_offset));
    return out;
}

} // namespace fable::bank
