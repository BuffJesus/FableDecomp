#include <cstdio>
bool __fastcall CCreatureAction_ReleaseStrengthBerserk_IsAbleToBeAttacked()
{
    return false;
}
int main()
{
    if (CCreatureAction_ReleaseStrengthBerserk_IsAbleToBeAttacked() == false) { std::printf("AUTO_TINY_008e6e10_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008e6e10_TEST FAIL\n");
    return 1;
}