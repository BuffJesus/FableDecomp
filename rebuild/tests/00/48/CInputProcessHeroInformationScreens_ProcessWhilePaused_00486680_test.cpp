#include <cstdio>
bool __fastcall CInputProcessHeroInformationScreens_ProcessWhilePaused()
{
    return true;
}
int main()
{
    if (CInputProcessHeroInformationScreens_ProcessWhilePaused() == true) { std::printf("AUTO_TINY_00486680_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00486680_TEST FAIL\n");
    return 1;
}