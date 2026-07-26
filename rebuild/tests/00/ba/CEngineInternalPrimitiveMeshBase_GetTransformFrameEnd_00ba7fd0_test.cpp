#include <cstdio>
bool __fastcall CEngineInternalPrimitiveMeshBase_GetTransformFrameEnd(void*, int, int)
{
    return false;
}
int main()
{
    int object = 0;
    if (!CEngineInternalPrimitiveMeshBase_GetTransformFrameEnd(&object, 1, 2)) { std::printf("AUTO_TINY_00ba7fd0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00ba7fd0_TEST FAIL\n");
    return 1;
}