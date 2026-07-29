// Real-object ABI/behavior fixture for
// NUISystem::CFrontEndManager::CreateComponent @ 0x00594F94.

#include <cstdio>

extern "C" void* __fastcall
CFrontEndManager_CreateComponent_00594f94(
    void* manager,
    void*,
    const void* definition);

static unsigned char g_manager;
static unsigned char g_component;
static int g_getCalls;
static int g_createCalls;
static const void* g_definition;
static long g_frontEnd;

extern "C" void* __cdecl
FableFrontEndCreateComponentGetManager()
{
    ++g_getCalls;
    return &g_manager;
}

extern "C" void* __fastcall
FableFrontEndCreateComponentFromDefinition(
    void* manager,
    void*,
    const void* definition,
    long frontEnd)
{
    if (manager == &g_manager)
    {
        ++g_createCalls;
        g_definition = definition;
        g_frontEnd = frontEnd;
    }
    return &g_component;
}

int main()
{
    unsigned long definition[2] = { 0x12345678, 0x87654321 };
    g_getCalls = 0;
    g_createCalls = 0;
    g_definition = 0;
    g_frontEnd = 0;

    void* result = CFrontEndManager_CreateComponent_00594f94(
        (void*)0x11111111,
        0,
        definition);

    if (result != &g_component ||
        g_getCalls != 1 ||
        g_createCalls != 1 ||
        g_definition != definition ||
        g_frontEnd != 1)
    {
        std::printf(
            "FSE2_00594f94_TEST FAIL result=%d get=%d create=%d "
            "definition=%d frontEnd=%ld\n",
            result == &g_component ? 1 : 0,
            g_getCalls,
            g_createCalls,
            g_definition == definition ? 1 : 0,
            g_frontEnd);
        return 1;
    }
    std::puts("FSE2_00594f94_TEST PASS");
    return 0;
}
