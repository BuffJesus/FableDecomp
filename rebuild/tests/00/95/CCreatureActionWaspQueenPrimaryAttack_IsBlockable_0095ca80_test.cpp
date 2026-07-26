#include <cstdio>
bool __fastcall CCreatureAction_WaspQueenPrimaryAttack_IsBlockable()
{
    return true;
}
int main()
{
    if (CCreatureAction_WaspQueenPrimaryAttack_IsBlockable() == true) { std::printf("AUTO_TINY_0095ca80_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0095ca80_TEST FAIL\n");
    return 1;
}