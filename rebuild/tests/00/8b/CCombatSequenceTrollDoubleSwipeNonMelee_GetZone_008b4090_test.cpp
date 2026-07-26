#include <cstdio>
int __fastcall CCombatSequence_TrollDoubleSwipeNonMelee_GetZone()
{
    return 4;
}
int main()
{
    if (CCombatSequence_TrollDoubleSwipeNonMelee_GetZone() == 4) { std::printf("AUTO_TINY_008b4090_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b4090_TEST FAIL\n");
    return 1;
}