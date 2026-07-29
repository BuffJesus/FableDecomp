#include <cstdio>

class CScriptThing
{
};

struct CGSIForceLookCamera_Destination
{
    unsigned char m_Pad00[0x34];
    unsigned char m_Enabled;
    unsigned char m_After;
};

class CGSIForceLookCamera_Node
{
public:
    long m_Key;
    CGSIForceLookCamera_Destination* m_pValue;
};

class CGSIForceLookCamera_Container
{
public:
    CGSIForceLookCamera_Node* LowerBound(const long* pKey);

    CGSIForceLookCamera_Node* m_pFirst;
    CGSIForceLookCamera_Node* m_pEnd;
};

class CGameScriptInterface
{
public:
    virtual void EntityForceToLookAtCamera(
        const CScriptThing& entity,
        bool enabled) const;
};

struct ThingVftable
{
    unsigned char m_Pad00[0x2C];
    void* m_GetTarget;
    unsigned char m_Pad30[0x12C - 0x30];
    void* m_IsValid;
};

struct ThingStorage
{
    ThingVftable* m_pVftable;
};

struct TargetStorage
{
    unsigned char m_Pad00[0x20];
    unsigned long m_Flags;
    unsigned char m_Pad24[0x20];
    CGSIForceLookCamera_Container m_Container;
};

static bool g_IsValid;
static TargetStorage* g_pTarget;
static CGSIForceLookCamera_Node* g_pLowerBoundResult;
static unsigned long g_IsValidCalls;
static unsigned long g_GetTargetCalls;
static unsigned long g_LowerBoundCalls;
static long g_ObservedKey;

static bool __fastcall IsValid(CScriptThing*)
{
    ++g_IsValidCalls;
    return g_IsValid;
}

static TargetStorage* __fastcall GetTarget(CScriptThing*)
{
    ++g_GetTargetCalls;
    return g_pTarget;
}

CGSIForceLookCamera_Node*
CGSIForceLookCamera_Container::LowerBound(const long* pKey)
{
    ++g_LowerBoundCalls;
    g_ObservedKey = *pKey;
    return g_pLowerBoundResult;
}

static void CallWithRawBool(
    const CGameScriptInterface* pInterface,
    const CScriptThing* pThing,
    unsigned char value)
{
    __asm
    {
        mov ecx, pInterface
        movzx eax, value
        push eax
        push pThing
        call CGameScriptInterface::EntityForceToLookAtCamera
    }
}

static int Fail(const char* pMessage)
{
    std::printf("FSE2_0089d8b0_TEST FAIL: %s\n", pMessage);
    return 1;
}

static void ResetCounts()
{
    g_IsValidCalls = 0;
    g_GetTargetCalls = 0;
    g_LowerBoundCalls = 0;
    g_ObservedKey = 0;
}

int main()
{
    ThingVftable thingVftable = {};
    ThingStorage thingStorage;
    const CScriptThing& scriptThing =
        reinterpret_cast<const CScriptThing&>(thingStorage);
    TargetStorage target = {};
    CGSIForceLookCamera_Destination foundDestination = {};
    CGSIForceLookCamera_Destination endDestination = {};
    CGSIForceLookCamera_Node foundNode = {};
    CGSIForceLookCamera_Node endNode = {};
    CGameScriptInterface interfaceObject;

    thingVftable.m_IsValid = reinterpret_cast<void*>(IsValid);
    thingVftable.m_GetTarget = reinterpret_cast<void*>(GetTarget);
    thingStorage.m_pVftable = &thingVftable;
    g_pTarget = &target;
    foundNode.m_pValue = &foundDestination;
    endNode.m_pValue = &endDestination;
    target.m_Container.m_pEnd = &endNode;
    foundDestination.m_After = 0xA5;
    endDestination.m_After = 0x5A;

    ResetCounts();
    g_IsValid = false;
    CallWithRawBool(&interfaceObject, &scriptThing, 0x71);
    if (g_IsValidCalls != 1 || g_GetTargetCalls != 0 ||
        g_LowerBoundCalls != 0)
        return Fail("invalid thing guard");

    ResetCounts();
    g_IsValid = true;
    target.m_Flags = 0;
    CallWithRawBool(&interfaceObject, &scriptThing, 0x72);
    if (g_GetTargetCalls != 1 || g_LowerBoundCalls != 0)
        return Fail("capability guard");

    ResetCounts();
    target.m_Flags = 0x100;
    foundNode.m_Key = 8;
    g_pLowerBoundResult = &foundNode;
    CallWithRawBool(&interfaceObject, &scriptThing, 0x7E);
    if (g_LowerBoundCalls != 1 || g_ObservedKey != 8 ||
        foundDestination.m_Enabled != 0x7E ||
        foundDestination.m_After != 0xA5)
        return Fail("eligible node or raw bool");

    ResetCounts();
    foundNode.m_Key = 9;
    g_pLowerBoundResult = &foundNode;
    CallWithRawBool(&interfaceObject, &scriptThing, 0x6D);
    if (endDestination.m_Enabled != 0x6D ||
        endDestination.m_After != 0x5A)
        return Fail("greater-key sentinel fallback");

    ResetCounts();
    g_pLowerBoundResult = &endNode;
    CallWithRawBool(&interfaceObject, &scriptThing, 0x3C);
    if (endDestination.m_Enabled != 0x3C)
        return Fail("end sentinel selection");

    std::printf("FSE2_0089d8b0_TEST PASS\n");
    return 0;
}
