#include <cstdio>
bool __fastcall CCreatureAction_BanditKingAttack_IsBlockable()
{
    return false;
}
int main()
{
    if (CCreatureAction_BanditKingAttack_IsBlockable() == false) { std::printf("AUTO_TINY_008c94a0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008c94a0_TEST FAIL\n");
    return 1;
}