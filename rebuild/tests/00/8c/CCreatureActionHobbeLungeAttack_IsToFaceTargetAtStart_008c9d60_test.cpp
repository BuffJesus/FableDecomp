#include <cstdio>
bool __fastcall CCreatureAction_HobbeLungeAttack_IsToFaceTargetAtStart()
{
    return true;
}
int main()
{
    if (CCreatureAction_HobbeLungeAttack_IsToFaceTargetAtStart() == true) { std::printf("AUTO_TINY_008c9d60_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008c9d60_TEST FAIL\n");
    return 1;
}