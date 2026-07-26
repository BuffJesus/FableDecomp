#include <cstdio>
bool __fastcall CCombatSequence_VillagerComboUnblockable_IsMeleeSequence()
{
    return true;
}
int main()
{
    if (CCombatSequence_VillagerComboUnblockable_IsMeleeSequence() == true) { std::printf("AUTO_TINY_008b4850_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b4850_TEST FAIL\n");
    return 1;
}