#include <cstdio>
bool __fastcall CCreatureAction_NymphTeleportLimboBase_IsToEvadeAllHits()
{
    return true;
}
int main()
{
    if (CCreatureAction_NymphTeleportLimboBase_IsToEvadeAllHits() == true) { std::printf("AUTO_TINY_008ca930_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008ca930_TEST FAIL\n");
    return 1;
}