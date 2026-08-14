#include <cstdio>

struct CEngineWeatherRenderer { int dummy; };

unsigned long __fastcall PeekSceneFilterFlags(CEngineWeatherRenderer* self)
{
    (void)self;
    return 1UL;
}

int main()
{
    CEngineWeatherRenderer r;
    r.dummy = 0x1234;
    unsigned long v = PeekSceneFilterFlags(&r);
    if (v == 1UL)
        printf("PARITY_OK\n");
    else
        printf("FAIL %lu\n", v);
    return 0;
}