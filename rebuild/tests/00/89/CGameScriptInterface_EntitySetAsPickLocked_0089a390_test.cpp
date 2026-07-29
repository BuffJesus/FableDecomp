#include <cstdio>

enum ETCInterfaceType { ETC_DUMMY = 0 };

struct CScriptThing;
struct CEntityOverlay;
struct CHeroOverlay;

struct CScriptThingVTable
{
    unsigned char m_Pad00[0x2c];
    CEntityOverlay* (__fastcall* m_Resolve)(CScriptThing* self);
    unsigned char m_Pad30[0xfc];
    bool (__fastcall* m_IsAvailable)(CScriptThing* self);
};

struct CScriptThing
{
    CScriptThingVTable* m_pVTable;
};

class CGSIPickLockedEvent
{
public:
    CGSIPickLockedEvent* Initialize(CHeroOverlay* pHero);
};

class CGSIEntityEvents
{
public:
    void AddEvent(CGSIPickLockedEvent* pEvent);
};

struct CKeyPair
{
    ETCInterfaceType m_Key;
    void*            m_Value;
};

struct CVectorMap
{
    CKeyPair* m_pBegin;
    CKeyPair* m_pEnd;
    CKeyPair* LowerBound(const ETCInterfaceType* pKey);
};

struct CEntityOverlay
{
    unsigned char m_Pad00[0x28];
    unsigned int  m_Flags28;
    unsigned char m_Pad2C[0x18];
    CVectorMap    m_InterfaceMap44;
};

struct CHeroOverlay
{
    unsigned char m_Pad00[0x91];
    unsigned char m_Flags91;
};

struct CMessageContext
{
    unsigned char m_Pad00[0x60];
    void*         m_MessageManager;
};

struct CGameScriptInterface
{
    void**           m_pVTable;
    CMessageContext* m_MessageContext;
    unsigned char    m_Pad08[0x0c];
    void*            m_Field14;
};

static bool g_available;
static CEntityOverlay* g_entity;
static CHeroOverlay* g_hero;
static unsigned long g_pre_a_calls;
static unsigned long g_pre_b_calls;
static unsigned long g_resolve_calls;
static unsigned long g_message_calls;
static bool g_message_vector_zero;
static CEntityOverlay* g_message_entity;
static CKeyPair* g_lower_bound_result;
static CVectorMap* g_observed_map;
static ETCInterfaceType g_observed_key;
static void* g_allocation;
static unsigned long g_allocate_size;
static CGSIPickLockedEvent* g_initialized_event;
static CHeroOverlay* g_initialized_hero;
static CGSIEntityEvents* g_observed_events;
static CGSIPickLockedEvent* g_added_event;
static unsigned long g_add_event_calls;

static bool __fastcall IsAvailable(CScriptThing*)
{
    return g_available;
}

static CEntityOverlay* __fastcall Resolve(CScriptThing*)
{
    ++g_resolve_calls;
    return g_entity;
}

static void __fastcall PreA(
    CGameScriptInterface*,
    int,
    CScriptThing*)
{
    ++g_pre_a_calls;
}

static void __fastcall PreB(
    CGameScriptInterface*,
    int,
    CScriptThing*)
{
    ++g_pre_b_calls;
}

extern "C" void __stdcall Sub_AddMessage(
    long type,
    long value,
    void* pVector,
    long zero1,
    long zero2,
    long zero3,
    long zero4,
    CEntityOverlay* pEntity)
{
    ++g_message_calls;
    const long* pValues = (const long*)pVector;
    g_message_vector_zero =
        type == 0x46 &&
        value == 0x32 &&
        pValues[0] == 0 &&
        pValues[1] == 0 &&
        pValues[2] == 0 &&
        zero1 == 0 &&
        zero2 == 0 &&
        zero3 == 0 &&
        zero4 == 0;
    g_message_entity = pEntity;
}

extern "C" void* __fastcall Sub_ResolveA(void* self)
{
    return self;
}

extern "C" CHeroOverlay* __fastcall Sub_ResolveB(void* obj)
{
    (void)obj;
    return g_hero;
}

extern "C" void* __cdecl Sub_Allocate(unsigned long size)
{
    g_allocate_size = size;
    return g_allocation;
}

const ETCInterfaceType kTargetType = (ETCInterfaceType)0x42;

CKeyPair* CVectorMap::LowerBound(const ETCInterfaceType* pKey)
{
    g_observed_map = this;
    g_observed_key = *pKey;
    return g_lower_bound_result;
}

CGSIPickLockedEvent* CGSIPickLockedEvent::Initialize(
    CHeroOverlay* pHero)
{
    g_initialized_event = this;
    g_initialized_hero = pHero;
    return this;
}

void CGSIEntityEvents::AddEvent(CGSIPickLockedEvent* pEvent)
{
    g_observed_events = this;
    g_added_event = pEvent;
    ++g_add_event_calls;
}

void __fastcall CGameScriptInterface_EntitySetAsPickLocked(
    CGameScriptInterface* self,
    int edxUnused,
    CScriptThing* pScriptThing);

static void ResetObservations()
{
    g_pre_a_calls = 0;
    g_pre_b_calls = 0;
    g_resolve_calls = 0;
    g_message_calls = 0;
    g_message_vector_zero = false;
    g_message_entity = 0;
    g_lower_bound_result = 0;
    g_observed_map = 0;
    g_observed_key = ETC_DUMMY;
    g_allocation = 0;
    g_allocate_size = 0;
    g_initialized_event = 0;
    g_initialized_hero = 0;
    g_observed_events = 0;
    g_added_event = 0;
    g_add_event_calls = 0;
}

static int Fail(const char* message)
{
    std::printf("CGameScriptInterface_0089a390_TEST FAIL: %s\n", message);
    return 1;
}

int main()
{
    void* interfaceVTable[430] = {};
    CScriptThingVTable scriptVTable = {};
    CScriptThing scriptThing = {};
    CMessageContext messageContext = {};
    CGameScriptInterface gsi = {};
    CEntityOverlay entity = {};
    CHeroOverlay hero = {};
    CGSIEntityEvents foundEvents;
    CGSIEntityEvents endEvents;
    CKeyPair foundEntry = {};
    CKeyPair greaterEntry = {};
    CKeyPair endEntry = {};
    unsigned char eventStorage[0x10] = {};

    interfaceVTable[0x6a4 / 4] = (void*)PreA;
    interfaceVTable[0x6b4 / 4] = (void*)PreB;
    scriptVTable.m_Resolve = Resolve;
    scriptVTable.m_IsAvailable = IsAvailable;
    scriptThing.m_pVTable = &scriptVTable;
    gsi.m_pVTable = interfaceVTable;
    gsi.m_MessageContext = &messageContext;
    gsi.m_Field14 = (void*)0x1234;
    messageContext.m_MessageManager = (void*)0x5678;
    g_entity = &entity;
    entity.m_InterfaceMap44.m_pEnd = &endEntry;
    foundEntry.m_Value = &foundEvents;
    endEntry.m_Value = &endEvents;

    ResetObservations();
    g_available = false;
    CGameScriptInterface_EntitySetAsPickLocked(
        &gsi, 0, &scriptThing);
    if (g_pre_a_calls != 0 ||
        g_pre_b_calls != 0 ||
        g_resolve_calls != 0 ||
        g_message_calls != 0)
        return Fail("unavailable script thing guard");

    ResetObservations();
    g_available = true;
    g_hero = 0;
    CGameScriptInterface_EntitySetAsPickLocked(
        &gsi, 0, &scriptThing);
    if (g_pre_a_calls != 1 ||
        g_pre_b_calls != 1 ||
        g_resolve_calls != 1 ||
        g_message_calls != 1 ||
        !g_message_vector_zero ||
        g_message_entity != &entity ||
        g_add_event_calls != 0)
        return Fail("common notifications before null hero");

    ResetObservations();
    g_hero = &hero;
    hero.m_Flags91 = 1;
    entity.m_Flags28 = 4;
    CGameScriptInterface_EntitySetAsPickLocked(
        &gsi, 0, &scriptThing);
    if (g_add_event_calls != 0)
        return Fail("dead hero guard");

    ResetObservations();
    hero.m_Flags91 = 0;
    entity.m_Flags28 = 0;
    CGameScriptInterface_EntitySetAsPickLocked(
        &gsi, 0, &scriptThing);
    if (g_add_event_calls != 0 || g_observed_map != 0)
        return Fail("missing entity-events capability");

    ResetObservations();
    entity.m_Flags28 = 4;
    foundEntry.m_Key = (ETCInterfaceType)0x42;
    g_lower_bound_result = &foundEntry;
    g_allocation = eventStorage;
    CGameScriptInterface_EntitySetAsPickLocked(
        &gsi, 0, &scriptThing);
    if (g_observed_map != &entity.m_InterfaceMap44 ||
        (int)g_observed_key != 0x42 ||
        g_allocate_size != 0x10 ||
        g_initialized_event !=
            (CGSIPickLockedEvent*)eventStorage ||
        g_initialized_hero != &hero ||
        g_observed_events != &foundEvents ||
        g_added_event != (CGSIPickLockedEvent*)eventStorage ||
        g_add_event_calls != 1)
        return Fail("allocated event dispatch");

    ResetObservations();
    greaterEntry.m_Key = (ETCInterfaceType)0x43;
    greaterEntry.m_Value = &foundEvents;
    g_lower_bound_result = &greaterEntry;
    g_allocation = 0;
    CGameScriptInterface_EntitySetAsPickLocked(
        &gsi, 0, &scriptThing);
    if (g_observed_events != &endEvents ||
        g_added_event != 0 ||
        g_add_event_calls != 1)
        return Fail("sentinel allocation-failure dispatch");

    std::printf("CGameScriptInterface_0089a390_TEST PASS\n");
    return 0;
}
