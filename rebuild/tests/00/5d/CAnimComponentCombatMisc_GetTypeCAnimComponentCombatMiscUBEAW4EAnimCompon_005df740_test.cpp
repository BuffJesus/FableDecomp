#include <cstdio>
int __fastcall CAnimComponentCombatMisc_GetType()
{
    return 6;
}
int main()
{
    if (CAnimComponentCombatMisc_GetType() == 6) { std::printf("AUTO_TINY_005df740_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_005df740_TEST FAIL\n");
    return 1;
}