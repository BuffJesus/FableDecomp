#include <cstdio>
bool __fastcall CCreatureAction_UndeadReturnToEarth_IsToEvadeAllHits()
{
    return true;
}
int main()
{
    if (CCreatureAction_UndeadReturnToEarth_IsToEvadeAllHits() == true) { std::printf("AUTO_TINY_008ec770_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008ec770_TEST FAIL\n");
    return 1;
}