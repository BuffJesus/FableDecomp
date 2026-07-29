#include <cstdio>
#include <cstring>

#include "../../../src/compiled/00/52/CFrontEndManager_Initialize_0052d9e0.cpp"

extern "C" unsigned char FrontEndManagerVTable_0052d9e0[4] =
{
    0x11, 0x22, 0x33, 0x44
};

static unsigned long g_RequestedSize;
static int g_AllocateCalls;
static bool g_VtableReadyAtAllocation;
static bool g_SentinelNullAtAllocation;
static unsigned long g_SizeSeenAtAllocation;
static CFrontEndManager_0052d9e0* g_ManagerBeingInitialized;
static unsigned long g_NodeStorage[5];

extern "C" void* __cdecl
FrontEndManagerAllocate_0052d9e0(unsigned long size)
{
    ++g_AllocateCalls;
    g_RequestedSize = size;
    g_VtableReadyAtAllocation =
        g_ManagerBeingInitialized->vtable ==
        FrontEndManagerVTable_0052d9e0;
    g_SentinelNullAtAllocation =
        g_ManagerBeingInitialized->sentinel == 0;
    g_SizeSeenAtAllocation = g_ManagerBeingInitialized->size;
    return g_NodeStorage;
}

int main()
{
    int failures = 0;
    CFrontEndManager_0052d9e0 manager;

    std::memset(&manager, 0xa5, sizeof(manager));
    std::memset(g_NodeStorage, 0xa5, sizeof(g_NodeStorage));
    g_ManagerBeingInitialized = &manager;
    g_RequestedSize = 0;
    g_AllocateCalls = 0;
    g_VtableReadyAtAllocation = false;
    g_SentinelNullAtAllocation = false;
    g_SizeSeenAtAllocation = 0;

    CFrontEndManager_0052d9e0* result = manager.Initialize();
    FrontEndManagerNode_0052d9e0* node =
        (FrontEndManagerNode_0052d9e0*)g_NodeStorage;

    if (result != &manager ||
        manager.vtable != FrontEndManagerVTable_0052d9e0 ||
        manager.sentinel != node ||
        manager.size != 0 ||
        manager.state_flag != 0)
    {
        std::printf("manager fields/result failed\n");
        ++failures;
    }

    if (g_AllocateCalls != 1 ||
        g_RequestedSize != 0x14 ||
        !g_VtableReadyAtAllocation ||
        !g_SentinelNullAtAllocation ||
        g_SizeSeenAtAllocation != 0xa5a5a5a5)
    {
        std::printf("allocation size/order failed\n");
        ++failures;
    }

    if (node->marker != 0 ||
        node->payload != 0 ||
        node->previous != node ||
        node->next != node)
    {
        std::printf("sentinel initialization failed\n");
        ++failures;
    }

    unsigned char* bytes = (unsigned char*)node;
    if (bytes[1] != 0xa5 ||
        bytes[2] != 0xa5 ||
        bytes[3] != 0xa5 ||
        node->reserved_10 != 0xa5a5a5a5)
    {
        std::printf("sentinel untouched bytes failed\n");
        ++failures;
    }

    if (failures != 0)
    {
        std::printf(
            "FRONTEND_0052d9e0_TEST FAIL count=%d\n",
            failures);
        return 1;
    }

    std::printf("FRONTEND_0052d9e0_TEST PASS\n");
    return 0;
}
