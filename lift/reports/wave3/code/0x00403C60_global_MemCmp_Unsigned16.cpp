#include <cstdint>

int __cdecl MemCmp_Unsigned16(
    std::uint16_t const* lhs,
    std::uint16_t const* rhs,
    std::uint32_t count)
{
    std::uint32_t index = 0;

    if (count != 0) {
        auto current = rhs;
        do {
            if (lhs[index] != *current) {
                return (-(static_cast<std::uint32_t>(lhs[index] < rhs[index])) & 0xFFFFFFFEu) + 1;
            }

            ++index;
            ++current;
        } while (index < count);
    }

    return 0;
}