#include <cstdio>
bool __fastcall CCreatureAction_UnblockableMidAttack_IsBlockable()
{
    return false;
}
int main()
{
    if (CCreatureAction_UnblockableMidAttack_IsBlockable() == false) { std::printf("AUTO_TINY_008bff20_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008bff20_TEST FAIL\n");
    return 1;
}