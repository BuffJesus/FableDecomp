#include <cstdio>
bool __fastcall CCreatureAction_SitDown_IsPreventingLookTurning()
{
    return true;
}
int main()
{
    if (CCreatureAction_SitDown_IsPreventingLookTurning() == true) { std::printf("AUTO_TINY_008aba80_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008aba80_TEST FAIL\n");
    return 1;
}