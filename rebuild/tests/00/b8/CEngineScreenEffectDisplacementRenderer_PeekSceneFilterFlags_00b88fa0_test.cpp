#include <cstdio>
int __fastcall CEngineScreenEffectDisplacementRenderer_PeekSceneFilterFlags()
{
    return 3;
}
int main()
{
    if (CEngineScreenEffectDisplacementRenderer_PeekSceneFilterFlags() == 3) { std::printf("AUTO_TINY_00b88fa0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00b88fa0_TEST FAIL\n");
    return 1;
}