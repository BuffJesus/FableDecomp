#include <cstdio>
bool __fastcall CCombatSequence_WillPhysicalShield_IsMeleeSequence()
{
    return true;
}
int main()
{
    if (CCombatSequence_WillPhysicalShield_IsMeleeSequence() == true) { std::printf("AUTO_TINY_008b5470_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b5470_TEST FAIL\n");
    return 1;
}