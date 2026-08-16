#include <cstdio>
int __fastcall CEngineLightingManager_PeekSceneFilterFlags()
{
    return 3;
}
int main()
{
    if (CEngineLightingManager_PeekSceneFilterFlags() == 3) { std::printf("AUTO_TINY_00b4a470_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00b4a470_TEST FAIL\n");
    return 1;
}