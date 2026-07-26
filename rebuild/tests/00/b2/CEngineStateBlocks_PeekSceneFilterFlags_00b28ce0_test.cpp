#include <cstdio>
int __fastcall CEngineStateBlocks_PeekSceneFilterFlags()
{
    return 3;
}
int main()
{
    if (CEngineStateBlocks_PeekSceneFilterFlags() == 3) { std::printf("AUTO_TINY_00b28ce0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00b28ce0_TEST FAIL\n");
    return 1;
}