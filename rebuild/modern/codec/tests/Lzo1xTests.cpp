#include <fable/codec/Lzo1x.hpp>

#include <cstddef>
#include <cstdint>
#include <exception>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

namespace {

int failures = 0;

std::vector<std::byte> bytes(const std::string& s)
{
    std::vector<std::byte> out;
    for (const char c : s) {
        out.push_back(static_cast<std::byte>(c));
    }
    return out;
}

void round_trip(const std::vector<std::byte>& data, const char* what)
{
    const auto packed = fable::codec::lzo1x_compress(data);
    const auto back = fable::codec::lzo1x_decompress(packed, data.size());
    if (!back || *back != data) {
        std::cerr << "FAIL: round trip of " << what << '\n';
        ++failures;
    }
}

} // namespace

int main()
{
    // A missing end marker must not hide that a literal/match already exceeds
    // the declared output. Reject before expanding the offending token.
    for (const auto& stream : {
             std::vector<std::byte>{std::byte{0x13}, std::byte{'a'}, std::byte{'b'}},
             std::vector<std::byte>{std::byte{0x12}, std::byte{'a'}, std::byte{0x21}, std::byte{0}, std::byte{0}}}) {
        const auto result = fable::codec::lzo1x_decompress(stream, 1);
        if (result || result.error() != fable::codec::Lzo1xError::size_mismatch) {
            std::cerr << "FAIL: output limit is enforced before the next token\n";
            ++failures;
        }
    }
    try {
        const auto result = fable::codec::lzo1x_decompress({}, std::numeric_limits<std::size_t>::max());
        if (result || result.error() != fable::codec::Lzo1xError::truncated) {
            std::cerr << "FAIL: empty stream is truncated regardless of declared size\n";
            ++failures;
        }
    } catch (const std::exception&) {
        std::cerr << "FAIL: declared size alone must not allocate the output\n";
        ++failures;
    }
    round_trip({}, "empty input");
    round_trip(bytes("ab"), "two bytes");
    round_trip(bytes("START_QUEST_CARDS\nEND_QUEST_CARDS\nSTART_ACTIVE_QUESTS\nEND_ACTIVE_QUESTS\n"), "quest text");
    std::string repeated;
    for (int i = 0; i < 4000; ++i) {
        repeated += "Name \"FACTION_" + std::to_string(i % 29) + "\";\n";
    }
    round_trip(bytes(repeated), "repetitive text");
    std::vector<std::byte> noise;
    std::uint32_t seed = 7;
    for (int i = 0; i < 70000; ++i) {
        seed = seed * 1664525U + 1013904223U;
        noise.push_back(static_cast<std::byte>(seed >> 24));
    }
    round_trip(noise, "incompressible data");
    const auto packed = fable::codec::lzo1x_compress(bytes(repeated));
    if (packed.size() * 4 > repeated.size()) {
        std::cerr << "FAIL: repetitive text should compress at least 4x\n";
        ++failures;
    }
    if (failures == 0) {
        std::cout << "fable_codec: all tests passed\n";
    }
    return failures == 0 ? 0 : 1;
}
