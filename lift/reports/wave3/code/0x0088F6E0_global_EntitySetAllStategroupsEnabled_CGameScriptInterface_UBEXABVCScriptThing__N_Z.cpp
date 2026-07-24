#include <cstddef>
#include <cstdint>

class CGameScriptInterface;
class CScriptThing;

namespace
{
struct CScriptThingVTableOverlay
{
    std::byte m_Pad00[0x2C];
    void* (__thiscall* GetThing)(const CScriptThing* pThis); // 0x2C
};

static_assert(offsetof(CScriptThingVTableOverlay, GetThing) == 0x2C);

struct CScriptThingOverlay
{
    const CScriptThingVTableOverlay* m_pVTable; // 0x00
};

static_assert(offsetof(CScriptThingOverlay, m_pVTable) == 0x00);

struct CThingStateGroupOverlay
{
    std::byte m_Pad00[0x10];
    std::uint8_t m_Field10; // 0x10
};

static_assert(offsetof(CThingStateGroupOverlay, m_Field10) == 0x10);
}

void __thiscall CGameScriptInterface::EntitySetAllStategroupsEnabled(
    const CScriptThing& entity,
    bool enabled) const
{
    using SetAllStategroupsEnabledFn = void (__thiscall*)(void* pThing, bool enabled);

    const auto* const entityOverlay = reinterpret_cast<const CScriptThingOverlay*>(&entity);
    void* const pThing = entityOverlay->m_pVTable->GetThing(&entity);

    if (pThing != nullptr)
    {
        auto* const pThingOverlay = reinterpret_cast<CThingStateGroupOverlay*>(pThing);
        if (pThingOverlay->m_Field10 == 1)
        {
            reinterpret_cast<SetAllStategroupsEnabledFn>(0x008302D0)(pThing, enabled);
        }
    }
}