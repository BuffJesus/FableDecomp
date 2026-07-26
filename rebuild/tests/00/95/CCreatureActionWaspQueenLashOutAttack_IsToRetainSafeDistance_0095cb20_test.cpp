#include <cstdio>
bool __fastcall CCreatureAction_WaspQueenLashOutAttack_IsToRetainSafeDistance()
{
    return false;
}
int main()
{
    if (CCreatureAction_WaspQueenLashOutAttack_IsToRetainSafeDistance() == false) { std::printf("AUTO_TINY_0095cb20_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0095cb20_TEST FAIL\n");
    return 1;
}