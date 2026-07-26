#include <cstdio>
bool __fastcall CEngineInternalPrimitiveBase_GetBoundingSphereRadiusWorldSpace(void*, int, int)
{
    return false;
}
int main()
{
    int object = 0;
    if (!CEngineInternalPrimitiveBase_GetBoundingSphereRadiusWorldSpace(&object, 1, 2)) { std::printf("AUTO_TINY_00b3aa00_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00b3aa00_TEST FAIL\n");
    return 1;
}