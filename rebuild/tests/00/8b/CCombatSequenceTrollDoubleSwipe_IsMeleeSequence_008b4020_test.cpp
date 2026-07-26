#include <cstdio>
bool __fastcall CCombatSequence_TrollDoubleSwipe_IsMeleeSequence()
{
    return true;
}
int main()
{
    if (CCombatSequence_TrollDoubleSwipe_IsMeleeSequence() == true) { std::printf("AUTO_TINY_008b4020_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b4020_TEST FAIL\n");
    return 1;
}