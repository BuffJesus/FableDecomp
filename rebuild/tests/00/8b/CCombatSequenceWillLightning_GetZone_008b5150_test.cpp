#include <cstdio>
int __fastcall CCombatSequence_WillLightning_GetZone()
{
    return 1;
}
int main()
{
    if (CCombatSequence_WillLightning_GetZone() == 1) { std::printf("AUTO_TINY_008b5150_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b5150_TEST FAIL\n");
    return 1;
}