#include <cstdio>
bool __fastcall CCreatureAction_TeleportIn_IsToEvadeAllHits()
{
    return true;
}
int main()
{
    if (CCreatureAction_TeleportIn_IsToEvadeAllHits() == true) { std::printf("AUTO_TINY_008bdb10_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008bdb10_TEST FAIL\n");
    return 1;
}