#include <cstdio>
bool __fastcall CCombatSequence_WillLightning_IsInterruptable()
{
    return false;
}
int main()
{
    if (CCombatSequence_WillLightning_IsInterruptable() == false) { std::printf("AUTO_TINY_008b5170_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b5170_TEST FAIL\n");
    return 1;
}