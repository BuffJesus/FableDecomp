#include <cstdio>
bool __fastcall CCreatureAction_TrollAngryGroundPound_IsBlockable()
{
    return false;
}
int main()
{
    if (CCreatureAction_TrollAngryGroundPound_IsBlockable() == false) { std::printf("AUTO_TINY_00908080_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00908080_TEST FAIL\n");
    return 1;
}