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
    void __thiscall EntitySetDeedReactionsEnabled(const CScriptThing& entity, bool enabled) const;
};

namespace
{
using EntitySetDeedReactionsEnabled_DispatchFn =
    void(__thiscall*)(const CGameScriptInterface*, const CScriptThing&, const CCharString&, bool);

struct CGameScriptInterface_VTable_Overlay
{
    std::byte m_Pad00[0x938];
    EntitySetDeedReactionsEnabled_DispatchFn m_Unknown938; // 0x938
};

static_assert(offsetof(CGameScriptInterface_VTable_Overlay, m_Unknown938) == 0x938);

struct CGameScriptInterface_Overlay
{
    const CGameScriptInterface_VTable_Overlay* m_pVTable; // 0x00
};

static_assert(offsetof(CGameScriptInterface_Overlay, m_pVTable) == 0x00);
}

void __thiscall
CGameScriptInterface::EntitySetDeedReactionsEnabled(const CScriptThing& entity, bool enabled) const
{
    const auto* const pThis = reinterpret_cast<const CGameScriptInterface_Overlay*>(this);

    {
        CCharString stateName("SG_DEED_REACTION_ROOT", -1);
        pThis->m_pVTable->m_Unknown938(this, entity, stateName, enabled);
    }
}