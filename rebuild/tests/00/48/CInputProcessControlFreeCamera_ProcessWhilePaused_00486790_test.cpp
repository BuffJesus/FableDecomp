#include <cstdio>
bool __fastcall CInputProcessControlFreeCamera_ProcessWhilePaused()
{
    return true;
}
int main()
{
    if (CInputProcessControlFreeCamera_ProcessWhilePaused() == true) { std::printf("AUTO_TINY_00486790_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00486790_TEST FAIL\n");
    return 1;
}