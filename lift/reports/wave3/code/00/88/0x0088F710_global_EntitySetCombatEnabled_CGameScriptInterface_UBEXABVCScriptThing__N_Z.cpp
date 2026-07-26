#include <cstddef>
#include <cstdint>

class CCharString;
class CGameScriptInterface;
class CScriptThing;

class CCharString
{
public:
    CCharString(const char* pText, int length);
    ~CCharString();
};

class CGameScriptInterface
{
public:
    void __thiscall EntitySetCombatEnabled(const CScriptThing& entity, bool enabled) const;
};

namespace
{
using EntitySetCombatEnabled_DispatchFn =
    void(__thiscall*)(const CGameScriptInterface*, const CScriptThing&, const CCharString&, bool);

struct CGameScriptInterface_VTable_Overlay
{
    std::byte m_Pad00[0x938];
    EntitySetCombatEnabled_DispatchFn m_Unknown938; // 0x938
};

static_assert(offsetof(CGameScriptInterface_VTable_Overlay, m_Unknown938) == 0x938);

struct CGameScriptInterface_Overlay
{
    const CGameScriptInterface_VTable_Overlay* m_pVTable; // 0x00
};

static_assert(offsetof(CGameScriptInterface_Overlay, m_pVTable) == 0x00);
}

void __thiscall
CGameScriptInterface::EntitySetCombatEnabled(const CScriptThing& entity, bool enabled) const
{
    const auto* const pThis = reinterpret_cast<const CGameScriptInterface_Overlay*>(this);

    {
        CCharString stateName("SG_SEEK_ENEMY", -1);
        pThis->m_pVTable->m_Unknown938(this, entity, stateName, enabled);
    }

    {
        CCharString stateName("SG_MELEE_COMBAT_NEARBY", -1);
        pThis->m_pVTable->m_Unknown938(this, entity, stateName, enabled);
    }

    {
        CCharString stateName("SG_MELEE_COMBAT_NEARBY2", -1);
        pThis->m_pVTable->m_Unknown938(this, entity, stateName, enabled);
    }

    {
        CCharString stateName("SG_MELEE_COMBAT_NEARBY_ATTACK_WHEN_VERY_CLOSE", -1);
        pThis->m_pVTable->m_Unknown938(this, entity, stateName, enabled);
    }
}