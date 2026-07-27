#include <cstdio>
bool __fastcall CCreatureAction_UseObject_IsPreventingLookTurning()
{
    return false;
}
int main()
{
    if (CCreatureAction_UseObject_IsPreventingLookTurning() == false) { std::printf("AUTO_TINY_006a0870_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_006a0870_TEST FAIL\n");
    return 1;
}