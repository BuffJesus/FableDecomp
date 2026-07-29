#include <cstdio>

class CScriptThing
{
};

class CGSIDecapitate_Entity;

class CGSIDecapitate_Action
{
public:
    CGSIDecapitate_Action* Construct(
        CGSIDecapitate_Entity* pEntity);
    void Destruct();
};

class CGSIDecapitate_Entity
{
public:
    void Decapitate(CGSIDecapitate_Action* pAction);
};

class CGameScriptInterface
{
public:
    virtual void EntityDecapitate(
        const CScriptThing& entity) const;
};

struct ScriptThingStorage
{
    void** m_pVTable;
};

struct EntityStorage
{
    unsigned char m_Pad00[0x38];
    unsigned long m_Flags38;
    unsigned char m_Pad3C[0x30];
    unsigned char m_Flags6C;
    unsigned char m_Pad6D[0x4F];
    unsigned char m_FlagsBC;
};

static void* g_ScriptThingVTable[76];
static bool g_IsValid;
static EntityStorage* g_pEntity;
static ScriptThingStorage* g_pObservedScriptThing;
static CGSIDecapitate_Action* g_pObservedAction;
static CGSIDecapitate_Entity* g_pObservedConstructEntity;
static CGSIDecapitate_Entity* g_pObservedDecapitateEntity;
static CGSIDecapitate_Action* g_pObservedDecapitateAction;
static CGSIDecapitate_Action* g_pObservedDestructAction;
static unsigned long g_IsValidCalls;
static unsigned long g_GetEntityCalls;
static unsigned long g_ConstructCalls;
static unsigned long g_DecapitateCalls;
static unsigned long g_DestructCalls;

static bool __fastcall IsValid(
    ScriptThingStorage* pScriptThing)
{
    g_pObservedScriptThing = pScriptThing;
    ++g_IsValidCalls;
    return g_IsValid;
}

static EntityStorage* __fastcall GetEntity(
    ScriptThingStorage* pScriptThing)
{
    g_pObservedScriptThing = pScriptThing;
    ++g_GetEntityCalls;
    return g_pEntity;
}

CGSIDecapitate_Action*
CGSIDecapitate_Action::Construct(
    CGSIDecapitate_Entity* pEntity)
{
    g_pObservedAction = this;
    g_pObservedConstructEntity = pEntity;
    ++g_ConstructCalls;
    return this;
}

void CGSIDecapitate_Entity::Decapitate(
    CGSIDecapitate_Action* pAction)
{
    g_pObservedDecapitateEntity = this;
    g_pObservedDecapitateAction = pAction;
    ++g_DecapitateCalls;
}

void CGSIDecapitate_Action::Destruct()
{
    g_pObservedDestructAction = this;
    ++g_DestructCalls;
}

static void ResetObservations()
{
    g_pObservedScriptThing = 0;
    g_pObservedAction = 0;
    g_pObservedConstructEntity = 0;
    g_pObservedDecapitateEntity = 0;
    g_pObservedDecapitateAction = 0;
    g_pObservedDestructAction = 0;
    g_IsValidCalls = 0;
    g_GetEntityCalls = 0;
    g_ConstructCalls = 0;
    g_DecapitateCalls = 0;
    g_DestructCalls = 0;
}

static int Fail(const char* pMessage)
{
    std::printf(
        "FSE2_008a8e40_TEST FAIL: %s\n",
        pMessage);
    return 1;
}

int main()
{
    g_ScriptThingVTable[11] =
        reinterpret_cast<void*>(GetEntity);
    g_ScriptThingVTable[75] =
        reinterpret_cast<void*>(IsValid);

    ScriptThingStorage scriptThingStorage = {
        g_ScriptThingVTable
    };
    const CScriptThing& scriptThing =
        reinterpret_cast<const CScriptThing&>(
            scriptThingStorage);
    EntityStorage entity = {};
    CGameScriptInterface interfaceObject;

    ResetObservations();
    g_IsValid = false;
    g_pEntity = &entity;
    interfaceObject.CGameScriptInterface::
        EntityDecapitate(scriptThing);
    if (g_IsValidCalls != 1 ||
        g_GetEntityCalls != 0 ||
        g_ConstructCalls != 0 ||
        g_pObservedScriptThing != &scriptThingStorage)
        return Fail("invalid script thing guard");

    ResetObservations();
    g_IsValid = true;
    entity.m_Flags38 = 0;
    entity.m_Flags6C = 0x0F;
    entity.m_FlagsBC = 2;
    interfaceObject.CGameScriptInterface::
        EntityDecapitate(scriptThing);
    if (g_GetEntityCalls != 1 ||
        g_ConstructCalls != 0)
        return Fail("entity type guard");

    ResetObservations();
    entity.m_Flags38 = 0x20000;
    entity.m_Flags6C = 0x10;
    interfaceObject.CGameScriptInterface::
        EntityDecapitate(scriptThing);
    if (g_ConstructCalls != 0)
        return Fail("body-state guard");

    ResetObservations();
    entity.m_Flags6C = 1;
    entity.m_FlagsBC = 0;
    interfaceObject.CGameScriptInterface::
        EntityDecapitate(scriptThing);
    if (g_ConstructCalls != 0)
        return Fail("decapitation capability guard");

    ResetObservations();
    entity.m_FlagsBC = 2;
    interfaceObject.CGameScriptInterface::
        EntityDecapitate(scriptThing);
    if (g_IsValidCalls != 1 ||
        g_GetEntityCalls != 1 ||
        g_ConstructCalls != 1 ||
        g_DecapitateCalls != 1 ||
        g_DestructCalls != 1 ||
        g_pObservedConstructEntity !=
            reinterpret_cast<CGSIDecapitate_Entity*>(&entity) ||
        g_pObservedDecapitateEntity !=
            reinterpret_cast<CGSIDecapitate_Entity*>(&entity) ||
        g_pObservedAction == 0 ||
        g_pObservedDecapitateAction != g_pObservedAction ||
        g_pObservedDestructAction != g_pObservedAction)
        return Fail("successful decapitation sequence");

    std::printf("FSE2_008a8e40_TEST PASS\n");
    return 0;
}
