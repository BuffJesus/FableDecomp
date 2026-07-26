#include <cstdio>
int __fastcall CCombatSequence_VillagerComboUnblockable_GetZone()
{
    return 2;
}
int main()
{
    if (CCombatSequence_VillagerComboUnblockable_GetZone() == 2) { std::printf("AUTO_TINY_008b4840_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b4840_TEST FAIL\n");
    return 1;
}