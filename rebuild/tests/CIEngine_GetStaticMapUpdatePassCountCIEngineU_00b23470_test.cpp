// Standalone behaviour test for GetStaticMapUpdatePassCount @ 0x00b23470
#include <cstdio>

struct CIEngine
{
    void *vftable;
};

long __fastcall CIEngine_GetStaticMapUpdatePassCount(CIEngine *thisptr)
{
    (void)thisptr;
    return 0;
}

int main()
{
    CIEngine eng;
    eng.vftable = 0;
    long r = CIEngine_GetStaticMapUpdatePassCount(&eng);
    if (r == 0)
        std::printf("CIENGINE_00b23470_TEST PASS\n");
    else
        std::printf("CIENGINE_00b23470_TEST FAIL r=%ld\n", r);
    return 0;
}