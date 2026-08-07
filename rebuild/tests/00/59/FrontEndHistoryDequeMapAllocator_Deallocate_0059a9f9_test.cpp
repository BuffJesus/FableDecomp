#include <cstdio>

class FrontEndHistoryDequeMapAllocator_0059a9f9
{
public:
    void Deallocate(void** allocation, unsigned long count);
};

static int g_DeleteCalls;
static void* g_DeletedAllocation;

extern "C" void __cdecl
FrontEndHistoryDequeMapDelete_0059a9f9(void* allocation)
{
    ++g_DeleteCalls;
    g_DeletedAllocation = allocation;
}

static void ResetProbe()
{
    g_DeleteCalls = 0;
    g_DeletedAllocation = 0;
}

int main()
{
    int failures = 0;
    FrontEndHistoryDequeMapAllocator_0059a9f9 allocator;
    void* slots[8];

    ResetProbe();
    allocator.Deallocate(0, 8);
    if (g_DeleteCalls != 0 || g_DeletedAllocation != 0)
    {
        std::printf("null map deallocation failed\n");
        ++failures;
    }

    ResetProbe();
    allocator.Deallocate(slots, 8);
    if (g_DeleteCalls != 1 || g_DeletedAllocation != slots)
    {
        std::printf("non-null map ownership release failed\n");
        ++failures;
    }

    // Retail forwards a non-null allocation regardless of the supplied size.
    ResetProbe();
    allocator.Deallocate(slots, 0);
    if (g_DeleteCalls != 1 || g_DeletedAllocation != slots)
    {
        std::printf("ignored slot-count behavior failed\n");
        ++failures;
    }

    if (failures != 0)
    {
        std::printf(
            "FRONTEND_0059a9f9_TEST FAIL count=%d\n",
            failures);
        return 1;
    }

    std::printf("FRONTEND_0059a9f9_TEST PASS\n");
    return 0;
}
