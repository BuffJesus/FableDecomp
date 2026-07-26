#include <cstdio>
bool __fastcall CCombatSequence_WillPhysicalShieldNonMelee_IsToPreferSequenceWhenValid()
{
    return true;
}
int main()
{
    if (CCombatSequence_WillPhysicalShieldNonMelee_IsToPreferSequenceWhenValid() == true) { std::printf("AUTO_TINY_008b54e0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b54e0_TEST FAIL\n");
    return 1;
}