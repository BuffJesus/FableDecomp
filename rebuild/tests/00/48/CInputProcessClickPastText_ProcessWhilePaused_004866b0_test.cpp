#include <cstdio>
bool __fastcall CInputProcessClickPastText_ProcessWhilePaused()
{
    return true;
}
int main()
{
    if (CInputProcessClickPastText_ProcessWhilePaused() == true) { std::printf("AUTO_TINY_004866b0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004866b0_TEST FAIL\n");
    return 1;
}