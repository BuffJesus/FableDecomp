#include <cstdio>
bool __fastcall CAIStateGroup_MeleeCombatNearbyBase_IsTimingCritical()
{
    return true;
}
int main()
{
    if (CAIStateGroup_MeleeCombatNearbyBase_IsTimingCritical() == true) { std::printf("AUTO_TINY_0093d170_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0093d170_TEST FAIL\n");
    return 1;
}