// Real-object ABI/behavior fixture for
// NUISystem::CFrontEndManager::GetInstance @ 0x00595582.

#include <cstdio>

extern "C" void* __fastcall
CFrontEndManager_GetInstance_00595582();

extern "C" void* FableFrontEndManagerSingleton = 0;

static unsigned char g_storage[0xe0];
static unsigned char g_constructed;
static void* g_allocationResult;
static void* g_constructorResult;
static unsigned long g_allocatedBytes;
static int g_allocateCalls;
static void* g_constructorStorage;
static int g_constructorCalls;

extern "C" void* __cdecl
FableFrontEndManagerAllocate(unsigned long bytes)
{
    ++g_allocateCalls;
    g_allocatedBytes = bytes;
    return g_allocationResult;
}

extern "C" void* __fastcall
FableFrontEndManagerConstructInstance(void* storage)
{
    ++g_constructorCalls;
    g_constructorStorage = storage;
    return g_constructorResult;
}

static void Reset()
{
    FableFrontEndManagerSingleton = 0;
    g_allocationResult = 0;
    g_constructorResult = 0;
    g_allocatedBytes = 0;
    g_allocateCalls = 0;
    g_constructorStorage = 0;
    g_constructorCalls = 0;
}

static bool RunExisting()
{
    Reset();
    FableFrontEndManagerSingleton = &g_constructed;
    void* result = CFrontEndManager_GetInstance_00595582();
    return result == &g_constructed &&
        FableFrontEndManagerSingleton == &g_constructed &&
        g_allocateCalls == 0 &&
        g_constructorCalls == 0;
}

static bool RunAllocationFailure()
{
    Reset();
    g_allocationResult = 0;
    void* result = CFrontEndManager_GetInstance_00595582();
    return result == 0 &&
        FableFrontEndManagerSingleton == 0 &&
        g_allocateCalls == 1 &&
        g_allocatedBytes == 0xe0 &&
        g_constructorCalls == 0;
}

static bool RunConstruction()
{
    Reset();
    g_allocationResult = g_storage;
    g_constructorResult = &g_constructed;
    void* result = CFrontEndManager_GetInstance_00595582();
    return result == &g_constructed &&
        FableFrontEndManagerSingleton == &g_constructed &&
        g_allocateCalls == 1 &&
        g_allocatedBytes == 0xe0 &&
        g_constructorCalls == 1 &&
        g_constructorStorage == g_storage;
}

int main()
{
    const bool existing = RunExisting();
    const bool failed = RunAllocationFailure();
    const bool constructed = RunConstruction();
    if (!existing || !failed || !constructed)
    {
        std::printf(
            "FSE2_00595582_TEST FAIL existing=%d failed=%d "
            "constructed=%d\n",
            existing ? 1 : 0,
            failed ? 1 : 0,
            constructed ? 1 : 0);
        return 1;
    }
    std::puts("FSE2_00595582_TEST PASS");
    return 0;
}
