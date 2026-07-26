#include <cstdio>
int __fastcall CEnginePrimitiveOcclusionManager_PeekSceneFilterFlags()
{
    return 1;
}
int main()
{
    if (CEnginePrimitiveOcclusionManager_PeekSceneFilterFlags() == 1) { std::printf("AUTO_TINY_00b2fae0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00b2fae0_TEST FAIL\n");
    return 1;
}