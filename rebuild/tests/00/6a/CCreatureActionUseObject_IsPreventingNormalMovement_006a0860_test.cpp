#include <cstdio>
bool __fastcall CCreatureAction_UseObject_IsPreventingNormalMovement()
{
    return true;
}
int main()
{
    if (CCreatureAction_UseObject_IsPreventingNormalMovement() == true) { std::printf("AUTO_TINY_006a0860_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_006a0860_TEST FAIL\n");
    return 1;
}