// Real-object ABI/behavior fixture for
// NUISystem::CFrontEndManager::ResetSlidersToProfileValues @ 0x00595356.

#include <cstdio>
#include <cstring>

extern "C" void __fastcall
CFrontEndManager_ResetSlidersToProfileValues_00595356(void*, void*);

struct ListNode
{
    ListNode* next;
    ListNode* previous;
    void* component;
};

struct ListProbe
{
    ListNode* head;
    unsigned long size;
};

struct CollectorProbe
{
    void** vtable;
};

enum Event
{
    kInitialise = 1,
    kTop,
    kCollect15,
    kAdjust15,
    kClear,
    kCollect16,
    kAdjust16,
    kDestroy
};

static Event g_events[16];
static int g_eventCount;
static void* g_expectedStack;
static void* g_topComponent;
static int g_topCalls;
static ListProbe* g_list;
static ListNode g_sentinel15;
static ListNode g_nodes15[2];
static ListNode g_sentinel16;
static ListNode g_nodes16[2];
static void* g_group15Component;
static void* g_group16Component;
static bool g_populate;
static unsigned long g_collectedKeys[2];
static int g_collectCalls;
static void* g_adjusted15;
static void* g_adjusted16;
static int g_clearCalls;
static int g_destroyCalls;
static void* g_collectorVtable[7];
static CollectorProbe g_collector;

static void AddEvent(Event event)
{
    g_events[g_eventCount++] = event;
}

static void MakeEmpty(ListNode& sentinel)
{
    sentinel.next = &sentinel;
    sentinel.previous = &sentinel;
    sentinel.component = 0;
}

static void MakeTwoNodeList(
    ListNode& sentinel,
    ListNode* nodes,
    void* first,
    void* second)
{
    sentinel.next = &nodes[0];
    sentinel.previous = &nodes[1];
    nodes[0].next = &nodes[1];
    nodes[0].previous = &sentinel;
    nodes[0].component = first;
    nodes[1].next = &sentinel;
    nodes[1].previous = &nodes[0];
    nodes[1].component = second;
}

extern "C" void __fastcall
FableFrontEndResetSlidersInitialiseList(
    void* list,
    void*,
    void*)
{
    AddEvent(kInitialise);
    g_list = (ListProbe*)list;
    MakeEmpty(g_sentinel15);
    g_list->head = &g_sentinel15;
    g_list->size = 0;
}

extern "C" void** __fastcall
FableFrontEndResetSlidersStackTop(void* stack, void*)
{
    AddEvent(kTop);
    if (stack == g_expectedStack)
        ++g_topCalls;
    return &g_topComponent;
}

static void __fastcall
CollectorCollect(
    void* object,
    void*,
    unsigned long key,
    void* list)
{
    if (object != &g_collector || list != g_list)
        return;
    g_collectedKeys[g_collectCalls++] = key;
    if (key == 0x0f)
    {
        AddEvent(kCollect15);
        MakeEmpty(g_sentinel15);
        if (g_populate)
            MakeTwoNodeList(
                g_sentinel15,
                g_nodes15,
                g_group15Component,
                0);
        g_list->head = &g_sentinel15;
        g_list->size = g_populate ? 2 : 0;
    }
    else
    {
        AddEvent(kCollect16);
        MakeEmpty(g_sentinel16);
        if (g_populate)
            MakeTwoNodeList(
                g_sentinel16,
                g_nodes16,
                0,
                g_group16Component);
        g_list->head = &g_sentinel16;
        g_list->size = g_populate ? 2 : 0;
    }
}

extern "C" void __fastcall
FableFrontEndResetSlidersAdjustGroup15(void* component, void*)
{
    AddEvent(kAdjust15);
    g_adjusted15 = component;
}

extern "C" void __fastcall
FableFrontEndResetSlidersClearList(void* list, void*)
{
    AddEvent(kClear);
    if (list == g_list)
    {
        ++g_clearCalls;
        MakeEmpty(g_sentinel15);
        g_list->head = &g_sentinel15;
        g_list->size = 0;
    }
}

extern "C" void __fastcall
FableFrontEndResetSlidersAdjustGroup16(void* component, void*)
{
    AddEvent(kAdjust16);
    g_adjusted16 = component;
}

extern "C" void __fastcall
FableFrontEndResetSlidersDestroyList(void* list, void*)
{
    AddEvent(kDestroy);
    if (list == g_list)
        ++g_destroyCalls;
}

static void Reset(unsigned char* manager, bool populate)
{
    std::memset(manager, 0, 0x40);
    std::memset(g_collectorVtable, 0, sizeof(g_collectorVtable));
    g_collectorVtable[0x18 / 4] = (void*)CollectorCollect;
    g_collector.vtable = g_collectorVtable;
    g_eventCount = 0;
    g_expectedStack = manager + 0x20;
    g_topComponent = &g_collector;
    g_topCalls = 0;
    g_list = 0;
    g_group15Component = (void*)0x11223344;
    g_group16Component = (void*)0x55667788;
    g_populate = populate;
    g_collectedKeys[0] = 0;
    g_collectedKeys[1] = 0;
    g_collectCalls = 0;
    g_adjusted15 = 0;
    g_adjusted16 = 0;
    g_clearCalls = 0;
    g_destroyCalls = 0;
}

static bool Run(bool populate)
{
    unsigned char manager[0x40];
    Reset(manager, populate);
    CFrontEndManager_ResetSlidersToProfileValues_00595356(manager, 0);

    static const Event emptyEvents[] =
    {
        kInitialise, kTop, kCollect15, kClear,
        kTop, kCollect16, kDestroy
    };
    static const Event populatedEvents[] =
    {
        kInitialise, kTop, kCollect15, kAdjust15, kClear,
        kTop, kCollect16, kAdjust16, kDestroy
    };
    const Event* expected = populate ? populatedEvents : emptyEvents;
    const unsigned long count = populate
        ? sizeof(populatedEvents) / sizeof(populatedEvents[0])
        : sizeof(emptyEvents) / sizeof(emptyEvents[0]);
    return g_eventCount == (int)count &&
        std::memcmp(g_events, expected, count * sizeof(Event)) == 0 &&
        g_topCalls == 2 &&
        g_collectCalls == 2 &&
        g_collectedKeys[0] == 0x0f &&
        g_collectedKeys[1] == 0x10 &&
        g_clearCalls == 1 &&
        g_destroyCalls == 1 &&
        (populate
            ? g_adjusted15 == g_group15Component &&
              g_adjusted16 == g_group16Component
            : g_adjusted15 == 0 && g_adjusted16 == 0);
}

int main()
{
    const bool empty = Run(false);
    const bool populated = Run(true);
    if (!empty || !populated)
    {
        std::printf(
            "FSE2_00595356_TEST FAIL empty=%d populated=%d\n",
            empty ? 1 : 0,
            populated ? 1 : 0);
        return 1;
    }
    std::puts("FSE2_00595356_TEST PASS");
    return 0;
}
