#include <cstdio>
bool __fastcall CCreatureAction_InterruptableMidAttack_IsPartOfComboChain()
{
    return true;
}
int main()
{
    if (CCreatureAction_InterruptableMidAttack_IsPartOfComboChain() == true) { std::printf("AUTO_TINY_006f58b0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_006f58b0_TEST FAIL\n");
    return 1;
}