#include <fable/compat/retail_bitset.hpp>

#include <bitset>
#include <cstdint>
#include <cstring>
#include <iostream>

int main()
{
    int failures = 0;
    auto expect = [&](bool ok, const char* what) {
        if (!ok) {
            std::cerr << "FAIL: " << what << '\n';
            ++failures;
        }
    };
    fable::compat::retail_bitset<274> bits;
    expect(sizeof(bits) == 36, "274 bits take VC7.1's 36 bytes");
    bits.set(0);
    bits.set(33);
    bits.set(273);
    expect(bits.test(0) && bits.test(33) && bits.test(273) && !bits.test(1), "set/test");
    std::uint32_t words[9];
    std::memcpy(words, &bits, sizeof(words));
    expect(words[0] == 1 && words[1] == 2 && words[8] == (1U << 17), "bit i lives in 32-bit word i/32");
    bits.reset(33);
    expect(!bits[33] && bits.count() == 2, "reset/count");
    bits[5] = true;
    expect(bits.test(5), "reference assignment");
    bits.set();
    expect(bits.count() == 274, "set() fills exactly N bits");
    // Same observable behaviour as std::bitset for the operations the engine uses.
    std::bitset<274> reference;
    fable::compat::retail_bitset<274> mine;
    for (std::size_t i = 0; i < 274; i += 7) {
        reference.set(i);
        mine.set(i);
    }
    bool same = true;
    for (std::size_t i = 0; i < 274; ++i) {
        same = same && reference.test(i) == mine.test(i);
    }
    expect(same && reference.count() == mine.count(), "matches std::bitset");
    if (failures == 0) {
        std::cout << "fable_compat: all tests passed\n";
    }
    return failures == 0 ? 0 : 1;
}
