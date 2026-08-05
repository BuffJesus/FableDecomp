// Real-object ABI/behavior fixture for
// NUISystem::CFrontEndManager::ChangeStateFirstTime @ 0x005952C3.

#include <cstdio>
#include <cstring>

extern "C" void __fastcall
CFrontEndManager_ChangeStateFirstTime_005952c3(void*, void*);

struct ComponentProbe
{
    void** vtable;
    long state;
    int stateCalls;
};

static void* g_expectedStack;
static void* g_topComponent;
static int g_topCalls;
static void* g_vtable[0xc4 / 4];

extern "C" void** __fastcall
FableFrontEndFirstStateStackTop(void* stack)
{
    if (stack == g_expectedStack)
        ++g_topCalls;
    return &g_topComponent;
}

static void __fastcall
ComponentSetState(void* object, void*, long state)
{
    ComponentProbe* component = (ComponentProbe*)object;
    component->state = state;
    ++component->stateCalls;
}

int main()
{
    unsigned char manager[0x40];
    ComponentProbe component;
    std::memset(manager, 0, sizeof(manager));
    std::memset(&component, 0, sizeof(component));
    std::memset(g_vtable, 0, sizeof(g_vtable));
    g_vtable[0xc0 / 4] = (void*)ComponentSetState;
    component.vtable = g_vtable;
    component.state = -1;
    g_expectedStack = manager + 0x20;
    g_topComponent = &component;
    g_topCalls = 0;

    CFrontEndManager_ChangeStateFirstTime_005952c3(manager, 0);

    if (g_topCalls != 1 ||
        component.state != 5 ||
        component.stateCalls != 1)
    {
        std::printf(
            "FSE2_005952c3_TEST FAIL top=%d state=%ld calls=%d\n",
            g_topCalls,
            component.state,
            component.stateCalls);
        return 1;
    }
    std::puts("FSE2_005952c3_TEST PASS");
    return 0;
}
