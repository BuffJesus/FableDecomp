#include <cstdio>
int __fastcall CEngineScreenEffectColourFilter_PeekSceneFilterFlags()
{
    return 1;
}
int main()
{
    if (CEngineScreenEffectColourFilter_PeekSceneFilterFlags() == 1) { std::printf("AUTO_TINY_00b5baf0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00b5baf0_TEST FAIL\n");
    return 1;
}