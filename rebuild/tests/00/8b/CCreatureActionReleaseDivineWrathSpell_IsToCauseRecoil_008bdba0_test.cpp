#include <cstdio>
bool __fastcall CCreatureAction_ReleaseDivineWrathSpell_IsToCauseRecoil()
{
    return true;
}
int main()
{
    if (CCreatureAction_ReleaseDivineWrathSpell_IsToCauseRecoil() == true) { std::printf("AUTO_TINY_008bdba0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008bdba0_TEST FAIL\n");
    return 1;
}