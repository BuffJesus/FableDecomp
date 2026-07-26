#include <cstdio>
bool __fastcall CCreatureAction_SnapDragon_IsKnockdownStrike()
{
    return true;
}
int main()
{
    if (CCreatureAction_SnapDragon_IsKnockdownStrike() == true) { std::printf("AUTO_TINY_00953230_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00953230_TEST FAIL\n");
    return 1;
}