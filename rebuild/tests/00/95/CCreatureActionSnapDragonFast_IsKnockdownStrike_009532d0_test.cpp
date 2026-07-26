#include <cstdio>
bool __fastcall CCreatureAction_SnapDragonFast_IsKnockdownStrike()
{
    return true;
}
int main()
{
    if (CCreatureAction_SnapDragonFast_IsKnockdownStrike() == true) { std::printf("AUTO_TINY_009532d0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_009532d0_TEST FAIL\n");
    return 1;
}