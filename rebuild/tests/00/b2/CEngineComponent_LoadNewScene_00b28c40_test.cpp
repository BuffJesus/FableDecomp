#include <cstdio>
bool __fastcall CEngineComponent_LoadNewScene(void*, int, int)
{
    return false;
}
int main()
{
    int object = 0;
    if (!CEngineComponent_LoadNewScene(&object, 1, 2)) { std::printf("AUTO_TINY_00b28c40_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00b28c40_TEST FAIL\n");
    return 1;
}