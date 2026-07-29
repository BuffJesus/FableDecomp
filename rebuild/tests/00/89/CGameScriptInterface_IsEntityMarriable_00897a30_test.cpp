#include <cstdio>

class CScriptThing
{
};

class CGSIMarriable_Node;

class CGSIMarriable_Container
{
public:
    CGSIMarriable_Node* LowerBound(const long* pKey);

    CGSIMarriable_Node* m_pFirst;
    CGSIMarriable_Node* m_pEnd;
};

class CGSIMarriable_Component
{
public:
    bool IsMarriable();
};

class CGSIMarriable_Node
{
public:
    long m_Key;
    CGSIMarriable_Component* m_pComponent;
};

class CGameScriptInterface
{
public:
    virtual bool IsEntityMarriable(
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
    CGSIMarriable_Container m_Container;
    unsigned char m_Pad4C[0x45];
    unsigned char m_Flags91;
};

static void* g_ScriptThingVTable[12];
static EntityStorage* g_pEntity;
static CGSIMarriable_Node* g_pLowerBoundResult;
static bool g_MarriableResult;
static ScriptThingStorage* g_pObservedScriptThing;
static CGSIMarriable_Container* g_pObservedContainer;
static CGSIMarriable_Component* g_pObservedComponent;
static long g_ObservedKey;
static unsigned long g_ResolveCalls;
static unsigned long g_LowerBoundCalls;
static unsigned long g_ComponentCalls;

static EntityStorage* __fastcall ResolveEntity(
    ScriptThingStorage* pScriptThing)
{
    g_pObservedScriptThing = pScriptThing;
    ++g_ResolveCalls;
    return g_pEntity;
}

CGSIMarriable_Node*
CGSIMarriable_Container::LowerBound(const long* pKey)
{
    g_pObservedContainer = this;
    g_ObservedKey = *pKey;
    ++g_LowerBoundCalls;
    return g_pLowerBoundResult;
}

bool CGSIMarriable_Component::IsMarriable()
{
    g_pObservedComponent = this;
    ++g_ComponentCalls;
    return g_MarriableResult;
}

static void ResetObservations()
{
    g_pObservedScriptThing = 0;
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
        "FSE2_00897a30_TEST FAIL: %s\n",
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
    CGSIMarriable_Component foundComponent;
    CGSIMarriable_Component endComponent;
    CGSIMarriable_Node foundNode = {};
    CGSIMarriable_Node endNode = {};
    CGameScriptInterface interfaceObject;

    foundNode.m_pComponent = &foundComponent;
    endNode.m_pComponent = &endComponent;
    entity.m_Container.m_pEnd = &endNode;

    ResetObservations();
    g_pEntity = 0;
    if (interfaceObject.CGameScriptInterface::
            IsEntityMarriable(scriptThing))
        return Fail("null entity");
    if (g_ResolveCalls != 1 || g_LowerBoundCalls != 0)
        return Fail("null entity calls");

    g_pEntity = &entity;
    entity.m_Type10 = 1;
    entity.m_Flags38 = 0x800000;

    ResetObservations();
    entity.m_Flags91 = 1;
    if (interfaceObject.CGameScriptInterface::
            IsEntityMarriable(scriptThing))
        return Fail("dead entity guard");

    ResetObservations();
    entity.m_Flags91 = 0;
    entity.m_Type10 = 2;
    if (interfaceObject.CGameScriptInterface::
            IsEntityMarriable(scriptThing))
        return Fail("entity type guard");

    ResetObservations();
    entity.m_Type10 = 1;
    entity.m_Flags38 = 0;
    if (interfaceObject.CGameScriptInterface::
            IsEntityMarriable(scriptThing))
        return Fail("creature capability guard");

    ResetObservations();
    entity.m_Flags38 = 0x800000;
    foundNode.m_Key = 0xD7;
    g_pLowerBoundResult = &foundNode;
    g_MarriableResult = false;
    if (interfaceObject.CGameScriptInterface::
            IsEntityMarriable(scriptThing))
        return Fail("component false");
    if (g_pObservedContainer != &entity.m_Container ||
        g_ObservedKey != 0xD7 ||
        g_pObservedComponent != &foundComponent ||
        g_LowerBoundCalls != 1 || g_ComponentCalls != 1)
        return Fail("found component dispatch");

    ResetObservations();
    foundNode.m_Key = 0xD8;
    g_pLowerBoundResult = &foundNode;
    g_MarriableResult = true;
    if (!interfaceObject.CGameScriptInterface::
            IsEntityMarriable(scriptThing))
        return Fail("sentinel component true");
    if (g_pObservedComponent != &endComponent)
        return Fail("greater-key sentinel selection");

    ResetObservations();
    g_pLowerBoundResult = &endNode;
    if (!interfaceObject.CGameScriptInterface::
            IsEntityMarriable(scriptThing))
        return Fail("direct sentinel true");

    std::printf("FSE2_00897a30_TEST PASS\n");
    return 0;
}
