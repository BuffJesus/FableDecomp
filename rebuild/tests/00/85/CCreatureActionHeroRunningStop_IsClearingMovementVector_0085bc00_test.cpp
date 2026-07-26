#include <cstdio>
bool __fastcall CCreatureActionHeroRunningStop_IsClearingMovementVector()
{
    return false;
}
int main()
{
    if (CCreatureActionHeroRunningStop_IsClearingMovementVector() == false) { std::printf("AUTO_TINY_0085bc00_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0085bc00_TEST FAIL\n");
    return 1;
}