#include <cstdio>
int __fastcall CEngineResourceManager_PeekSceneFilterFlags()
{
    return 3;
}
int main()
{
    if (CEngineResourceManager_PeekSceneFilterFlags() == 3) { std::printf("AUTO_TINY_00b3ec80_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00b3ec80_TEST FAIL\n");
    return 1;
}