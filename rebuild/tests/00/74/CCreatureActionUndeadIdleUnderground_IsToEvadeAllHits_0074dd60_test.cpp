#include <cstdio>
bool __fastcall CCreatureAction_UndeadIdleUnderground_IsToEvadeAllHits()
{
    return true;
}
int main()
{
    if (CCreatureAction_UndeadIdleUnderground_IsToEvadeAllHits() == true) { std::printf("AUTO_TINY_0074dd60_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0074dd60_TEST FAIL\n");
    return 1;
}