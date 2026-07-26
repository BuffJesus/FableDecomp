#include <cstdio>
bool __fastcall CCreatureActionHero180RunningTurn_IsClearingMovementVector()
{
    return false;
}
int main()
{
    if (CCreatureActionHero180RunningTurn_IsClearingMovementVector() == false) { std::printf("AUTO_TINY_0085bbe0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0085bbe0_TEST FAIL\n");
    return 1;
}