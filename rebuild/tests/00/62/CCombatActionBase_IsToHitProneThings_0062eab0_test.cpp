#include <cstdio>
bool __fastcall CCombatActionBase_IsToHitProneThings()
{
    return false;
}
int main()
{
    if (CCombatActionBase_IsToHitProneThings() == false) { std::printf("AUTO_TINY_0062eab0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0062eab0_TEST FAIL\n");
    return 1;
}