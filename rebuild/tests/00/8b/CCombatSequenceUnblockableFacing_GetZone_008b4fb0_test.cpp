#include <cstdio>
int __fastcall CCombatSequenceUnblockableFacing_GetZone()
{
    return 2;
}
int main()
{
    if (CCombatSequenceUnblockableFacing_GetZone() == 2) { std::printf("AUTO_TINY_008b4fb0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b4fb0_TEST FAIL\n");
    return 1;
}