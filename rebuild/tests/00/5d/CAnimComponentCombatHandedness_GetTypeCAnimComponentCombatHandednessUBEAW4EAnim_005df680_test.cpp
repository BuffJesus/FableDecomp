#include <cstdio>
int __fastcall CAnimComponentCombatHandedness_GetType()
{
    return 3;
}
int main()
{
    if (CAnimComponentCombatHandedness_GetType() == 3) { std::printf("AUTO_TINY_005df680_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_005df680_TEST FAIL\n");
    return 1;
}