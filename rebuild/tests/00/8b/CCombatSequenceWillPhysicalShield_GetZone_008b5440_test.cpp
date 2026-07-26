#include <cstdio>
int __fastcall CCombatSequence_WillPhysicalShield_GetZone()
{
    return 3;
}
int main()
{
    if (CCombatSequence_WillPhysicalShield_GetZone() == 3) { std::printf("AUTO_TINY_008b5440_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b5440_TEST FAIL\n");
    return 1;
}