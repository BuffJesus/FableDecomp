#include <cstdio>

class CScriptThing
{
};

class CGSIWielding_Node;

class CGSIWielding_Weapon
{
};

class CGSIWielding_Container
{
public:
    CGSIWielding_Node* LowerBound(const long* pKey);

    CGSIWielding_Node* m_pFirst;
    CGSIWielding_Node* m_pEnd;
};

class CGSIWielding_Component
{
public:
    CGSIWielding_Weapon* GetWieldedWeapon();
};

class CGSIWielding_Node
{
public:
    long m_Key;
    CGSIWielding_Component* m_pComponent;
};

class CGameScriptInterface
{
public:
    virtual bool IsEntityWieldingWeapon(
        const CScriptThing& entity) const;
};

struct ScriptThingStorage
{
    void** m_pVTable;
};

struct EntityStorage
{
    unsigned char m_Pad00[0x28];
    unsigned char m_Flags28;
    unsigned char m_Pad29[0x1B];
    CGSIWielding_Container m_Container;
    unsigned char m_Pad4C[0x45];
    unsigned char m_Flags91;
};

struct WeaponStorage
{
    unsigned char m_Pad00[0x28];
    unsigned long m_Flags28;
};

static void* g_ScriptThingVTable[12];
static EntityStorage* g_pEntity;
static CGSIWielding_Node* g_pLowerBoundResult;
static WeaponStorage* g_pWeapon;
static CGSIWielding_Container* g_pObservedContainer;
static CGSIWielding_Component* g_pObservedComponent;
static long g_ObservedKey;
static unsigned long g_ResolveCalls;
static unsigned long g_LowerBoundCalls;
static unsigned long g_GetWeaponCalls;

static EntityStorage* __fastcall ResolveEntity(
    ScriptThingStorage*)
{
    ++g_ResolveCalls;
    return g_pEntity;
}

CGSIWielding_Node*
CGSIWielding_Container::LowerBound(const long* pKey)
{
    g_pObservedContainer = this;
    g_ObservedKey = *pKey;
    ++g_LowerBoundCalls;
    return g_pLowerBoundResult;
}

CGSIWielding_Weapon*
CGSIWielding_Component::GetWieldedWeapon()
{
    g_pObservedComponent = this;
    ++g_GetWeaponCalls;
    return reinterpret_cast<CGSIWielding_Weapon*>(
        g_pWeapon);
}

static void ResetObservations()
{
    g_pObservedContainer = 0;
    g_pObservedComponent = 0;
    g_ObservedKey = 0;
    g_ResolveCalls = 0;
    g_LowerBoundCalls = 0;
    g_GetWeaponCalls = 0;
}

static int Fail(const char* pMessage)
{
    std::printf(
        "FSE2_008974f0_TEST FAIL: %s\n",
        pMessage);
    return 1;
}

int main()
{
    g_ScriptThingVTable[11] =
        reinterpret_cast<void*>(ResolveEntity);
    ScriptThingStorage scriptThingStorage = {
        g_ScriptThingVTable
    };
    const CScriptThing& scriptThing =
        reinterpret_cast<const CScriptThing&>(
            scriptThingStorage);
    EntityStorage entity = {};
    WeaponStorage weapon = {};
    CGSIWielding_Component foundComponent;
    CGSIWielding_Component endComponent;
    CGSIWielding_Node foundNode = {};
    CGSIWielding_Node endNode = {};
    CGameScriptInterface interfaceObject;

    foundNode.m_pComponent = &foundComponent;
    endNode.m_pComponent = &endComponent;
    entity.m_Container.m_pEnd = &endNode;

    ResetObservations();
    g_pEntity = 0;
    if (interfaceObject.CGameScriptInterface::
            IsEntityWieldingWeapon(scriptThing) ||
        g_LowerBoundCalls != 0)
        return Fail("null entity guard");

    g_pEntity = &entity;
    entity.m_Flags28 = 0x40;

    ResetObservations();
    entity.m_Flags91 = 1;
    if (interfaceObject.CGameScriptInterface::
            IsEntityWieldingWeapon(scriptThing))
        return Fail("dead entity guard");

    ResetObservations();
    entity.m_Flags91 = 0;
    entity.m_Flags28 = 0;
    if (interfaceObject.CGameScriptInterface::
            IsEntityWieldingWeapon(scriptThing))
        return Fail("inventory capability guard");

    ResetObservations();
    entity.m_Flags28 = 0x40;
    foundNode.m_Key = 0x46;
    g_pLowerBoundResult = &foundNode;
    g_pWeapon = 0;
    if (interfaceObject.CGameScriptInterface::
            IsEntityWieldingWeapon(scriptThing) ||
        g_pObservedContainer != &entity.m_Container ||
        g_ObservedKey != 0x46 ||
        g_pObservedComponent != &foundComponent ||
        g_GetWeaponCalls != 1)
        return Fail("no wielded weapon");

    ResetObservations();
    g_pWeapon = &weapon;
    weapon.m_Flags28 = 0;
    if (interfaceObject.CGameScriptInterface::
            IsEntityWieldingWeapon(scriptThing))
        return Fail("weapon inactive");

    ResetObservations();
    weapon.m_Flags28 = 0x100;
    if (!interfaceObject.CGameScriptInterface::
            IsEntityWieldingWeapon(scriptThing))
        return Fail("active found weapon");

    ResetObservations();
    foundNode.m_Key = 0x47;
    g_pLowerBoundResult = &foundNode;
    if (!interfaceObject.CGameScriptInterface::
            IsEntityWieldingWeapon(scriptThing) ||
        g_pObservedComponent != &endComponent)
        return Fail("greater-key sentinel");

    ResetObservations();
    g_pLowerBoundResult = &endNode;
    if (!interfaceObject.CGameScriptInterface::
            IsEntityWieldingWeapon(scriptThing) ||
        g_pObservedComponent != &endComponent)
        return Fail("direct sentinel");

    std::printf("FSE2_008974f0_TEST PASS\n");
    return 0;
}
