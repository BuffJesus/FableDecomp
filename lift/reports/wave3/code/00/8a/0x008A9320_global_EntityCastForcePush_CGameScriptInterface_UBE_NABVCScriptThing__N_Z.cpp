#include <cstddef>
#include <cstdint>

class CCreatureActionBase;
class CCreatureAction_SheatheWeapons;
class CGameScriptInterface;
class CScriptThing;
class CThingCreatureBase;

class CThingCreatureBase
{
public:
    bool __thiscall SetCurrentAction(CCreatureActionBase* pAction);
};

class CGameScriptInterface
{
public:
    bool __thiscall EntityCastForcePush(
        const CScriptThing& thing,
        bool forcePush) const;
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
    std::byte m_Pad00[0x6C];
    std::uint8_t m_Flags6C; // 0x6C
};

static_assert(offsetof(CThingCreatureBase_Overlay, m_Flags6C) == 0x6C);

struct CCreatureAction_SheatheWeapons_Overlay
{
    std::byte m_Pad00[0x110];
    std::uint8_t m_Field110; // 0x110
    std::byte m_Pad111[0x03];
};

static_assert(offsetof(CCreatureAction_SheatheWeapons_Overlay, m_Field110) == 0x110);
static_assert(sizeof(CCreatureAction_SheatheWeapons_Overlay) == 0x114);

extern "C" void __thiscall CCreatureAction_SheatheWeapons__CCreatureAction_SheatheWeapons(
    CCreatureAction_SheatheWeapons_Overlay* pThis,
    CThingCreatureBase* pCreature);

extern "C" void __thiscall CTCLightningOrb___CTCLightningOrb(
    CCreatureAction_SheatheWeapons_Overlay* pThis);
}

bool __thiscall CGameScriptInterface::EntityCastForcePush(
    const CScriptThing& thing,
    bool forcePush) const
{
    const auto* const thingOverlay = reinterpret_cast<const CScriptThing_Overlay*>(&thing);

    if (!thingOverlay->m_pVTable->IsValidThing(&thing))
    {
        return false;
    }

    CThingCreatureBase* const pCreature = thingOverlay->m_pVTable->GetThing(&thing);
    if ((reinterpret_cast<const CThingCreatureBase_Overlay*>(pCreature)->m_Flags6C & 0x08) == 0)
    {
        return false;
    }

    CCreatureAction_SheatheWeapons_Overlay action;
    CCreatureAction_SheatheWeapons__CCreatureAction_SheatheWeapons(&action, pCreature);

    if (forcePush)
    {
        action.m_Field110 = 1;
    }

    const bool result =
        pCreature->SetCurrentAction(reinterpret_cast<CCreatureActionBase*>(&action));

    CTCLightningOrb___CTCLightningOrb(&action);
    return result;
}