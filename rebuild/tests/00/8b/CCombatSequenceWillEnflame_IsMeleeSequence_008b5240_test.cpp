#include <cstdio>
bool __fastcall CCombatSequence_WillEnflame_IsMeleeSequence()
{
    return true;
}
int main()
{
    if (CCombatSequence_WillEnflame_IsMeleeSequence() == true) { std::printf("AUTO_TINY_008b5240_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b5240_TEST FAIL\n");
    return 1;
}