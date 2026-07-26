#include <cstdio>
bool __fastcall CCreatureAction_SnapDragonOutOf_IsKnockdownStrike()
{
    return true;
}
int main()
{
    if (CCreatureAction_SnapDragonOutOf_IsKnockdownStrike() == true) { std::printf("AUTO_TINY_008e4a30_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008e4a30_TEST FAIL\n");
    return 1;
}