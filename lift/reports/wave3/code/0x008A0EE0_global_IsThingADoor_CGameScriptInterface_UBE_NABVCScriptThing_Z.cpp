#include <cstddef>
#include <cstdint>

class CGameScriptInterface;
class CScriptThing;
class CThing;

namespace
{
struct CThing_IsThingADoor_Overlay
{
    std::byte m_Pad00[0x24];
    std::uint8_t m_Flags24; // 0x24
    std::byte m_Pad25[0x6C];
    std::uint8_t m_Flags91; // 0x91
};

static_assert(offsetof(CThing_IsThingADoor_Overlay, m_Flags24) == 0x24);
static_assert(offsetof(CThing_IsThingADoor_Overlay, m_Flags91) == 0x91);

using CScriptThing_RawGetThingFn = CThing* (__thiscall*)(const CScriptThing*);
}

bool __thiscall CGameScriptInterface::IsThingADoor(const CScriptThing& scriptThing) const
{
    const auto* const vtable =
        *reinterpret_cast<CScriptThing_RawGetThingFn* const*>(&scriptThing);
    CThing* const pThing = vtable[0x2C / sizeof(void*)](&scriptThing);

    if (pThing != nullptr)
    {
        const auto* const pThingOverlay =
            reinterpret_cast<const CThing_IsThingADoor_Overlay*>(pThing);

        if (((pThingOverlay->m_Flags91 & 0x01u) == 0u) &&
            ((pThingOverlay->m_Flags24 & 0x40u) != 0u))
        {
            return true;
        }
    }

    return false;
}