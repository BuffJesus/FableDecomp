#include <cstdio>
bool __fastcall CEngineInternalPrimitiveBase_GetBoundingSphereWorldSpace(void*, int, int)
{
    return false;
}
int main()
{
    int object = 0;
    if (!CEngineInternalPrimitiveBase_GetBoundingSphereWorldSpace(&object, 1, 2)) { std::printf("AUTO_TINY_00b3a9f0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00b3a9f0_TEST FAIL\n");
    return 1;
}