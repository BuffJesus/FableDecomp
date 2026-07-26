#include <cstdio>
bool __fastcall CCreatureAction_BlockCounter_IsKnockdownStrike()
{
    return true;
}
int main()
{
    if (CCreatureAction_BlockCounter_IsKnockdownStrike() == true) { std::printf("AUTO_TINY_00855d80_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00855d80_TEST FAIL\n");
    return 1;
}