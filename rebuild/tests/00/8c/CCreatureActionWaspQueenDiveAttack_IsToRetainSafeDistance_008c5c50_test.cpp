#include <cstdio>
bool __fastcall CCreatureAction_WaspQueenDiveAttack_IsToRetainSafeDistance()
{
    return false;
}
int main()
{
    if (CCreatureAction_WaspQueenDiveAttack_IsToRetainSafeDistance() == false) { std::printf("AUTO_TINY_008c5c50_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008c5c50_TEST FAIL\n");
    return 1;
}