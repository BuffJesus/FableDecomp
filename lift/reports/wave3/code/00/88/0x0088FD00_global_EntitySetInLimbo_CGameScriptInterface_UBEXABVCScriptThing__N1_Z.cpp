#include <cstddef>
#include <cstdint>

class CScriptThing;
class CThing
{
public:
    void __thiscall SetInLimbo(bool in_limbo);
};

class CGameScriptInterface
{
public:
    void __thiscall EntitySetInLimbo(const CScriptThing& entity, bool in_limbo, bool param_3) const;
};

namespace
{
using CScriptThing_GetThingFn = void* (__thiscall*)(const CScriptThing*);

struct CScriptThing_VTable_Overlay
{
    std::byte m_Pad00[0x2C];
    CScriptThing_GetThingFn m_GetThing; // 0x2C
};

static_assert(offsetof(CScriptThing_VTable_Overlay, m_GetThing) == 0x2C);

struct CThingInLimbo_Overlay
{
    std::byte m_Pad00[0x91];
    std::uint8_t m_Byte91; // 0x91
    std::uint8_t m_Byte92; // 0x92
};

static_assert(offsetof(CThingInLimbo_Overlay, m_Byte91) == 0x91);
static_assert(offsetof(CThingInLimbo_Overlay, m_Byte92) == 0x92);
}

void __thiscall
CGameScriptInterface::EntitySetInLimbo(const CScriptThing& entity, bool in_limbo, bool param_3) const
{
    (void)this;

    const auto* const vtable =
        *reinterpret_cast<CScriptThing_VTable_Overlay* const*>(const_cast<CScriptThing*>(&entity));
    auto* const thing = static_cast<CThing*>(vtable->m_GetThing(&entity));

    if ((thing != nullptr) && ((reinterpret_cast<CThingInLimbo_Overlay*>(thing)->m_Byte91 & 0x01u) == 0))
    {
        thing->SetInLimbo(in_limbo);

        auto* const thing_overlay = reinterpret_cast<CThingInLimbo_Overlay*>(thing);
        const std::uint8_t cVar1 = ((in_limbo == false) || (param_3 == false)) ? 0u : 1u;

        thing_overlay->m_Byte92 =
            thing_overlay->m_Byte92 ^ (((cVar1 << 6) ^ thing_overlay->m_Byte92) & 0x40u);
    }
}