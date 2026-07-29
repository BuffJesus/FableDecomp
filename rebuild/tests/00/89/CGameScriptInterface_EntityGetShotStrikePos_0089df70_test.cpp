#include <cstdio>

class CScriptThing
{
};

class C3DVector
{
public:
    unsigned long m_X;
    unsigned long m_Y;
    unsigned long m_Z;
};

class CGSIShotStrike_Events
{
public:
    bool GetHitEvent(void* pEvent, void* pFilter);
};

class CGSIShotStrike_Node
{
public:
    long m_Key;
    CGSIShotStrike_Events* m_pEvents;
};

class CGSIShotStrike_Container
{
public:
    CGSIShotStrike_Node* LowerBound(const long* pKey);

    CGSIShotStrike_Node* m_pFirst;
    CGSIShotStrike_Node* m_pEnd;
};

class CGameScriptInterface
{
public:
    virtual bool EntityGetShotStrikePos(
        const CScriptThing& entity,
        C3DVector& outStrikePosition) const;
};

struct ThingVftable
{
    unsigned char m_Pad00[0x2C];
    void* m_GetTarget;
};

struct ThingStorage
{
    ThingVftable* m_pVftable;
};

struct EntityStorage
{
    unsigned char m_Pad00[0x28];
    unsigned char m_Flags28;
    unsigned char m_Pad29[0x1B];
    CGSIShotStrike_Container m_Container;
    unsigned char m_Pad4C[0x45];
    unsigned char m_Flags91;
};

struct HitEventBuffer
{
    long m_InterfaceType;
    unsigned long m_Pad04;
    C3DVector m_StrikePosition;
};

static EntityStorage* g_pEntity;
static CGSIShotStrike_Node* g_pLowerBoundResult;
static bool g_EventResult;
static C3DVector g_EventStrikePosition;
static HitEventBuffer g_HitEvent;
static unsigned long g_GetTargetCalls;
static unsigned long g_LowerBoundCalls;
static unsigned long g_GetEventCalls;
static long g_ObservedKey;

static EntityStorage* __fastcall GetTarget(CScriptThing*)
{
    ++g_GetTargetCalls;
    return g_pEntity;
}

CGSIShotStrike_Node*
CGSIShotStrike_Container::LowerBound(const long* pKey)
{
    ++g_LowerBoundCalls;
    g_ObservedKey = *pKey;
    return g_pLowerBoundResult;
}

bool CGSIShotStrike_Events::GetHitEvent(
    void* pEvent,
    void* pFilter)
{
    ++g_GetEventCalls;
    if (pEvent != pFilter)
        std::printf("event/filter pointer mismatch\n");
    if (*reinterpret_cast<long*>(pFilter) != 0x42)
        std::printf("event interface type mismatch\n");
    g_HitEvent.m_StrikePosition = g_EventStrikePosition;
    *reinterpret_cast<HitEventBuffer**>(pEvent) = &g_HitEvent;
    return g_EventResult;
}

static int Fail(const char* pMessage)
{
    std::printf("FSE2_0089df70_TEST FAIL: %s\n", pMessage);
    return 1;
}

static void ResetCounts()
{
    g_GetTargetCalls = 0;
    g_LowerBoundCalls = 0;
    g_GetEventCalls = 0;
    g_ObservedKey = 0;
}

int main()
{
    ThingVftable thingVftable = {};
    ThingStorage thingStorage;
    const CScriptThing& scriptThing =
        reinterpret_cast<const CScriptThing&>(thingStorage);
    EntityStorage entity = {};
    CGSIShotStrike_Events foundEvents;
    CGSIShotStrike_Events endEvents;
    CGSIShotStrike_Node foundNode = {};
    CGSIShotStrike_Node endNode = {};
    C3DVector output = {0xAAAAAAAA, 0xBBBBBBBB, 0xCCCCCCCC};
    CGameScriptInterface interfaceObject;

    thingVftable.m_GetTarget = reinterpret_cast<void*>(GetTarget);
    thingStorage.m_pVftable = &thingVftable;
    foundNode.m_pEvents = &foundEvents;
    endNode.m_pEvents = &endEvents;
    entity.m_Container.m_pEnd = &endNode;

    ResetCounts();
    g_pEntity = 0;
    if (interfaceObject.CGameScriptInterface::
            EntityGetShotStrikePos(scriptThing, output))
        return Fail("null entity");
    if (g_LowerBoundCalls != 0)
        return Fail("null entity lookup");

    ResetCounts();
    g_pEntity = &entity;
    entity.m_Flags91 = 1;
    entity.m_Flags28 = 4;
    if (interfaceObject.CGameScriptInterface::
            EntityGetShotStrikePos(scriptThing, output))
        return Fail("flags91 guard");

    ResetCounts();
    entity.m_Flags91 = 0;
    entity.m_Flags28 = 0;
    if (interfaceObject.CGameScriptInterface::
            EntityGetShotStrikePos(scriptThing, output))
        return Fail("flags28 guard");

    ResetCounts();
    entity.m_Flags28 = 4;
    foundNode.m_Key = 0x42;
    g_pLowerBoundResult = &foundNode;
    g_EventResult = false;
    if (interfaceObject.CGameScriptInterface::
            EntityGetShotStrikePos(scriptThing, output))
        return Fail("event failure");
    if (g_LowerBoundCalls != 1 || g_ObservedKey != 0x42 ||
        g_GetEventCalls != 1)
        return Fail("event failure calls");
    if (output.m_X != 0xAAAAAAAA || output.m_Y != 0xBBBBBBBB ||
        output.m_Z != 0xCCCCCCCC)
        return Fail("failure modified output");

    ResetCounts();
    foundNode.m_Key = 0x43;
    g_pLowerBoundResult = &foundNode;
    g_EventResult = true;
    g_EventStrikePosition.m_X = 0x7FC12345;
    g_EventStrikePosition.m_Y = 0x80000000;
    g_EventStrikePosition.m_Z = 0x7F800000;
    if (!interfaceObject.CGameScriptInterface::
            EntityGetShotStrikePos(scriptThing, output))
        return Fail("successful sentinel event");
    if (output.m_X != g_EventStrikePosition.m_X ||
        output.m_Y != g_EventStrikePosition.m_Y ||
        output.m_Z != g_EventStrikePosition.m_Z)
        return Fail("raw strike copy");

    ResetCounts();
    g_pLowerBoundResult = &endNode;
    if (!interfaceObject.CGameScriptInterface::
            EntityGetShotStrikePos(scriptThing, output))
        return Fail("direct end sentinel");

    std::printf("FSE2_0089df70_TEST PASS\n");
    return 0;
}
