#include <cstdio>
bool __fastcall CCombatActionBase_IsToPerformActionOnStrike()
{
    return false;
}
int main()
{
    if (CCombatActionBase_IsToPerformActionOnStrike() == false) { std::printf("AUTO_TINY_0062eaa0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0062eaa0_TEST FAIL\n");
    return 1;
}