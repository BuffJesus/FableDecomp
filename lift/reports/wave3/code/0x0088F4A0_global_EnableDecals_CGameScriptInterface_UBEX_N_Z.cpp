#include <cstddef>
#include <cstdint>

long __cdecl GFGetPreMainMemoryUsed();

class CGameScriptInterface
{
public:
    void __thiscall EnableDecals(bool enabled) const;
};

namespace
{
struct PreMainMemoryUsedOverlay
{
    std::byte pad00[0x1010];
    std::uint8_t decalsFlagByte_1010; // 0x1010
};

static_assert(offsetof(PreMainMemoryUsedOverlay, decalsFlagByte_1010) == 0x1010);
}

void __thiscall CGameScriptInterface::EnableDecals(bool enabled) const
{
    (void)this;

    auto* const preMainMemory =
        reinterpret_cast<PreMainMemoryUsedOverlay*>(GFGetPreMainMemoryUsed());

    const std::uint8_t current = preMainMemory->decalsFlagByte_1010;
    preMainMemory->decalsFlagByte_1010 =
        current ^ (((static_cast<std::uint8_t>(enabled) << 1) ^ current) & 2u);
}