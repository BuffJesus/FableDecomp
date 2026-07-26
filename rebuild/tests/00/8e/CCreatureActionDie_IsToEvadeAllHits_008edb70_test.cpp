#include <cstdio>
bool __fastcall CCreatureAction_Die_IsToEvadeAllHits()
{
    return true;
}
int main()
{
    if (CCreatureAction_Die_IsToEvadeAllHits() == true) { std::printf("AUTO_TINY_008edb70_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008edb70_TEST FAIL\n");
    return 1;
}