#include <cstddef>
#include <cstdint>

class CCharString;
class CCreatureActionBase;
class CGameScriptInterface;
class CScriptThing;
class CThingCreatureBase;

class CThingCreatureBase
{
public:
    void __thiscall SetCurrentAction(CCreatureActionBase* pAction);
};

class CGameScriptInterface
{
public:
    void __thiscall SetThingAsConscious(
        const CScriptThing& thing,
        bool conscious,
        const CCharString& reason) const;
};

namespace
{
struct CScriptThingVTableOverlay
{
    std::byte m_Pad00[0x2C];
    CThingCreatureBase* (__thiscall* GetThing)(const CScriptThing* pThis); // 0x2C
    std::byte m_Pad30[0xFC];
    bool (__thiscall* IsValidThing)(const CScriptThing* pThis); // 0x12C
};

static_assert(offsetof(CScriptThingVTableOverlay, GetThing) == 0x2C);
static_assert(offsetof(CScriptThingVTableOverlay, IsValidThing) == 0x12C);

struct CScriptThingOverlay
{
    const CScriptThingVTableOverlay* m_pVTable; // 0x00
};

static_assert(offsetof(CScriptThingOverlay, m_pVTable) == 0x00);

struct CThingCreatureBaseOverlay
{
    std::byte m_Pad00[0x24];
    std::uint32_t m_Flags24; // 0x24
    std::byte m_Pad28[0x44];
    std::uint8_t m_Flags6C; // 0x6C
    std::byte m_Pad6D[0x24];
    std::uint8_t m_Flags91; // 0x91
};

static_assert(offsetof(CThingCreatureBaseOverlay, m_Flags24) == 0x24);
static_assert(offsetof(CThingCreatureBaseOverlay, m_Flags6C) == 0x6C);
static_assert(offsetof(CThingCreatureBaseOverlay, m_Flags91) == 0x91);

struct SetThingAsConsciousActionStorage
{
    std::byte m_Bytes[0x118];
};

static_assert(sizeof(SetThingAsConsciousActionStorage) == 0x118);

extern "C" CCreatureActionBase* __thiscall
NPlayerGui_CDrawAutoPickups__CDrawAutoPickups(
    SetThingAsConsciousActionStorage* pThis,
    CThingCreatureBase* pCreature,
    void* pDrawList);

extern "C" CCreatureActionBase* __thiscall
CCombatAction_FallStunned__CCombatAction_FallStunned(
    SetThingAsConsciousActionStorage* pThis,
    CThingCreatureBase* pCreature,
    CThingCreatureBase* pOtherCreature,
    const CCharString* pReason);

extern "C" void __thiscall CTCLightningOrb___CTCLightningOrb(
    SetThingAsConsciousActionStorage* pThis);
}

void __thiscall CGameScriptInterface::SetThingAsConscious(
    const CScriptThing& thing,
    bool conscious,
    const CCharString& reason) const
{
    (void)this;

    const auto* const thingOverlay = reinterpret_cast<const CScriptThingOverlay*>(&thing);
    if (!thingOverlay->m_pVTable->IsValidThing(&thing))
    {
        return;
    }

    CThingCreatureBase* const pCreature = thingOverlay->m_pVTable->GetThing(&thing);
    if ((pCreature == nullptr) ||
        ((reinterpret_cast<const CThingCreatureBaseOverlay*>(pCreature)->m_Flags91 & 0x01) != 0) ||
        ((reinterpret_cast<const CThingCreatureBaseOverlay*>(pCreature)->m_Flags6C & 0x08) == 0) ||
        ((reinterpret_cast<const CThingCreatureBaseOverlay*>(pCreature)->m_Flags24 & 0x00020000u) == 0))
    {
        return;
    }

    SetThingAsConsciousActionStorage actionStorage;

    if (conscious)
    {
        CCreatureActionBase* const pAction =
            NPlayerGui_CDrawAutoPickups__CDrawAutoPickups(
                &actionStorage,
                pCreature,
                nullptr);
        pCreature->SetCurrentAction(pAction);
        CTCLightningOrb___CTCLightningOrb(&actionStorage);
        return;
    }

    CCreatureActionBase* const pAction =
        CCombatAction_FallStunned__CCombatAction_FallStunned(
            &actionStorage,
            pCreature,
            nullptr,
            &reason);
    pCreature->SetCurrentAction(pAction);
    CTCLightningOrb___CTCLightningOrb(&actionStorage);
}