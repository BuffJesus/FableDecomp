#include <cstdio>
bool __fastcall CCreatureAction_ReleaseStrengthBerserk_IsToCauseRecoil()
{
    return true;
}
int main()
{
    if (CCreatureAction_ReleaseStrengthBerserk_IsToCauseRecoil() == true) { std::printf("AUTO_TINY_008e6e00_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008e6e00_TEST FAIL\n");
    return 1;
}