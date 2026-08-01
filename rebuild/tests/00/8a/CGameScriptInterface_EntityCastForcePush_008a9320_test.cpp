#include <cstdio>

class CCreatureActionBase;
class CScriptThing;
class CThingCreatureBase;

class CThingCreatureBase
{
public:
    bool SetCurrentAction(CCreatureActionBase* pAction);
};

class CGameScriptInterface
{
public:
    bool EntityCastForcePush(const CScriptThing& thing, bool forcePush) const;
};

namespace
{
struct CScriptThing_VTable_Overlay
{
    unsigned char m_Pad00[0x2C];
    CThingCreatureBase* (__fastcall* GetThing)(const CScriptThing* pScriptThing);
    unsigned char m_Pad30[0xFC];
    bool (__fastcall* IsValidThing)(const CScriptThing* pScriptThing);
};

struct CScriptThing_Overlay
{
    const CScriptThing_VTable_Overlay* m_pVTable;
};

struct CThingCreatureBase_Overlay
{
    unsigned char m_Pad00[0x6C];
    unsigned char m_Flags6C;
};

struct CCreatureAction_SheatheWeapons_Overlay
{
    unsigned char m_Pad00[0x114];
    unsigned char m_Field110;
    unsigned char m_Pad115[0x03];
    void Construct(CThingCreatureBase* pCreature);
    void Destruct();
};
}

bool CGameScriptInterface::EntityCastForcePush(
    const CScriptThing& thing,
    bool forcePush) const
{
    const CScriptThing_Overlay* const thingOverlay =
        reinterpret_cast<const CScriptThing_Overlay*>(&thing);

    if (thingOverlay->m_pVTable->IsValidThing(&thing))
    {
        CThingCreatureBase* const pCreature = thingOverlay->m_pVTable->GetThing(&thing);
        if ((reinterpret_cast<const CThingCreatureBase_Overlay*>(pCreature)->m_Flags6C & 0x08) != 0)
        {
            CCreatureAction_SheatheWeapons_Overlay action;
            action.Construct(pCreature);

            if (forcePush)
            {
                action.m_Field110 = 1;
            }

            const bool result =
                pCreature->SetCurrentAction(reinterpret_cast<CCreatureActionBase*>(&action));

            action.Destruct();
            return result;
        }
    }

    return false;
}

// ---- test scaffolding ----
static CThingCreatureBase_Overlay g_creature;
static int g_ctorCalls = 0;
static int g_dtorCalls = 0;
static unsigned char g_actionFieldAtSet = 0xFF;
static CCreatureActionBase* g_setActionArg = 0;
static bool g_setActionResult = true;
static CThingCreatureBase* g_ctorCreatureArg = 0;

bool CThingCreatureBase::SetCurrentAction(CCreatureActionBase* pAction)
{
    g_setActionArg = pAction;
    g_actionFieldAtSet =
        reinterpret_cast<CCreatureAction_SheatheWeapons_Overlay*>(pAction)->m_Field110;
    return g_setActionResult;
}

void CCreatureAction_SheatheWeapons_Overlay::Construct(CThingCreatureBase* pCreature)
{
    ++g_ctorCalls;
    g_ctorCreatureArg = pCreature;
    m_Field110 = 0;
}

void CCreatureAction_SheatheWeapons_Overlay::Destruct()
{
    ++g_dtorCalls;
}

static CThingCreatureBase* __fastcall GetThing_Impl(const CScriptThing*)
{
    return reinterpret_cast<CThingCreatureBase*>(&g_creature);
}

static bool g_isValid = true;
static bool __fastcall IsValidThing_Impl(const CScriptThing*)
{
    return g_isValid;
}

static CScriptThing_VTable_Overlay g_vtbl;
static CScriptThing_Overlay g_thing;

int main()
{
    g_vtbl.GetThing = reinterpret_cast<CThingCreatureBase* (__fastcall*)(const CScriptThing*)>(&GetThing_Impl);
    g_vtbl.IsValidThing = reinterpret_cast<bool (__fastcall*)(const CScriptThing*)>(&IsValidThing_Impl);
    g_thing.m_pVTable = &g_vtbl;

    CGameScriptInterface gsi;
    const CScriptThing& thingRef = *reinterpret_cast<const CScriptThing*>(&g_thing);

    g_isValid = false;
    g_ctorCalls = 0;
    bool r1 = gsi.EntityCastForcePush(thingRef, true);
    if (r1 != false || g_ctorCalls != 0) { std::printf("FAIL c1\n"); return 1; }

    g_isValid = true;
    g_creature.m_Flags6C = 0x00;
    g_ctorCalls = 0;
    bool r2 = gsi.EntityCastForcePush(thingRef, true);
    if (r2 != false || g_ctorCalls != 0) { std::printf("FAIL c2\n"); return 1; }

    g_isValid = true;
    g_creature.m_Flags6C = 0x08;
    g_ctorCalls = 0; g_dtorCalls = 0; g_actionFieldAtSet = 0xFF;
    g_setActionResult = true;
    bool r3 = gsi.EntityCastForcePush(thingRef, true);
    if (r3 != true || g_ctorCalls != 1 || g_dtorCalls != 1 || g_actionFieldAtSet != 1)
    { std::printf("FAIL c3\n"); return 1; }
    if (g_ctorCreatureArg != reinterpret_cast<CThingCreatureBase*>(&g_creature))
    { std::printf("FAIL c3b\n"); return 1; }

    g_creature.m_Flags6C = 0xFF;
    g_ctorCalls = 0; g_dtorCalls = 0; g_actionFieldAtSet = 0xFF;
    g_setActionResult = false;
    bool r4 = gsi.EntityCastForcePush(thingRef, false);
    if (r4 != false || g_actionFieldAtSet != 0 || g_dtorCalls != 1)
    { std::printf("FAIL c4\n"); return 1; }

    std::printf("ENTITYCASTFORCEPUSH_OK\n");
    return 0;
}