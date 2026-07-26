#include <cstdio>
bool __fastcall CCombatAction_StabThing_IsUsingActionMovement()
{
    return true;
}
int main()
{
    if (CCombatAction_StabThing_IsUsingActionMovement() == true) { std::printf("AUTO_TINY_00880040_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00880040_TEST FAIL\n");
    return 1;
}