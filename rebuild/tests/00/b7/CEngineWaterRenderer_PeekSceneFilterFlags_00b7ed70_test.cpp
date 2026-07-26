#include <cstdio>
int __fastcall CEngineWaterRenderer_PeekSceneFilterFlags()
{
    return 1;
}
int main()
{
    if (CEngineWaterRenderer_PeekSceneFilterFlags() == 1) { std::printf("AUTO_TINY_00b7ed70_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00b7ed70_TEST FAIL\n");
    return 1;
}