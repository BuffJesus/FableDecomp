#include <cstdio>
bool __fastcall CCreatureAction_RiseUp_IsToEvadeAllHits()
{
    return true;
}
int main()
{
    if (CCreatureAction_RiseUp_IsToEvadeAllHits() == true) { std::printf("AUTO_TINY_0074db60_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0074db60_TEST FAIL\n");
    return 1;
}