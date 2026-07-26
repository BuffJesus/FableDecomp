#include <cstdio>
bool __fastcall CCreatureAction_ReleaseTimeSpell_IsToCauseRecoil()
{
    return true;
}
int main()
{
    if (CCreatureAction_ReleaseTimeSpell_IsToCauseRecoil() == true) { std::printf("AUTO_TINY_008cebc0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008cebc0_TEST FAIL\n");
    return 1;
}