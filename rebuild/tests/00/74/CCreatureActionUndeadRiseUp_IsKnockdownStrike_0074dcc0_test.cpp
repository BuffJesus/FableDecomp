#include <cstdio>
bool __fastcall CCreatureAction_UndeadRiseUp_IsKnockdownStrike()
{
    return false;
}
int main()
{
    if (CCreatureAction_UndeadRiseUp_IsKnockdownStrike() == false) { std::printf("AUTO_TINY_0074dcc0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0074dcc0_TEST FAIL\n");
    return 1;
}