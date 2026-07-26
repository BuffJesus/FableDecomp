#include <cstdio>
int __fastcall CEngineScreenEffectGlowRenderer_PeekSceneFilterFlags()
{
    return 1;
}
int main()
{
    if (CEngineScreenEffectGlowRenderer_PeekSceneFilterFlags() == 1) { std::printf("AUTO_TINY_00b61a10_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00b61a10_TEST FAIL\n");
    return 1;
}