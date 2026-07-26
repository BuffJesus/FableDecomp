#include <cstdio>
bool __fastcall CIEngine_Is2DSceneRenderingEnabled()
{
    return true;
}
int main()
{
    if (CIEngine_Is2DSceneRenderingEnabled() == true) { std::printf("AUTO_TINY_00b23710_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00b23710_TEST FAIL\n");
    return 1;
}