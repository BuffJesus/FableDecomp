#include <cstdio>
bool __fastcall CCombatSequence_BanditKingAdvance_IsAllowedOnOuterRing()
{
    return false;
}
int main()
{
    if (CCombatSequence_BanditKingAdvance_IsAllowedOnOuterRing() == false) { std::printf("AUTO_TINY_008b3b20_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b3b20_TEST FAIL\n");
    return 1;
}