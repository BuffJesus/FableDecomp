#include <cstddef>
#include <cstdint>

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
    void __thiscall EntityDecapitate(const CScriptThing& thing) const;
};

namespace
{
struct CScriptThing_VTable_Overlay
{
    std::byte m_Pad00[0x2C];
    CThingCreatureBase* (__thiscall* GetThing)(const CScriptThing* pScriptThing); // 0x2C
    std::byte m_Pad30[0xFC];
    bool (__thiscall* IsValidThing)(const CScriptThing* pScriptThing); // 0x12C
};

static_assert(offsetof(CScriptThing_VTable_Overlay, GetThing) == 0x2C);
static_assert(offsetof(CScriptThing_VTable_Overlay, IsValidThing) == 0x12C);

struct CScriptThing_Overlay
{
    const CScriptThing_VTable_Overlay* m_pVTable; // 0x00
};

static_assert(offsetof(CScriptThing_Overlay, m_pVTable) == 0x00);

struct CThingCreatureBase_Overlay
{
    std::byte m_Pad00[0x38];
    std::uint32_t m_Flags38; // 0x38
    std::byte m_Pad3C[0x30];
    std::uint8_t m_Flags6C; // 0x6C
    std::byte m_Pad6D[0x4F];
    std::uint8_t m_FlagsBC; // 0xBC
};

static_assert(offsetof(CThingCreatureBase_Overlay, m_Flags38) == 0x38);
static_assert(offsetof(CThingCreatureBase_Overlay, m_Flags6C) == 0x6C);
static_assert(offsetof(CThingCreatureBase_Overlay, m_FlagsBC) == 0xBC);

struct CCombatAction_GenericResponseToDecapitation_Overlay
{
    std::byte m_Pad00[0x114];
};

static_assert(sizeof(CCombatAction_GenericResponseToDecapitation_Overlay) == 0x114);

extern "C" CCreatureActionBase*
__thiscall CCombatAction_GenericResponseToDecapitation__CCombatAction_GenericResponseToDecapitation(
    CCombatAction_GenericResponseToDecapitation_Overlay* pThis,
    CThingCreatureBase* pCreature);

extern "C" void __thiscall CTCLightningOrb___CTCLightningOrb(
    CCombatAction_GenericResponseToDecapitation_Overlay* pThis);
}

void __thiscall CGameScriptInterface::EntityDecapitate(const CScriptThing& thing) const
{
    const auto* const thingOverlay = reinterpret_cast<const CScriptThing_Overlay*>(&thing);
    if (!thingOverlay->m_pVTable->IsValidThing(&thing))
    {
        return;
    }

    CThingCreatureBase* const pCreature = thingOverlay->m_pVTable->GetThing(&thing);
    const auto* const creatureOverlay = reinterpret_cast<const CThingCreatureBase_Overlay*>(pCreature);

    if (((creatureOverlay->m_Flags38 & 0x20000) != 0) &&
        ((creatureOverlay->m_Flags6C & 0x0F) != 0) &&
        ((creatureOverlay->m_FlagsBC & 0x02) != 0))
    {
        CCombatAction_GenericResponseToDecapitation_Overlay action;
        CCreatureActionBase* const pAction =
            CCombatAction_GenericResponseToDecapitation__CCombatAction_GenericResponseToDecapitation(
                &action,
                pCreature);
        pCreature->SetCurrentAction(pAction);
        CTCLightningOrb___CTCLightningOrb(&action);
    }
}