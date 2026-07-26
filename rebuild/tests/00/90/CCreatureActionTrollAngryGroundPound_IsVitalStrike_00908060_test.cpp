#include <cstdio>
bool __fastcall CCreatureAction_TrollAngryGroundPound_IsVitalStrike()
{
    return true;
}
int main()
{
    if (CCreatureAction_TrollAngryGroundPound_IsVitalStrike() == true) { std::printf("AUTO_TINY_00908060_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00908060_TEST FAIL\n");
    return 1;
}