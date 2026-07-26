#include <cstdio>
bool __fastcall CCreatureAction_ReleaseTimeSpell_IsAbleToBeAttacked()
{
    return false;
}
int main()
{
    if (CCreatureAction_ReleaseTimeSpell_IsAbleToBeAttacked() == false) { std::printf("AUTO_TINY_008cebd0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008cebd0_TEST FAIL\n");
    return 1;
}