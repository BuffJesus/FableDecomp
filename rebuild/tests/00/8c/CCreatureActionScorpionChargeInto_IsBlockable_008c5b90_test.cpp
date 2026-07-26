#include <cstdio>
bool __fastcall CCreatureAction_ScorpionChargeInto_IsBlockable()
{
    return false;
}
int main()
{
    if (CCreatureAction_ScorpionChargeInto_IsBlockable() == false) { std::printf("AUTO_TINY_008c5b90_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008c5b90_TEST FAIL\n");
    return 1;
}