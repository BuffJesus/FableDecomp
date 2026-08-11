#include <cstdio>

struct CEngineBillboardGenerator;

unsigned long __fastcall PeekSceneFilterFlags(CEngineBillboardGenerator* self);

int main()
{
    unsigned long r = PeekSceneFilterFlags((CEngineBillboardGenerator*)0);
    if (r == 1)
        std::printf("00b8c780_TEST PASS\n");
    else
        std::printf("FAIL %lu\n", r);
    return 0;
}