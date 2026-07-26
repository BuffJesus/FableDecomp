#include <cstdio>
int __fastcall CAnimComponentCombatComboChain_GetType()
{
    return 4;
}
int main()
{
    if (CAnimComponentCombatComboChain_GetType() == 4) { std::printf("AUTO_TINY_005df6c0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_005df6c0_TEST FAIL\n");
    return 1;
}