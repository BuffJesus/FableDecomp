#include <cstdio>

struct CMemoryAllocator;
typedef void (__stdcall *ClearFn)(void*);

int g_perfStatsObject = 0;
ClearFn g_clearFn0 = 0;
ClearFn g_clearFn1 = 0;

void __fastcall ClearPerformanceStats(CMemoryAllocator* self);

static void* g_arg0 = 0;
static void* g_arg1 = 0;
static int g_calls = 0;

static void __stdcall fake0(void* p) { g_arg0 = p; g_calls++; }
static void __stdcall fake1(void* p) { g_arg1 = p; g_calls++; }

int main()
{
    g_clearFn0 = fake0;
    g_clearFn1 = fake1;
    ClearPerformanceStats((CMemoryAllocator*)0);

    if (g_calls == 2 &&
        g_arg0 == (void*)&g_perfStatsObject &&
        g_arg1 == (void*)&g_perfStatsObject)
    {
        printf("009f26b0_TEST PASS\n");
    }
    else
    {
        printf("FAIL calls=%d\n", g_calls);
    }
    return 0;
}