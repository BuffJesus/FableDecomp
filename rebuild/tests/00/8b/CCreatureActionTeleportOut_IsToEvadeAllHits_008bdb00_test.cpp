#include <cstdio>
bool __fastcall CCreatureAction_TeleportOut_IsToEvadeAllHits()
{
    return true;
}
int main()
{
    if (CCreatureAction_TeleportOut_IsToEvadeAllHits() == true) { std::printf("AUTO_TINY_008bdb00_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008bdb00_TEST FAIL\n");
    return 1;
}