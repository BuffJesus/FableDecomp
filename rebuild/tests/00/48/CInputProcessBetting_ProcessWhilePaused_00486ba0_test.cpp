#include <cstdio>
bool __fastcall CInputProcessBetting_ProcessWhilePaused()
{
    return true;
}
int main()
{
    if (CInputProcessBetting_ProcessWhilePaused() == true) { std::printf("AUTO_TINY_00486ba0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00486ba0_TEST FAIL\n");
    return 1;
}