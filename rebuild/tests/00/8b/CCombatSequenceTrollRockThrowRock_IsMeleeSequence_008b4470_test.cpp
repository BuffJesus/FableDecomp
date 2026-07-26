#include <cstdio>
bool __fastcall CCombatSequence_TrollRockThrowRock_IsMeleeSequence()
{
    return true;
}
int main()
{
    if (CCombatSequence_TrollRockThrowRock_IsMeleeSequence() == true) { std::printf("AUTO_TINY_008b4470_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b4470_TEST FAIL\n");
    return 1;
}