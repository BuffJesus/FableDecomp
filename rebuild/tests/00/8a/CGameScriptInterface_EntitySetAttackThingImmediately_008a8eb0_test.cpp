#include <cstdio>

class CScriptThing
{
};

class CGSISetAttack_Entity;

class CGSISetAttack_Action
{
public:
    CGSISetAttack_Action* Construct(
        CGSISetAttack_Entity* pAttacker,
        CGSISetAttack_Entity* pTarget);
    void Destruct();
};

class CGSISetAttack_Entity
{
public:
    void Dispatch(CGSISetAttack_Action* pAction);
};

class CGameScriptInterface
{
public:
    virtual void EntitySetAttackThingImmediately(
        const CScriptThing& attacker,
        const CScriptThing& target,
        bool unknown0,
        bool unknown1) const;
};

struct ScriptThingStorage
{
    void** m_pVTable;
    CGSISetAttack_Entity* m_pEntity;
};

struct EntityStorage
{
    unsigned char m_Pad00[0x6C];
    unsigned char m_Flags6C;
    unsigned char m_Pad6D[0x24];
    unsigned char m_Flags91;
};

static void* g_ScriptThingVTable[12];
static ScriptThingStorage* g_pObservedScriptThings[2];
static unsigned long g_ResolveCalls;
static CGSISetAttack_Action* g_pObservedAction;
static CGSISetAttack_Entity* g_pObservedConstructAttacker;
static CGSISetAttack_Entity* g_pObservedConstructTarget;
static CGSISetAttack_Entity* g_pObservedDispatchEntity;
static CGSISetAttack_Action* g_pObservedDispatchAction;
static CGSISetAttack_Action* g_pObservedDestructAction;
static unsigned long g_ConstructCalls;
static unsigned long g_DispatchCalls;
static unsigned long g_DestructCalls;

static CGSISetAttack_Entity* __fastcall ResolveEntity(
    ScriptThingStorage* pScriptThing)
{
    if (g_ResolveCalls < 2)
        g_pObservedScriptThings[g_ResolveCalls] = pScriptThing;
    ++g_ResolveCalls;
    return pScriptThing->m_pEntity;
}

CGSISetAttack_Action*
CGSISetAttack_Action::Construct(
    CGSISetAttack_Entity* pAttacker,
    CGSISetAttack_Entity* pTarget)
{
    g_pObservedAction = this;
    g_pObservedConstructAttacker = pAttacker;
    g_pObservedConstructTarget = pTarget;
    ++g_ConstructCalls;
    return this;
}

void CGSISetAttack_Entity::Dispatch(
    CGSISetAttack_Action* pAction)
{
    g_pObservedDispatchEntity = this;
    g_pObservedDispatchAction = pAction;
    ++g_DispatchCalls;
}

void CGSISetAttack_Action::Destruct()
{
    g_pObservedDestructAction = this;
    ++g_DestructCalls;
}

static void ResetObservations()
{
    g_pObservedScriptThings[0] = 0;
    g_pObservedScriptThings[1] = 0;
    g_ResolveCalls = 0;
    g_pObservedAction = 0;
    g_pObservedConstructAttacker = 0;
    g_pObservedConstructTarget = 0;
    g_pObservedDispatchEntity = 0;
    g_pObservedDispatchAction = 0;
    g_pObservedDestructAction = 0;
    g_ConstructCalls = 0;
    g_DispatchCalls = 0;
    g_DestructCalls = 0;
}

static int Fail(const char* pMessage)
{
    std::printf(
        "FSE2_008a8eb0_TEST FAIL: %s\n",
        pMessage);
    return 1;
}

int main()
{
    g_ScriptThingVTable[11] =
        reinterpret_cast<void*>(ResolveEntity);

    EntityStorage attackerEntity = {};
    EntityStorage targetEntity = {};
    ScriptThingStorage attackerStorage = {
        g_ScriptThingVTable,
        reinterpret_cast<CGSISetAttack_Entity*>(
            &attackerEntity)
    };
    ScriptThingStorage targetStorage = {
        g_ScriptThingVTable,
        reinterpret_cast<CGSISetAttack_Entity*>(
            &targetEntity)
    };
    const CScriptThing& attacker =
        reinterpret_cast<const CScriptThing&>(
            attackerStorage);
    const CScriptThing& target =
        reinterpret_cast<const CScriptThing&>(
            targetStorage);
    CGameScriptInterface interfaceObject;

    attackerEntity.m_Flags6C = 8;

    ResetObservations();
    attackerStorage.m_pEntity = 0;
    interfaceObject.CGameScriptInterface::
        EntitySetAttackThingImmediately(
            attacker, target, false, true);
    if (g_ResolveCalls != 2 ||
        g_pObservedScriptThings[0] != &attackerStorage ||
        g_pObservedScriptThings[1] != &targetStorage ||
        g_ConstructCalls != 0)
        return Fail("null attacker guard and resolve order");

    ResetObservations();
    attackerStorage.m_pEntity =
        reinterpret_cast<CGSISetAttack_Entity*>(
            &attackerEntity);
    attackerEntity.m_Flags91 = 1;
    interfaceObject.CGameScriptInterface::
        EntitySetAttackThingImmediately(
            attacker, target, true, false);
    if (g_ResolveCalls != 2 || g_ConstructCalls != 0)
        return Fail("dead attacker guard");

    ResetObservations();
    attackerEntity.m_Flags91 = 0;
    targetStorage.m_pEntity = 0;
    interfaceObject.CGameScriptInterface::
        EntitySetAttackThingImmediately(
            attacker, target, false, false);
    if (g_ResolveCalls != 2 || g_ConstructCalls != 0)
        return Fail("null target guard");

    ResetObservations();
    targetStorage.m_pEntity =
        reinterpret_cast<CGSISetAttack_Entity*>(
            &targetEntity);
    targetEntity.m_Flags91 = 1;
    interfaceObject.CGameScriptInterface::
        EntitySetAttackThingImmediately(
            attacker, target, false, false);
    if (g_ConstructCalls != 0)
        return Fail("dead target guard");

    ResetObservations();
    targetEntity.m_Flags91 = 0;
    attackerEntity.m_Flags6C = 0;
    interfaceObject.CGameScriptInterface::
        EntitySetAttackThingImmediately(
            attacker, target, true, true);
    if (g_ConstructCalls != 0)
        return Fail("attacker capability guard");

    ResetObservations();
    attackerEntity.m_Flags6C = 8;
    interfaceObject.CGameScriptInterface::
        EntitySetAttackThingImmediately(
            attacker, target, true, false);
    if (g_ResolveCalls != 2 ||
        g_ConstructCalls != 1 ||
        g_DispatchCalls != 1 ||
        g_DestructCalls != 1 ||
        g_pObservedConstructAttacker !=
            reinterpret_cast<CGSISetAttack_Entity*>(
                &attackerEntity) ||
        g_pObservedConstructTarget !=
            reinterpret_cast<CGSISetAttack_Entity*>(
                &targetEntity) ||
        g_pObservedDispatchEntity !=
            reinterpret_cast<CGSISetAttack_Entity*>(
                &attackerEntity) ||
        g_pObservedAction == 0 ||
        g_pObservedDispatchAction != g_pObservedAction ||
        g_pObservedDestructAction != g_pObservedAction)
        return Fail("successful attack dispatch sequence");

    std::printf("FSE2_008a8eb0_TEST PASS\n");
    return 0;
}
