#include <cstdio>
bool __fastcall CInputProcessConsole_ProcessWhilePaused()
{
    return true;
}
int main()
{
    if (CInputProcessConsole_ProcessWhilePaused() == true) { std::printf("AUTO_TINY_0048b3a0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0048b3a0_TEST FAIL\n");
    return 1;
}