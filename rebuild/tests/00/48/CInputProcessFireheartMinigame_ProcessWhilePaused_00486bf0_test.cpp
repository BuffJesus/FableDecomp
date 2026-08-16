#include <cstdio>
bool __fastcall CInputProcessFireheartMinigame_ProcessWhilePaused()
{
    return true;
}
int main()
{
    if (CInputProcessFireheartMinigame_ProcessWhilePaused() == true) { std::printf("AUTO_TINY_00486bf0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00486bf0_TEST FAIL\n");
    return 1;
}