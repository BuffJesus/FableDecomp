#include <cstdio>
bool __fastcall CCreatureAction_UseObject_IsUsingForcedMovement()
{
    return true;
}
int main()
{
    if (CCreatureAction_UseObject_IsUsingForcedMovement() == true) { std::printf("AUTO_TINY_006a0830_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_006a0830_TEST FAIL\n");
    return 1;
}