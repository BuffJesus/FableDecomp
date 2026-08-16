#include <cstdio>
bool __fastcall CInputProcessTavernGame_ProcessWhilePaused()
{
    return true;
}
int main()
{
    if (CInputProcessTavernGame_ProcessWhilePaused() == true) { std::printf("AUTO_TINY_00486a50_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00486a50_TEST FAIL\n");
    return 1;
}