#include <cstddef>
#include <cstdint>

class CScriptThing;

class CGameScriptInterface
{
public:
    void __thiscall EntityClearMaxNumberOfAttackers(const CScriptThing& thing) const;
};

namespace
{
struct CGameScriptInterface_VTable_Overlay
{
    std::byte m_Pad00[0x73C];
    void (__thiscall* m_EntityClearMaxNumberOfAttackers)(
        const CGameScriptInterface* pThis,
        const CScriptThing& thing,
        std::int32_t maxNumberOfAttackers); // 0x73C
};

static_assert(
    offsetof(CGameScriptInterface_VTable_Overlay, m_EntityClearMaxNumberOfAttackers) == 0x73C);
}

void __thiscall CGameScriptInterface::EntityClearMaxNumberOfAttackers(
    const CScriptThing& thing) const
{
    const auto* const pVTable =
        *reinterpret_cast<const CGameScriptInterface_VTable_Overlay* const*>(this);

    pVTable->m_EntityClearMaxNumberOfAttackers(this, thing, -1);
}