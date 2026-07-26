#include <cstdio>
bool __fastcall CCreatureAction_TrollProjectileAttack_IsInterruptableDueToZoneChange()
{
    return true;
}
int main()
{
    if (CCreatureAction_TrollProjectileAttack_IsInterruptableDueToZoneChange() == true) { std::printf("AUTO_TINY_00908130_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00908130_TEST FAIL\n");
    return 1;
}