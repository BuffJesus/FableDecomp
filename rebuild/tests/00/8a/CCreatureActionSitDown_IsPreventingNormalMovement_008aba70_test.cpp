#include <cstdio>
bool __fastcall CCreatureAction_SitDown_IsPreventingNormalMovement()
{
    return true;
}
int main()
{
    if (CCreatureAction_SitDown_IsPreventingNormalMovement() == true) { std::printf("AUTO_TINY_008aba70_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008aba70_TEST FAIL\n");
    return 1;
}