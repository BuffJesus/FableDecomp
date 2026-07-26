#include <cstddef>
#include <cstdint>

class CGameScriptInterface;
class CScriptThing;
class CThing;

namespace
{
struct CScriptThingVTableOverlay
{
    std::byte m_Pad00[0x2C];
    CThing* (__thiscall* GetThing)(const CScriptThing* pThis); // 0x2C
};

static_assert(offsetof(CScriptThingVTableOverlay, GetThing) == 0x2C);

struct CScriptThingOverlay
{
    const CScriptThingVTableOverlay* m_pVTable; // 0x00
};

static_assert(offsetof(CScriptThingOverlay, m_pVTable) == 0x00);

struct CThingOverlay
{
    std::byte m_Pad00[0x91];
    std::uint8_t m_Flags91; // 0x91
};

static_assert(offsetof(CThingOverlay, m_Flags91) == 0x91);
}

class CGameScriptInterface
{
public:
    bool __thiscall IsEntityInLimbo(const CScriptThing& entity) const
    {
        const auto* const entityOverlay = reinterpret_cast<const CScriptThingOverlay*>(&entity);
        CThing* const pThing = entityOverlay->m_pVTable->GetThing(&entity);

        if (pThing != nullptr)
        {
            const std::uint8_t flags91 =
                reinterpret_cast<const CThingOverlay*>(pThing)->m_Flags91;

            if ((flags91 & 0x01) == 0)
            {
                return ((flags91 >> 5) & 0x01) != 0;
            }
        }

        return false;
    }
};