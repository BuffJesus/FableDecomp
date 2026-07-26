#include <cstddef>
#include <cstdint>

class CCombatAction_GenericResponseToDrain;
class CCreatureActionBase;
class CGameScriptInterface;
class CScriptThing;
class CThing;
class CThingCreatureBase;

class CThingCreatureBase
{
public:
    void __thiscall SetCurrentAction(CCreatureActionBase* pAction);
};

class CGameScriptInterface
{
public:
    void __thiscall EntitySetAttackThingImmediately(
        const CScriptThing& entity,
        const CScriptThing& target,
        bool a4,
        bool a5) const;
};

namespace
{
struct CScriptThing_VTable_Overlay
{
    std::byte m_Pad00[0x2C];
    CThing* (__thiscall* GetThing)(const CScriptThing* pScriptThing); // 0x2C
};

static_assert(offsetof(CScriptThing_VTable_Overlay, GetThing) == 0x2C);

struct CScriptThing_Overlay
{
    const CScriptThing_VTable_Overlay* m_pVTable; // 0x00
};

static_assert(offsetof(CScriptThing_Overlay, m_pVTable) == 0x00);

struct CThing_Overlay
{
    std::byte m_Pad00[0x91];
    std::uint8_t m_Flags91; // 0x91
};

static_assert(offsetof(CThing_Overlay, m_Flags91) == 0x91);

struct CThingCreatureBase_Overlay
{
    std::byte m_Pad00[0x6C];
    std::uint8_t m_Flags6C; // 0x6C
    std::byte m_Pad6D[0x24];
    std::uint8_t m_Flags91; // 0x91
};

static_assert(offsetof(CThingCreatureBase_Overlay, m_Flags6C) == 0x6C);
static_assert(offsetof(CThingCreatureBase_Overlay, m_Flags91) == 0x91);

struct CCombatAction_GenericResponseToDrain_Overlay
{
    std::byte m_Pad00[0x118];
};

static_assert(sizeof(CCombatAction_GenericResponseToDrain_Overlay) == 0x118);

extern "C" CCreatureActionBase*
__thiscall CCombatAction_GenericResponseToDrain__CCombatAction_GenericResponseToDrain(
    CCombatAction_GenericResponseToDrain_Overlay* pThis,
    CThingCreatureBase* pCreature,
    CThing* pTarget);

extern "C" void __thiscall CTCLightningOrb___CTCLightningOrb(
    CCombatAction_GenericResponseToDrain_Overlay* pThis);
}

void __thiscall CGameScriptInterface::EntitySetAttackThingImmediately(
    const CScriptThing& entity,
    const CScriptThing& target,
    bool a4,
    bool a5) const
{
    (void)a4;
    (void)a5;

    const auto* const entityOverlay = reinterpret_cast<const CScriptThing_Overlay*>(&entity);
    CThingCreatureBase* const pCreature =
        reinterpret_cast<CThingCreatureBase*>(entityOverlay->m_pVTable->GetThing(&entity));

    const auto* const targetOverlay = reinterpret_cast<const CScriptThing_Overlay*>(&target);
    CThing* const pTarget = targetOverlay->m_pVTable->GetThing(&target);

    if ((pCreature != nullptr) &&
        ((reinterpret_cast<const CThingCreatureBase_Overlay*>(pCreature)->m_Flags91 & 0x01) == 0) &&
        (pTarget != nullptr) &&
        ((reinterpret_cast<const CThing_Overlay*>(pTarget)->m_Flags91 & 0x01) == 0) &&
        ((reinterpret_cast<const CThingCreatureBase_Overlay*>(pCreature)->m_Flags6C & 0x08) != 0))
    {
        CCombatAction_GenericResponseToDrain_Overlay action;
        CCreatureActionBase* const pAction =
            CCombatAction_GenericResponseToDrain__CCombatAction_GenericResponseToDrain(
                &action,
                pCreature,
                pTarget);
        pCreature->SetCurrentAction(pAction);
        CTCLightningOrb___CTCLightningOrb(&action);
    }
}