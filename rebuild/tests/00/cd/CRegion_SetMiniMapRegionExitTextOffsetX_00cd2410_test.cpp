#include <cstdio>

struct CRegion {
    void* f0;
};

extern "C" void* __cdecl helper_4292c0(void* arg);

CRegion* __fastcall SetMiniMapRegionExitTextOffsetX(CRegion* self);

static void* g_seen = 0;
extern "C" void* __cdecl helper_4292c0(void* arg)
{
    g_seen = arg;
    return (void*)((char*)arg + 0x10);
}

int main()
{
    CRegion r;
    r.f0 = (void*)0x1000;
    CRegion* ret = SetMiniMapRegionExitTextOffsetX(&r);
    if (ret == &r && g_seen == (void*)0x1000 && r.f0 == (void*)0x1010)
        printf("00cd2410_TEST PASS\n");
    else
        printf("FAIL ret=%p seen=%p f0=%p\n", (void*)ret, g_seen, r.f0);
    return 0;
}