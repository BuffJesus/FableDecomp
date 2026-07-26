#include <cstdio>
bool __fastcall CCreatureAction_HobbeLungeAttack_IsBlockable()
{
    return true;
}
int main()
{
    if (CCreatureAction_HobbeLungeAttack_IsBlockable() == true) { std::printf("AUTO_TINY_008c9d50_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008c9d50_TEST FAIL\n");
    return 1;
}