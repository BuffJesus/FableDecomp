#include <cstdio>
bool __fastcall CCreatureAction_GoUnderground_IsToEvadeAllHits()
{
    return true;
}
int main()
{
    if (CCreatureAction_GoUnderground_IsToEvadeAllHits() == true) { std::printf("AUTO_TINY_008b86a0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b86a0_TEST FAIL\n");
    return 1;
}