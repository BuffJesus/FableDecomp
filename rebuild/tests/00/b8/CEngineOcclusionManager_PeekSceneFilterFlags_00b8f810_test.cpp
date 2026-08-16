#include <cstdio>
int __fastcall CEngineOcclusionManager_PeekSceneFilterFlags()
{
    return 1;
}
int main()
{
    if (CEngineOcclusionManager_PeekSceneFilterFlags() == 1) { std::printf("AUTO_TINY_00b8f810_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00b8f810_TEST FAIL\n");
    return 1;
}