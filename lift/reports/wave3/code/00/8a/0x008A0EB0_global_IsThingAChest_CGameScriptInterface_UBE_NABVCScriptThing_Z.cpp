#include <cstddef>
#include <cstdint>

class CGameScriptInterface;
class CScriptThing;
class CThing;

namespace
{
struct CThing_IsThingAChest_Overlay
{
    std::byte m_Pad00[0x24];
    std::uint32_t m_Flags24; // 0x24
    std::byte m_Pad28[0x69];
    std::uint8_t m_Flags91; // 0x91
};

static_assert(offsetof(CThing_IsThingAChest_Overlay, m_Flags24) == 0x24);
static_assert(offsetof(CThing_IsThingAChest_Overlay, m_Flags91) == 0x91);

using CScriptThing_RawGetThingFn = CThing* (__thiscall*)(const CScriptThing*);
}

bool __thiscall CGameScriptInterface::IsThingAChest(const CScriptThing& scriptThing) const
{
    const auto* const vtable =
        *reinterpret_cast<CScriptThing_RawGetThingFn* const*>(&scriptThing);
    CThing* const pThing = vtable[0x2C / sizeof(void*)](&scriptThing);

    if (pThing != nullptr)
    {
        const auto* const thingOverlay =
            reinterpret_cast<const CThing_IsThingAChest_Overlay*>(pThing);

        if (((thingOverlay->m_Flags91 & 0x01u) == 0) &&
            ((thingOverlay->m_Flags24 & 0x10000000u) != 0))
        {
            return true;
        }
    }

    return false;
}