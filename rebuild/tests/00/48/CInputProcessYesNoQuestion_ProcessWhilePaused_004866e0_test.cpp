#include <cstdio>
bool __fastcall CInputProcessYesNoQuestion_ProcessWhilePaused()
{
    return true;
}
int main()
{
    if (CInputProcessYesNoQuestion_ProcessWhilePaused() == true) { std::printf("AUTO_TINY_004866e0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004866e0_TEST FAIL\n");
    return 1;
}