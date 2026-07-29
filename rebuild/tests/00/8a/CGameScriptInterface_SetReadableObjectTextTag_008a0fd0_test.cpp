#include <cstdio>

class CScriptThing
{
};

class CGSIReadable_Node;

class CGSIReadable_Temporary
{
public:
    CGSIReadable_Temporary();
    ~CGSIReadable_Temporary();
    void Destruct();

    unsigned long m_Data;
};

class CCharString
{
public:
    CGSIReadable_Temporary BuildTemporary() const;

    unsigned long m_Data;
};

class CGSIReadable_Container
{
public:
    CGSIReadable_Node* LowerBound(const long* pKey);

    CGSIReadable_Node* m_pFirst;
    CGSIReadable_Node* m_pEnd;
};

class CGSIReadable_Destination
{
public:
    void Assign(const CGSIReadable_Temporary& value);
};

class CGSIReadable_Component
{
public:
    unsigned char m_Pad00[0x18];
    CGSIReadable_Destination m_Destination;
};

class CGSIReadable_Node
{
public:
    long m_Key;
    CGSIReadable_Component* m_pComponent;
};

class CGameScriptInterface
{
public:
    virtual void SetReadableObjectTextTag(
        const CScriptThing& entity,
        const CCharString& textTag) const;
};

struct ScriptThingStorage
{
    void** m_pVTable;
};

struct EntityStorage
{
    unsigned char m_Pad00[0x20];
    unsigned long m_Flags20;
    unsigned char m_Pad24[0x20];
    CGSIReadable_Container m_Container;
    unsigned char m_Pad4C[0x45];
    unsigned char m_Flags91;
};

static void* g_ScriptThingVTable[12];
static EntityStorage* g_pEntity;
static CGSIReadable_Node* g_pLowerBoundResult;
static CGSIReadable_Container* g_pObservedContainer;
static const CCharString* g_pObservedString;
static CGSIReadable_Destination* g_pObservedDestination;
static CGSIReadable_Temporary* g_pObservedAssignedTemporary;
static CGSIReadable_Temporary* g_pObservedDestructedTemporary;
static unsigned long g_AssignedData;
static long g_ObservedKey;
static unsigned long g_LowerBoundCalls;
static unsigned long g_BuildCalls;
static unsigned long g_AssignCalls;
static unsigned long g_DestructCalls;

static EntityStorage* __fastcall ResolveEntity(
    ScriptThingStorage*)
{
    return g_pEntity;
}

CGSIReadable_Temporary::CGSIReadable_Temporary()
    : m_Data(0)
{
}

CGSIReadable_Temporary::~CGSIReadable_Temporary()
{
}

void CGSIReadable_Temporary::Destruct()
{
    g_pObservedDestructedTemporary = this;
    ++g_DestructCalls;
}

CGSIReadable_Temporary
CCharString::BuildTemporary() const
{
    g_pObservedString = this;
    ++g_BuildCalls;
    CGSIReadable_Temporary result;
    result.m_Data = m_Data;
    return result;
}

CGSIReadable_Node*
CGSIReadable_Container::LowerBound(const long* pKey)
{
    g_pObservedContainer = this;
    g_ObservedKey = *pKey;
    ++g_LowerBoundCalls;
    return g_pLowerBoundResult;
}

void CGSIReadable_Destination::Assign(
    const CGSIReadable_Temporary& value)
{
    g_pObservedDestination = this;
    g_pObservedAssignedTemporary =
        const_cast<CGSIReadable_Temporary*>(&value);
    g_AssignedData = value.m_Data;
    ++g_AssignCalls;
}

static void ResetObservations()
{
    g_pObservedContainer = 0;
    g_pObservedString = 0;
    g_pObservedDestination = 0;
    g_pObservedAssignedTemporary = 0;
    g_pObservedDestructedTemporary = 0;
    g_AssignedData = 0;
    g_ObservedKey = 0;
    g_LowerBoundCalls = 0;
    g_BuildCalls = 0;
    g_AssignCalls = 0;
    g_DestructCalls = 0;
}

static int Fail(const char* pMessage)
{
    std::printf(
        "FSE2_008a0fd0_TEST FAIL: %s\n",
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
    CCharString textTag = {0xCAFEBABE};
    EntityStorage entity = {};
    CGSIReadable_Component foundComponent;
    CGSIReadable_Component endComponent;
    CGSIReadable_Node foundNode = {};
    CGSIReadable_Node endNode = {};
    CGameScriptInterface interfaceObject;

    foundNode.m_pComponent = &foundComponent;
    endNode.m_pComponent = &endComponent;
    entity.m_Container.m_pEnd = &endNode;

    ResetObservations();
    g_pEntity = 0;
    interfaceObject.CGameScriptInterface::
        SetReadableObjectTextTag(scriptThing, textTag);
    if (g_LowerBoundCalls != 0 || g_BuildCalls != 0)
        return Fail("null entity guard");

    g_pEntity = &entity;
    entity.m_Flags20 = 0x2000;

    ResetObservations();
    entity.m_Flags91 = 1;
    interfaceObject.CGameScriptInterface::
        SetReadableObjectTextTag(scriptThing, textTag);
    if (g_BuildCalls != 0)
        return Fail("dead entity guard");

    ResetObservations();
    entity.m_Flags91 = 0;
    entity.m_Flags20 = 0;
    interfaceObject.CGameScriptInterface::
        SetReadableObjectTextTag(scriptThing, textTag);
    if (g_BuildCalls != 0)
        return Fail("readable capability guard");

    ResetObservations();
    entity.m_Flags20 = 0x2000;
    foundNode.m_Key = 0x0D;
    g_pLowerBoundResult = &foundNode;
    interfaceObject.CGameScriptInterface::
        SetReadableObjectTextTag(scriptThing, textTag);
    if (g_pObservedContainer != &entity.m_Container ||
        g_ObservedKey != 0x0D ||
        g_pObservedString != &textTag ||
        g_pObservedDestination !=
            &foundComponent.m_Destination ||
        g_AssignedData != 0xCAFEBABE ||
        g_LowerBoundCalls != 1 || g_BuildCalls != 1 ||
        g_AssignCalls != 1 || g_DestructCalls != 1 ||
        g_pObservedAssignedTemporary == 0 ||
        g_pObservedAssignedTemporary !=
            g_pObservedDestructedTemporary)
        return Fail("found component assignment sequence");

    ResetObservations();
    foundNode.m_Key = 0x0E;
    g_pLowerBoundResult = &foundNode;
    interfaceObject.CGameScriptInterface::
        SetReadableObjectTextTag(scriptThing, textTag);
    if (g_pObservedDestination !=
            &endComponent.m_Destination)
        return Fail("greater-key sentinel");

    ResetObservations();
    g_pLowerBoundResult = &endNode;
    interfaceObject.CGameScriptInterface::
        SetReadableObjectTextTag(scriptThing, textTag);
    if (g_pObservedDestination !=
            &endComponent.m_Destination)
        return Fail("direct sentinel");

    std::printf("FSE2_008a0fd0_TEST PASS\n");
    return 0;
}
