#include <cstdio>
int __fastcall CEngineShadowRenderer_PeekSceneFilterFlags()
{
    return 1;
}
int main()
{
    if (CEngineShadowRenderer_PeekSceneFilterFlags() == 1) { std::printf("AUTO_TINY_00b50b70_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00b50b70_TEST FAIL\n");
    return 1;
}