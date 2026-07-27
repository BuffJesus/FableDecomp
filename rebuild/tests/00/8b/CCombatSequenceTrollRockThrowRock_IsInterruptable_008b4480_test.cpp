#include <cstdio>
bool __fastcall CCombatSequence_TrollRockThrowRock_IsInterruptable()
{
    return false;
}
int main()
{
    if (CCombatSequence_TrollRockThrowRock_IsInterruptable() == false) { std::printf("AUTO_TINY_008b4480_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b4480_TEST FAIL\n");
    return 1;
}