#include <cstdio>
#include <cstring>

#include "../../../src/compiled/00/49/CFrontendGameComponent_SetMode_00495fe0.cpp"

CFrontendModePointer_00495fe0 g_EmptyFrontendModePointer_00495fe0;

static unsigned long g_PrepareCalls;
static unsigned long g_RefreshCalls;
static unsigned long g_AssignCalls;
static void* g_PrepareThis;
static void* g_RefreshThis;
static void* g_AssignThis;
static const CFrontendModePointer_00495fe0* g_AssignValue;

void CFrontendGameComponent_00495fe0::PrepareModeTen()
{
    ++g_PrepareCalls;
    g_PrepareThis = this;
}

void CFrontendModeList_00495fe0::Refresh()
{
    ++g_RefreshCalls;
    g_RefreshThis = this;
}

void CFrontendModePointer_00495fe0::Assign(
    const CFrontendModePointer_00495fe0* value)
{
    ++g_AssignCalls;
    g_AssignThis = this;
    g_AssignValue = value;
}

static void ResetCalls()
{
    g_PrepareCalls = 0;
    g_RefreshCalls = 0;
    g_AssignCalls = 0;
    g_PrepareThis = 0;
    g_RefreshThis = 0;
    g_AssignThis = 0;
    g_AssignValue = 0;
}

static bool CheckMode(long mode)
{
    CFrontendGameComponent_00495fe0 component;
    unsigned char expected[sizeof(component)];
    unsigned char* bytes = reinterpret_cast<unsigned char*>(&component);
    std::memset(bytes, 0x5a, sizeof(component));
    std::memset(expected, 0x5a, sizeof(expected));

    *reinterpret_cast<long*>(expected + 0x1c) = mode;
    std::memset(expected + 0x168, 0, 32 * sizeof(long));
    *reinterpret_cast<long*>(expected + 0x1e8) = 0;
    *reinterpret_cast<long*>(expected + 0x1e44) = 1;
    if (mode == 11)
        *reinterpret_cast<long*>(expected + 0x1e28) = 0;
    if (mode == 12)
        *reinterpret_cast<long*>(expected + 0x1e18) = 0;

    ResetCalls();
    component.SetMode(mode);

    if (std::memcmp(bytes, expected, sizeof(component)) != 0)
        return false;

    if (mode == 10)
        return g_PrepareCalls == 1 && g_PrepareThis == &component &&
               g_RefreshCalls == 0 && g_AssignCalls == 0;

    if (mode == 11)
        return g_PrepareCalls == 0 && g_RefreshCalls == 1 &&
               g_RefreshThis == bytes + 0x1e1c && g_AssignCalls == 1 &&
               g_AssignThis == bytes + 0x1e40 &&
               g_AssignValue == &g_EmptyFrontendModePointer_00495fe0;

    if (mode == 12)
        return g_PrepareCalls == 0 && g_RefreshCalls == 1 &&
               g_RefreshThis == bytes + 0x1e0c && g_AssignCalls == 0;

    return g_PrepareCalls == 0 && g_RefreshCalls == 0 && g_AssignCalls == 0;
}

int main()
{
    if (!CheckMode(9) || !CheckMode(10) || !CheckMode(11) || !CheckMode(12) ||
        !CheckMode(13))
    {
        std::printf("CFrontendGameComponent_SetMode_00495fe0_test FAIL\n");
        return 1;
    }

    std::printf("CFrontendGameComponent_SetMode_00495fe0_test PASS\n");
    return 0;
}
