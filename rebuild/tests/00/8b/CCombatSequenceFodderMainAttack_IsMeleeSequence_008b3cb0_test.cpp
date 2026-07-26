#include <cstdio>
bool __fastcall CCombatSequence_FodderMainAttack_IsMeleeSequence()
{
    return true;
}
int main()
{
    if (CCombatSequence_FodderMainAttack_IsMeleeSequence() == true) { std::printf("AUTO_TINY_008b3cb0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b3cb0_TEST FAIL\n");
    return 1;
}