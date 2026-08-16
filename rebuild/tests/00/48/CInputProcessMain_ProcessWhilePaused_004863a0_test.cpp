#include <cstdio>
bool __fastcall CInputProcessMain_ProcessWhilePaused()
{
    return true;
}
int main()
{
    if (CInputProcessMain_ProcessWhilePaused() == true) { std::printf("AUTO_TINY_004863a0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004863a0_TEST FAIL\n");
    return 1;
}