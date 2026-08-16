#include <cstdio>
bool __fastcall CInputProcessInventoryBase_ProcessWhilePaused()
{
    return true;
}
int main()
{
    if (CInputProcessInventoryBase_ProcessWhilePaused() == true) { std::printf("AUTO_TINY_00486500_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00486500_TEST FAIL\n");
    return 1;
}