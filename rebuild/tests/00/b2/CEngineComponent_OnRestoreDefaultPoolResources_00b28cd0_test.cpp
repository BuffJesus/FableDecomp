#include <cstdio>
bool __fastcall CEngineComponent_OnRestoreDefaultPoolResources()
{
    return true;
}
int main()
{
    if (CEngineComponent_OnRestoreDefaultPoolResources() == true) { std::printf("AUTO_TINY_00b28cd0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00b28cd0_TEST FAIL\n");
    return 1;
}