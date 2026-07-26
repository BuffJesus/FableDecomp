#include <cstddef>
#include <cstdint>

class CGameScriptInterface;
class CScriptThing;
class CThing;
class CTCCarrying;

class CThingMeleeOverlay
{
public:
    std::byte m_Pad00[0x91];
    std::uint8_t m_Flags91; // 0x91
};

static_assert(offsetof(CThingMeleeOverlay, m_Flags91) == 0x91);

class CTCCarrying
{
public:
    static bool IsThingCarryingMeleeWeapon(CThing* pThing);
};

bool CGameScriptInterface::IsEntityWieldingMeleeWeapon(const CScriptThing& scriptThing) const
{
    using RawScriptThingCall = CThing* (__thiscall*)(const CScriptThing*);

    const auto* const vtable = *reinterpret_cast<RawScriptThingCall* const*>(&scriptThing);
    CThing* const pThing = vtable[0x2c / sizeof(void*)](&scriptThing);
    if (pThing != nullptr)
    {
        const auto* const pThingOverlay = reinterpret_cast<const CThingMeleeOverlay*>(pThing);
        if ((static_cast<std::uint8_t>(pThingOverlay->m_Flags91) & 0x01u) == 0)
        {
            return CTCCarrying::IsThingCarryingMeleeWeapon(pThing);
        }
    }

    return false;
}