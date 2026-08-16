#include <cstdio>
bool __fastcall CAInputProcess_ProcessWhilePaused()
{
    return false;
}
int main()
{
    if (CAInputProcess_ProcessWhilePaused() == false) { std::printf("AUTO_TINY_004863f0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004863f0_TEST FAIL\n");
    return 1;
}