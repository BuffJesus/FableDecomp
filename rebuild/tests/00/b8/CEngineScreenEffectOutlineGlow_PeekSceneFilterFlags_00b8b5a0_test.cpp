#include <cstdio>
int __fastcall CEngineScreenEffectOutlineGlow_PeekSceneFilterFlags()
{
    return 1;
}
int main()
{
    if (CEngineScreenEffectOutlineGlow_PeekSceneFilterFlags() == 1) { std::printf("AUTO_TINY_00b8b5a0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00b8b5a0_TEST FAIL\n");
    return 1;
}