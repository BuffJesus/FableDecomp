#include <cstdio>
int __fastcall CCombatSequence_TrollRockThrowRock_GetZone()
{
    return 1;
}
int main()
{
    if (CCombatSequence_TrollRockThrowRock_GetZone() == 1) { std::printf("AUTO_TINY_008b4460_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b4460_TEST FAIL\n");
    return 1;
}