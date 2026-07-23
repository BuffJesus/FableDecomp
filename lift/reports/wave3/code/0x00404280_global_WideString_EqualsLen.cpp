#include <cstddef>
#include <cstdint>

extern int __cdecl MemCmp_Unsigned16(
    std::uint16_t const* lhs,
    std::uint16_t const* rhs,
    std::uint32_t count);

#pragma pack(push, 1)
struct WideStringRange
{
    std::uint16_t* begin;
    std::uint16_t* end;
};
#pragma pack(pop)

static_assert(offsetof(WideStringRange, begin) == 0x0);
static_assert(offsetof(WideStringRange, end) == 0x4);
static_assert(sizeof(WideStringRange) == 0x8);

int __cdecl WideString_EqualsLen(WideStringRange const* wideString, std::uint16_t const* text)
{
    int length = 0;
    std::uint16_t ch = *text;

    while (ch != 0) {
        length = length + 1;
        ch = text[length];
    }

    const std::int32_t byteSpan =
        static_cast<std::int32_t>(
            static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(wideString->end)) -
            static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(wideString->begin)));

    if (((byteSpan >> 1) == length) &&
        ((length = MemCmp_Unsigned16(wideString->begin, text, static_cast<std::uint32_t>(length))), length == 0)) {
        return 1;
    }

    return 0;
}