#include <cstdio>
bool __fastcall CCombatAction_StrikeResponseScorpionKing_AllowsCombinationAnims()
{
    return true;
}
int main()
{
    if (CCombatAction_StrikeResponseScorpionKing_AllowsCombinationAnims() == true) { std::printf("AUTO_TINY_006f57b0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_006f57b0_TEST FAIL\n");
    return 1;
}