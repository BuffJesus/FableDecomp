#include <cstdio>
int __fastcall CCombatSequence_NonInterruptableBoxStrafe_GetZone()
{
    return 2;
}
int main()
{
    if (CCombatSequence_NonInterruptableBoxStrafe_GetZone() == 2) { std::printf("AUTO_TINY_008b3650_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b3650_TEST FAIL\n");
    return 1;
}