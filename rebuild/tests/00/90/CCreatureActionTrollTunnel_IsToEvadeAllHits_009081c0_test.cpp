#include <cstdio>
bool __fastcall CCreatureAction_TrollTunnel_IsToEvadeAllHits()
{
    return true;
}
int main()
{
    if (CCreatureAction_TrollTunnel_IsToEvadeAllHits() == true) { std::printf("AUTO_TINY_009081c0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_009081c0_TEST FAIL\n");
    return 1;
}