#include <cstdio>
int __fastcall CCombatSequence_FodderMainAttack_GetZone()
{
    return 3;
}
int main()
{
    if (CCombatSequence_FodderMainAttack_GetZone() == 3) { std::printf("AUTO_TINY_008b3ca0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b3ca0_TEST FAIL\n");
    return 1;
}