#include <cstdio>
bool __fastcall CCreatureAction_JackFloatUp_IsToEvadeAllHits()
{
    return true;
}
int main()
{
    if (CCreatureAction_JackFloatUp_IsToEvadeAllHits() == true) { std::printf("AUTO_TINY_009772f0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_009772f0_TEST FAIL\n");
    return 1;
}