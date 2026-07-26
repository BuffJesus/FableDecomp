#include <cstdio>
bool __fastcall CCombatSequence_VillagerNearAttack_IsMeleeSequence()
{
    return true;
}
int main()
{
    if (CCombatSequence_VillagerNearAttack_IsMeleeSequence() == true) { std::printf("AUTO_TINY_008b48d0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b48d0_TEST FAIL\n");
    return 1;
}