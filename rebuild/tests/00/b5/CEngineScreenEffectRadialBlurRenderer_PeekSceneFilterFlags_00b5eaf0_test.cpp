#include <cstdio>
int __fastcall CEngineScreenEffectRadialBlurRenderer_PeekSceneFilterFlags()
{
    return 3;
}
int main()
{
    if (CEngineScreenEffectRadialBlurRenderer_PeekSceneFilterFlags() == 3) { std::printf("AUTO_TINY_00b5eaf0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00b5eaf0_TEST FAIL\n");
    return 1;
}