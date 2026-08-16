#include <cstdio>
bool __fastcall CInputProcessCreditsUI_ProcessWhilePaused()
{
    return true;
}
int main()
{
    if (CInputProcessCreditsUI_ProcessWhilePaused() == true) { std::printf("AUTO_TINY_00486b70_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00486b70_TEST FAIL\n");
    return 1;
}