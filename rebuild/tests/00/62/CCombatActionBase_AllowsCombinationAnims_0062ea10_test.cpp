#include <cstdio>
bool __fastcall CCombatActionBase_AllowsCombinationAnims()
{
    return false;
}
int main()
{
    if (CCombatActionBase_AllowsCombinationAnims() == false) { std::printf("AUTO_TINY_0062ea10_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0062ea10_TEST FAIL\n");
    return 1;
}