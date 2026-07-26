#include <cstdio>
bool __fastcall CCreatureAction_BanditKingGetHit_IsToEvadeAllHits()
{
    return true;
}
int main()
{
    if (CCreatureAction_BanditKingGetHit_IsToEvadeAllHits() == true) { std::printf("AUTO_TINY_006f5a60_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_006f5a60_TEST FAIL\n");
    return 1;
}