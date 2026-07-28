#include <cstddef>
#include <cstdint>

struct Unsigned16ArrayRange {
    std::uint32_t begin;
    std::uint32_t end;
};

static_assert(offsetof(Unsigned16ArrayRange, begin) == 0x0);
static_assert(offsetof(Unsigned16ArrayRange, end) == 0x4);
static_assert(sizeof(Unsigned16ArrayRange) == 0x8);

extern "C" int MemCmp_Unsigned16(std::uint32_t lhs, std::uint32_t rhs, int count);

std::uint32_t Compare_Unsigned16Array(const Unsigned16ArrayRange* param_1, const Unsigned16ArrayRange* param_2)
{
    int iVar1 = (static_cast<std::int32_t>(param_1->end) - static_cast<std::int32_t>(param_1->begin)) >> 1;
    if (iVar1 == ((static_cast<std::int32_t>(param_2->end) - static_cast<std::int32_t>(param_2->begin)) >> 1)) {
        iVar1 = MemCmp_Unsigned16(param_1->begin, param_2->begin, iVar1);
        if (iVar1 == 0) {
            return 1;
        }
    }
    return 0;
}