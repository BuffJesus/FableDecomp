#include <cstdio>
int __fastcall CEngineCamera_PeekSceneFilterFlags()
{
    return 1;
}
int main()
{
    if (CEngineCamera_PeekSceneFilterFlags() == 1) { std::printf("AUTO_TINY_00b31c00_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00b31c00_TEST FAIL\n");
    return 1;
}