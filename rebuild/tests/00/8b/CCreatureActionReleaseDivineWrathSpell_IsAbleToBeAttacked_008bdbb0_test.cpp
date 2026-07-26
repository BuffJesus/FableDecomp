#include <cstdio>
bool __fastcall CCreatureAction_ReleaseDivineWrathSpell_IsAbleToBeAttacked()
{
    return false;
}
int main()
{
    if (CCreatureAction_ReleaseDivineWrathSpell_IsAbleToBeAttacked() == false) { std::printf("AUTO_TINY_008bdbb0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008bdbb0_TEST FAIL\n");
    return 1;
}