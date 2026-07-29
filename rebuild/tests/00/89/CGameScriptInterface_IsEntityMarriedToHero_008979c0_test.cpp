#include <cstdio>

class CScriptThing
{
};

class CGSIMarried_Node;

class CGSIMarried_Container
{
public:
    CGSIMarried_Node* LowerBound(const long* pKey);

    CGSIMarried_Node* m_pFirst;
    CGSIMarried_Node* m_pEnd;
};

class CGSIMarried_Component
{
public:
    bool IsMarriedToHero();
};

class CGSIMarried_Node
{
public:
    long m_Key;
    CGSIMarried_Component* m_pComponent;
};

class CGameScriptInterface
{
public:
    virtual bool IsEntityMarriedToHero(
        const CScriptThing& entity) const;
};

struct ScriptThingStorage
{
    void** m_pVTable;
};

struct EntityStorage
{
    unsigned char m_Pad00[0x10];
    unsigned char m_Type10;
    unsigned char m_Pad11[0x27];
    unsigned long m_Flags38;
    unsigned char m_Pad3C[0x08];
    CGSIMarried_Container m_Container;
    unsigned char m_Pad4C[0x45];
    unsigned char m_Flags91;
};

static void* g_ScriptThingVTable[12];
static EntityStorage* g_pEntity;
static CGSIMarried_Node* g_pLowerBoundResult;
static bool g_MarriedResult;
static CGSIMarried_Container* g_pObservedContainer;
static CGSIMarried_Component* g_pObservedComponent;
static long g_ObservedKey;
static unsigned long g_ResolveCalls;
static unsigned long g_LowerBoundCalls;
static unsigned long g_ComponentCalls;

static EntityStorage* __fastcall ResolveEntity(
    ScriptThingStorage*)
{
    ++g_ResolveCalls;
    return g_pEntity;
}

CGSIMarried_Node*
CGSIMarried_Container::LowerBound(const long* pKey)
{
    g_pObservedContainer = this;
    g_ObservedKey = *pKey;
    ++g_LowerBoundCalls;
    return g_pLowerBoundResult;
}

bool CGSIMarried_Component::IsMarriedToHero()
{
    g_pObservedComponent = this;
    ++g_ComponentCalls;
    return g_MarriedResult;
}

static void ResetObservations()
{
    g_pObservedContainer = 0;
    g_pObservedComponent = 0;
    g_ObservedKey = 0;
    g_ResolveCalls = 0;
    g_LowerBoundCalls = 0;
    g_ComponentCalls = 0;
}

static int Fail(const char* pMessage)
{
    std::printf(
        "FSE2_008979c0_TEST FAIL: %s\n",
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
    CGSIMarried_Component foundComponent;
    CGSIMarried_Component endComponent;
    CGSIMarried_Node foundNode = {};
    CGSIMarried_Node endNode = {};
    CGameScriptInterface interfaceObject;

    foundNode.m_pComponent = &foundComponent;
    endNode.m_pComponent = &endComponent;
    entity.m_Container.m_pEnd = &endNode;

    ResetObservations();
    g_pEntity = 0;
    if (interfaceObject.CGameScriptInterface::
            IsEntityMarriedToHero(scriptThing) ||
        g_ResolveCalls != 1 || g_LowerBoundCalls != 0)
        return Fail("null entity guard");

    g_pEntity = &entity;
    entity.m_Type10 = 1;
    entity.m_Flags38 = 0x800000;

    ResetObservations();
    entity.m_Flags91 = 1;
    if (interfaceObject.CGameScriptInterface::
            IsEntityMarriedToHero(scriptThing))
        return Fail("dead entity guard");

    ResetObservations();
    entity.m_Flags91 = 0;
    entity.m_Type10 = 2;
    if (interfaceObject.CGameScriptInterface::
            IsEntityMarriedToHero(scriptThing))
        return Fail("entity type guard");

    ResetObservations();
    entity.m_Type10 = 1;
    entity.m_Flags38 = 0;
    if (interfaceObject.CGameScriptInterface::
            IsEntityMarriedToHero(scriptThing))
        return Fail("creature capability guard");

    ResetObservations();
    entity.m_Flags38 = 0x800000;
    foundNode.m_Key = 0xD7;
    g_pLowerBoundResult = &foundNode;
    g_MarriedResult = false;
    if (interfaceObject.CGameScriptInterface::
            IsEntityMarriedToHero(scriptThing) ||
        g_pObservedContainer != &entity.m_Container ||
        g_ObservedKey != 0xD7 ||
        g_pObservedComponent != &foundComponent ||
        g_ComponentCalls != 1)
        return Fail("found component false");

    ResetObservations();
    foundNode.m_Key = 0xD8;
    g_pLowerBoundResult = &foundNode;
    g_MarriedResult = true;
    if (!interfaceObject.CGameScriptInterface::
            IsEntityMarriedToHero(scriptThing) ||
        g_pObservedComponent != &endComponent)
        return Fail("greater-key sentinel true");

    ResetObservations();
    g_pLowerBoundResult = &endNode;
    if (!interfaceObject.CGameScriptInterface::
            IsEntityMarriedToHero(scriptThing) ||
        g_pObservedComponent != &endComponent)
        return Fail("direct sentinel true");

    std::printf("FSE2_008979c0_TEST PASS\n");
    return 0;
}
