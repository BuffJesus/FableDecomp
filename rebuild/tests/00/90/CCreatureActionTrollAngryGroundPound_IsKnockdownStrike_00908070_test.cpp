#include <cstdio>
bool __fastcall CCreatureAction_TrollAngryGroundPound_IsKnockdownStrike()
{
    return true;
}
int main()
{
    if (CCreatureAction_TrollAngryGroundPound_IsKnockdownStrike() == true) { std::printf("AUTO_TINY_00908070_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00908070_TEST FAIL\n");
    return 1;
}