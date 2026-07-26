#include <cstddef>
#include <cstdint>

class CGameScriptInterface;
class CScriptThing;
class CThing;

namespace
{
struct CThing_IsThingSmashable_Overlay
{
    std::byte m_Pad00[0x28];
    std::uint32_t m_Flags28; // 0x28
    std::byte m_Pad2C[0x65];
    std::uint8_t m_Flags91; // 0x91
};

static_assert(offsetof(CThing_IsThingSmashable_Overlay, m_Flags28) == 0x28);
static_assert(offsetof(CThing_IsThingSmashable_Overlay, m_Flags91) == 0x91);

using CScriptThing_RawGetThingFn = CThing* (__thiscall*)(const CScriptThing*);
}

bool __thiscall CGameScriptInterface::IsThingSmashable(const CScriptThing& scriptThing) const
{
    const auto* const vtable =
        *reinterpret_cast<CScriptThing_RawGetThingFn* const*>(&scriptThing);
    CThing* const pThing = vtable[0x2C / sizeof(void*)](&scriptThing);

    if ((pThing != nullptr) &&
        ((reinterpret_cast<const CThing_IsThingSmashable_Overlay*>(pThing)->m_Flags91 & 0x01) == 0) &&
        ((reinterpret_cast<const CThing_IsThingSmashable_Overlay*>(pThing)->m_Flags28 & 0x4000) != 0)) {
        return true;
    }

    return false;
}