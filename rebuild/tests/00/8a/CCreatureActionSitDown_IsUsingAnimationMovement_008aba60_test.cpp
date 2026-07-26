#include <cstdio>
bool __fastcall CCreatureAction_SitDown_IsUsingAnimationMovement()
{
    return true;
}
int main()
{
    if (CCreatureAction_SitDown_IsUsingAnimationMovement() == true) { std::printf("AUTO_TINY_008aba60_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008aba60_TEST FAIL\n");
    return 1;
}