#include <cstdio>
bool __fastcall CCreatureAction_Die_IsPreventingQuickAccessItemUsage()
{
    return true;
}
int main()
{
    if (CCreatureAction_Die_IsPreventingQuickAccessItemUsage() == true) { std::printf("AUTO_TINY_008edb80_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008edb80_TEST FAIL\n");
    return 1;
}