#include <cstdio>
bool __fastcall CCreatureAction_DragonFireBlastBase_IsPreventingLookTurning()
{
    return false;
}
int main()
{
    if (CCreatureAction_DragonFireBlastBase_IsPreventingLookTurning() == false) { std::printf("AUTO_TINY_009536c0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_009536c0_TEST FAIL\n");
    return 1;
}