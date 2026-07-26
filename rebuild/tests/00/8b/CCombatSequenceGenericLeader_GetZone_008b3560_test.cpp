#include <cstdio>
int __fastcall CCombatSequence_GenericLeader_GetZone()
{
    return 128;
}
int main()
{
    if (CCombatSequence_GenericLeader_GetZone() == 128) { std::printf("AUTO_TINY_008b3560_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b3560_TEST FAIL\n");
    return 1;
}