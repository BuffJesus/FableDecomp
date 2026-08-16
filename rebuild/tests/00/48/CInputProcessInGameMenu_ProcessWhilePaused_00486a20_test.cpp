#include <cstdio>
bool __fastcall CInputProcessInGameMenu_ProcessWhilePaused()
{
    return true;
}
int main()
{
    if (CInputProcessInGameMenu_ProcessWhilePaused() == true) { std::printf("AUTO_TINY_00486a20_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00486a20_TEST FAIL\n");
    return 1;
}