#include <cstdio>
bool __fastcall CCombatSequence_WillPhysicalShieldNonMelee_IsMeleeSequence()
{
    return false;
}
int main()
{
    if (CCombatSequence_WillPhysicalShieldNonMelee_IsMeleeSequence() == false) { std::printf("AUTO_TINY_008b5520_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b5520_TEST FAIL\n");
    return 1;
}