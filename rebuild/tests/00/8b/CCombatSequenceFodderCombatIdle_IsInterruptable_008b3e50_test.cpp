#include <cstdio>
bool __fastcall CCombatSequence_FodderCombatIdle_IsInterruptable()
{
    return true;
}
int main()
{
    if (CCombatSequence_FodderCombatIdle_IsInterruptable() == true) { std::printf("AUTO_TINY_008b3e50_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b3e50_TEST FAIL\n");
    return 1;
}