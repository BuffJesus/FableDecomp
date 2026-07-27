#include <cstdio>
bool __fastcall CCreatureAction_ScorpionCharge_IsBlockable()
{
    return false;
}
int main()
{
    if (CCreatureAction_ScorpionCharge_IsBlockable() == false) { std::printf("AUTO_TINY_008c5bc0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008c5bc0_TEST FAIL\n");
    return 1;
}