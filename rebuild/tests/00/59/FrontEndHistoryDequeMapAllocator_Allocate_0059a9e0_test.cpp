#include <cstdio>

class FrontEndHistoryDequeMapAllocator_0059a9e0
{
public:
    void** Allocate(unsigned long count, const void* hint);
};

static int g_AllocateCalls;
static unsigned long g_LastSize;
static void* g_ReturnAllocation;

extern "C" void* __cdecl
FrontEndHistoryDequeMapAllocate_0059a9e0(unsigned long size)
{
    ++g_AllocateCalls;
    g_LastSize = size;
    return g_ReturnAllocation;
}

static void ResetProbe(void* result)
{
    g_AllocateCalls = 0;
    g_LastSize = 0;
    g_ReturnAllocation = result;
}

int main()
{
    int failures = 0;
    FrontEndHistoryDequeMapAllocator_0059a9e0 allocator;
    void* slots[8];
    int hint_a = 1;
    int hint_b = 2;

    ResetProbe(slots);
    if (allocator.Allocate(0, &hint_a) != 0 ||
        g_AllocateCalls != 0 ||
        g_LastSize != 0)
    {
        std::printf("zero-count allocation failed\n");
        ++failures;
    }

    ResetProbe(slots);
    if (allocator.Allocate(1, &hint_a) != slots ||
        g_AllocateCalls != 1 ||
        g_LastSize != 4)
    {
        std::printf("single-slot allocation failed\n");
        ++failures;
    }

    ResetProbe(slots);
    if (allocator.Allocate(7, &hint_b) != slots ||
        g_AllocateCalls != 1 ||
        g_LastSize != 28)
    {
        std::printf("multi-slot allocation/hint handling failed\n");
        ++failures;
    }

    ResetProbe(0);
    if (allocator.Allocate(3, &hint_a) != 0 ||
        g_AllocateCalls != 1 ||
        g_LastSize != 12)
    {
        std::printf("allocator null-result forwarding failed\n");
        ++failures;
    }

    if (failures != 0)
    {
        std::printf(
            "FRONTEND_0059a9e0_TEST FAIL count=%d\n",
            failures);
        return 1;
    }

    std::printf("FRONTEND_0059a9e0_TEST PASS\n");
    return 0;
}
