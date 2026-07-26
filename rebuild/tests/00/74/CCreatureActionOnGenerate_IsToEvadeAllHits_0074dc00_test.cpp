#include <cstdio>
bool __fastcall CCreatureAction_OnGenerate_IsToEvadeAllHits()
{
    return true;
}
int main()
{
    if (CCreatureAction_OnGenerate_IsToEvadeAllHits() == true) { std::printf("AUTO_TINY_0074dc00_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0074dc00_TEST FAIL\n");
    return 1;
}