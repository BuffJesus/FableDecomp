#include <cstdio>
bool __fastcall CCreatureAction_PlayAnimationWithLookTurning_IsPreventingLookTurning()
{
    return false;
}
int main()
{
    if (CCreatureAction_PlayAnimationWithLookTurning_IsPreventingLookTurning() == false) { std::printf("AUTO_TINY_0084ac60_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0084ac60_TEST FAIL\n");
    return 1;
}