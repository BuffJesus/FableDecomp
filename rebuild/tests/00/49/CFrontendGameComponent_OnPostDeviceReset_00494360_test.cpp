#include <cstdio>
bool __fastcall CFrontendGameComponent_OnPostDeviceReset()
{
    return true;
}
int main()
{
    if (CFrontendGameComponent_OnPostDeviceReset() == true) { std::printf("AUTO_TINY_00494360_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00494360_TEST FAIL\n");
    return 1;
}