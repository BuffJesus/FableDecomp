#include <cstdio>
bool __fastcall CCombatSequence_WillEnflame_IsInterruptable()
{
    return false;
}
int main()
{
    if (CCombatSequence_WillEnflame_IsInterruptable() == false) { std::printf("AUTO_TINY_008b5220_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b5220_TEST FAIL\n");
    return 1;
}