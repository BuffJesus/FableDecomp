#include <cstdio>
bool __fastcall CCreatureActionHeroJump_IsClearingMovementVector()
{
    return false;
}
int main()
{
    if (CCreatureActionHeroJump_IsClearingMovementVector() == false) { std::printf("AUTO_TINY_0085bbf0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0085bbf0_TEST FAIL\n");
    return 1;
}