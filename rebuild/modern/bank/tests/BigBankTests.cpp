#include <fable/bank/BigBank.hpp>

#include <cstddef>
#include <cstdint>
#include <exception>
#include <iostream>
#include <memory>
#include <string_view>
#include <vector>

namespace {

using fable::bank::BankEntry;
using fable::bank::BigBank;
using fable::bank::Payload;
using fable::bank::SubBank;

int failures = 0;

void expect(const bool condition, const std::string_view message)
{
    if (!condition) {
        std::cerr << "FAIL: " << message << '\n';
        ++failures;
    }
}

std::vector<std::byte> bytes(const std::string_view text)
{
    std::vector<std::byte> out;
    for (const char c : text) {
        out.push_back(static_cast<std::byte>(c));
    }
    return out;
}

std::uint32_t u32_at(const std::vector<std::byte>& b, const std::size_t at)
{
    std::uint32_t v = 0;
    for (std::size_t i = 0; i < 4; ++i) {
        v |= std::to_integer<std::uint32_t>(b[at + i]) << (8 * i);
    }
    return v;
}

BankEntry entry(const std::uint32_t id, const std::string_view name, const std::string_view payload)
{
    BankEntry e;
    e.id = id;
    e.type = 1;
    e.crc = 0x1234;
    e.name = bytes(name);
    e.name.push_back(std::byte{0});   // retail length prefixes include the NUL
    e.deps.push_back(bytes("\\Dev\\source.tga"));
    e.info = bytes("INFO");
    if (!payload.empty()) {
        e.payload = Payload(bytes(payload));
    }
    return e;
}

BigBank sample()
{
    BigBank bank;
    SubBank text;
    text.name = "TEXT_ENGLISH_MAIN";
    text.version = 106;
    text.align = 1;
    text.stats = std::vector<std::pair<std::uint32_t, std::uint32_t>>{{1, 2}};
    text.entries.push_back(entry(1, "HELLO", "hello world"));
    text.entries.push_back(entry(2, "EMPTY", ""));
    SubBank tex;
    tex.name = "GBANK_MAIN_PC";
    tex.version = 1;
    tex.align = 16;
    tex.stats = std::vector<std::pair<std::uint32_t, std::uint32_t>>{{0, 1}};
    tex.entries.push_back(entry(7, "TEX", "pixels"));
    bank.banks = {text, tex};
    return bank;
}

void round_trip_is_exact()
{
    const auto first = sample().encode();
    expect(first.has_value(), "a new bank encodes");
    if (!first) {
        return;
    }
    expect(std::to_integer<char>((*first)[0]) == 'B' && u32_at(*first, 4) == 100, "BIGB header, version 100");
    const auto file = std::make_shared<const std::vector<std::byte>>(*first);
    const auto decoded = BigBank::decode(file);
    expect(decoded.has_value(), "the encoded bank decodes");
    if (!decoded) {
        return;
    }
    expect(decoded->banks.size() == 2 && decoded->banks[0].entries[0].name_text() == "HELLO", "names survive");
    expect(decoded->banks[0].entries[1].payload.size() == 0, "empty payloads stay empty");
    const auto again = decoded->encode();
    expect(again.has_value() && *again == *first, "decode then encode is byte-identical");
    for (const auto& sub : decoded->banks) {
        for (const auto& e : sub.entries) {
            expect(e.payload.size() == 0 || e.original_offset % 16 == 0, "regions sit on the file's largest alignment");
        }
    }
}

void resize_ripples_and_reverts()
{
    const auto file = std::make_shared<const std::vector<std::byte>>(*sample().encode());
    auto bank = *BigBank::decode(file);
    bank.banks[0].entries[0].payload = Payload(bytes("hello world, now longer than before"));
    const auto grown = bank.encode();
    const auto reread = BigBank::decode(std::make_shared<const std::vector<std::byte>>(*grown));
    expect(reread.has_value() && reread->banks[0].entries[0].payload == bank.banks[0].entries[0].payload,
           "the resized payload reads back");
    expect(reread.has_value() && reread->banks[1].entries[0].payload == Payload(bytes("pixels")),
           "the other payloads are unchanged");
    expect(reread.has_value() && reread->banks[1].entries[0].original_offset % 16 == 0, "rippled payloads stay aligned");
    auto reverted = *reread;
    reverted.banks[0].entries[0].payload = Payload(bytes("hello world"));
    const auto restored = reverted.encode();
    expect(restored.has_value() && *restored == *file, "grow then revert restores the original bytes");
}

void gates_are_enforced()
{
    auto bad = std::make_shared<std::vector<std::byte>>(*sample().encode());
    (*bad)[0] = std::byte{'X'};
    expect(BigBank::decode(bad).error() == fable::bank::BigError::bad_magic, "bad magic is rejected");
    auto short_file = std::make_shared<std::vector<std::byte>>(bytes("BIGB"));
    expect(!BigBank::decode(short_file).has_value(), "a truncated header is rejected");
    auto footer = std::make_shared<std::vector<std::byte>>(*sample().encode());
    (*footer)[8] = std::byte{0xFF};
    (*footer)[9] = std::byte{0xFF};
    (*footer)[10] = std::byte{0xFF};
    expect(BigBank::decode(footer).error() == fable::bank::BigError::footer_out_of_range, "a footer past the end is rejected");
}

void entry_counts_are_bounded_before_reserving()
{
    auto malformed = std::make_shared<std::vector<std::byte>>(*sample().encode());
    const std::size_t footer = u32_at(*malformed, 8);
    const std::size_t count_at = footer + 4 + sample().banks[0].name.size() + 1 + 4;
    for (std::size_t i = 0; i < 4; ++i) {
        (*malformed)[count_at + i] = std::byte{0xff};
    }
    try {
        const auto decoded = BigBank::decode(malformed);
        expect(!decoded && decoded.error() == fable::bank::BigError::bad_toc,
               "an impossible entry count is rejected before allocation");
    } catch (const std::exception&) {
        expect(false, "an impossible entry count must return bad_toc, not throw");
    }

    BigBank smallest;
    SubBank sub;
    sub.name = "EMPTY_FIELDS";
    sub.stats.emplace();
    sub.entries.emplace_back();
    smallest.banks.push_back(sub);
    const auto encoded = smallest.encode();
    const auto decoded = BigBank::decode(std::make_shared<const std::vector<std::byte>>(*encoded));
    expect(decoded && decoded->banks[0].entries.size() == 1,
           "a minimum-size forty-byte entry is still accepted");
    const auto null_file = BigBank::decode(nullptr);
    expect(!null_file && null_file.error() == fable::bank::BigError::truncated,
           "a null file is rejected as empty input");
}

} // namespace

int main()
{
    round_trip_is_exact();
    resize_ripples_and_reverts();
    gates_are_enforced();
    entry_counts_are_bounded_before_reserving();
    if (failures == 0) {
        std::cout << "fable_bank: all tests passed\n";
    }
    return failures == 0 ? 0 : 1;
}
