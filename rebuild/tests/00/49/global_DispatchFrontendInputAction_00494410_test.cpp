#include <cstdio>

#include "../../../src/compiled/00/49/global_DispatchFrontendInputAction_00494410.cpp"

static CResolvedFrontendInputAction_00494410 g_Action;
static unsigned long g_ResolveCalls;
static unsigned long g_DispatchCalls;
static void* g_First;
static void* g_Second;
static CResolvedFrontendInputAction_00494410* g_DispatchThis;

CResolvedFrontendInputAction_00494410* __stdcall
ResolveFrontendInputAction_00494410(void* first, void* second)
{
    ++g_ResolveCalls;
    g_First = first;
    g_Second = second;
    return &g_Action;
}

void CResolvedFrontendInputAction_00494410::Dispatch()
{
    ++g_DispatchCalls;
    g_DispatchThis = this;
}

int main()
{
    void* first = reinterpret_cast<void*>(0x12345678);
    void* second = reinterpret_cast<void*>(0x76543210);

    DispatchFrontendInputAction_00494410(first, second);

    if (g_ResolveCalls != 1 || g_DispatchCalls != 1 ||
        g_First != first || g_Second != second ||
        g_DispatchThis != &g_Action)
    {
        std::printf("global_DispatchFrontendInputAction_00494410_test FAIL\n");
        return 1;
    }

    std::printf("global_DispatchFrontendInputAction_00494410_test PASS\n");
    return 0;
}
