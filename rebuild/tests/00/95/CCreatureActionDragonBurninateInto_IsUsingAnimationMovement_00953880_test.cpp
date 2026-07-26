#include <cstdio>
bool __fastcall CCreatureAction_DragonBurninateInto_IsUsingAnimationMovement()
{
    return false;
}
int main()
{
    if (CCreatureAction_DragonBurninateInto_IsUsingAnimationMovement() == false) { std::printf("AUTO_TINY_00953880_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00953880_TEST FAIL\n");
    return 1;
}