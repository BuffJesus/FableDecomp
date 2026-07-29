// Real-object ABI/behavior fixture for
// NUISystem::CFrontEndManager::Draw @ 0x00595222.

#include <cstdio>
#include <cstring>

extern "C" void __fastcall
CFrontEndManager_Draw_00595222(
    void* manager,
    void*,
    void* engine,
    void* primitiveHandle);

struct ComponentProbe
{
    void** vtable;
};

enum Event
{
    kDraw = 1,
    kNext
};

static Event g_events[8];
static int g_eventCount;
static unsigned char g_sentinel[0x20];
static unsigned char g_firstNode[0x20];
static unsigned char g_secondNode[0x20];
static void* g_componentVtable[3];
static ComponentProbe g_component;
static void* g_expectedEngine;
static void* g_expectedHandle;
static int g_drawCalls;
static void* g_drawObject;
static long g_drawFlags[3];
static int g_nextCalls;

static void AddEvent(Event event)
{
    g_events[g_eventCount++] = event;
}

static void* NodeComponent(unsigned char* node)
{
    return *(void**)(node + 0x14);
}

static void SetNodeComponent(unsigned char* node, void* component)
{
    *(void**)(node + 0x14) = component;
}

extern "C" void* __cdecl
FableFrontEndDrawNextNode(void* node)
{
    AddEvent(kNext);
    ++g_nextCalls;
    if (node == g_firstNode)
        return g_secondNode;
    if (node == g_secondNode)
        return g_sentinel;
    return g_sentinel;
}

static void __fastcall
ComponentDraw(
    void* object,
    void*,
    void* engine,
    void* primitiveHandle,
    long flag0,
    long flag1,
    long flag2)
{
    AddEvent(kDraw);
    ++g_drawCalls;
    g_drawObject = object;
    if (engine == g_expectedEngine &&
        primitiveHandle == g_expectedHandle)
    {
        g_drawFlags[0] = flag0;
        g_drawFlags[1] = flag1;
        g_drawFlags[2] = flag2;
    }
}

static void Reset(unsigned char* manager)
{
    std::memset(manager, 0, 0x80);
    std::memset(g_sentinel, 0, sizeof(g_sentinel));
    std::memset(g_firstNode, 0, sizeof(g_firstNode));
    std::memset(g_secondNode, 0, sizeof(g_secondNode));
    std::memset(g_componentVtable, 0, sizeof(g_componentVtable));
    g_componentVtable[2] = (void*)ComponentDraw;
    g_component.vtable = g_componentVtable;
    *(void**)(manager + 0x54) = g_sentinel;
    g_eventCount = 0;
    g_expectedEngine = (void*)0x11223344;
    g_expectedHandle = (void*)0x55667788;
    g_drawCalls = 0;
    g_drawObject = 0;
    g_drawFlags[0] = -1;
    g_drawFlags[1] = -1;
    g_drawFlags[2] = -1;
    g_nextCalls = 0;
}

static bool RunEmpty()
{
    unsigned char manager[0x80];
    Reset(manager);
    *(void**)(g_sentinel + 8) = g_sentinel;

    CFrontEndManager_Draw_00595222(
        manager, 0, g_expectedEngine, g_expectedHandle);

    return g_eventCount == 0 &&
        g_drawCalls == 0 &&
        g_nextCalls == 0;
}

static bool RunList()
{
    unsigned char manager[0x80];
    Reset(manager);
    *(void**)(g_sentinel + 8) = g_firstNode;
    SetNodeComponent(g_firstNode, 0);
    SetNodeComponent(g_secondNode, &g_component);

    CFrontEndManager_Draw_00595222(
        manager, 0, g_expectedEngine, g_expectedHandle);

    static const Event expected[] = { kNext, kDraw, kNext };
    return g_eventCount == 3 &&
        std::memcmp(g_events, expected, sizeof(expected)) == 0 &&
        NodeComponent(g_firstNode) == 0 &&
        g_nextCalls == 2 &&
        g_drawCalls == 1 &&
        g_drawObject == &g_component &&
        g_drawFlags[0] == 0 &&
        g_drawFlags[1] == 0 &&
        g_drawFlags[2] == 0;
}

int main()
{
    const bool empty = RunEmpty();
    const bool list = RunList();
    if (!empty || !list)
    {
        std::printf(
            "FSE2_00595222_TEST FAIL empty=%d list=%d\n",
            empty ? 1 : 0,
            list ? 1 : 0);
        return 1;
    }
    std::puts("FSE2_00595222_TEST PASS");
    return 0;
}
