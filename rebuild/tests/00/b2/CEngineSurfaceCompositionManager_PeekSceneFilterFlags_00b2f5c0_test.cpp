#include <stdio.h>

struct CEngineSurfaceCompositionManager {
    unsigned long __fastcall PeekSceneFilterFlags();
};

unsigned long __fastcall CEngineSurfaceCompositionManager::PeekSceneFilterFlags()
{
    return 3;
}

int main()
{
    CEngineSurfaceCompositionManager obj;
    unsigned long r = obj.PeekSceneFilterFlags();
    if (r == 3)
        printf("00b2f5c0_TEST PASS\n");
    else
        printf("FAIL %lu\n", r);
    return 0;
}