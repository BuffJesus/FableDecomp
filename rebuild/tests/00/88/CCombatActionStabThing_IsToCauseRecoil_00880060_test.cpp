#include <cstdio>
bool __fastcall CCombatAction_StabThing_IsToCauseRecoil()
{
    return true;
}
int main()
{
    if (CCombatAction_StabThing_IsToCauseRecoil() == true) { std::printf("AUTO_TINY_00880060_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00880060_TEST FAIL\n");
    return 1;
}