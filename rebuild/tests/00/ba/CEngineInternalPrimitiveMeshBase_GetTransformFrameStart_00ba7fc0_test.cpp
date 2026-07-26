#include <cstdio>
bool __fastcall CEngineInternalPrimitiveMeshBase_GetTransformFrameStart(void*, int, int)
{
    return false;
}
int main()
{
    int object = 0;
    if (!CEngineInternalPrimitiveMeshBase_GetTransformFrameStart(&object, 1, 2)) { std::printf("AUTO_TINY_00ba7fc0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00ba7fc0_TEST FAIL\n");
    return 1;
}