#include <cstdio>
bool __fastcall CCombatActionBase_IsVitalStrike()
{
    return false;
}
int main()
{
    if (CCombatActionBase_IsVitalStrike() == false) { std::printf("AUTO_TINY_008562e0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008562e0_TEST FAIL\n");
    return 1;
}