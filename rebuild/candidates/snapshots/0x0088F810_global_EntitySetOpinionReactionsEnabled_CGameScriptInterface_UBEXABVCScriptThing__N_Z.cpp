#include <cstddef>
#include <cstdint>

class CScriptThing;

struct CCharStringRep
{
    const char* m_pString;   // 0x00
    std::int32_t m_Length;   // 0x04
};

static_assert(offsetof(CCharStringRep, m_pString) == 0x00);
static_assert(offsetof(CCharStringRep, m_Length) == 0x04);
static_assert(sizeof(CCharStringRep) == 0x08);

class CCharString
{
public:
    CCharStringRep* m_pRep; // 0x00

    CCharString(const char* pText, int length);
    ~CCharString();
};

static_assert(offsetof(CCharString, m_pRep) == 0x00);
static_assert(sizeof(CCharString) == 0x04);

class CGameScriptInterface
{
public:
    void __thiscall EntitySetOpinionReactionsEnabled(
        const CScriptThing& entity,
        bool enabled) const;
};

namespace
{
using EntitySetOpinionReactionsEnabledSlotFn = void(__thiscall*)(
    const CGameScriptInterface* pThis,
    const CScriptThing& entity,
    const CCharString& tcName,
    bool enabled);

struct CGameScriptInterfaceVTableOverlay
{
    std::byte m_Pad00[0x938];
    EntitySetOpinionReactionsEnabledSlotFn m_Slot938; // 0x938
};

static_assert(offsetof(CGameScriptInterfaceVTableOverlay, m_Slot938) == 0x938);
}

void __thiscall CGameScriptInterface::EntitySetOpinionReactionsEnabled(
    const CScriptThing& entity,
    bool enabled) const
{
    CCharString tcName("SG_OPINION_REACTION_ROOT", -1);

    const auto* const vtable =
        *reinterpret_cast<const CGameScriptInterfaceVTableOverlay* const*>(this);
    vtable->m_Slot938(this, entity, tcName, enabled);
}