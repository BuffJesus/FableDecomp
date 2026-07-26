#include <cstdio>
int __fastcall CAnimComponentCombatFlourishTargetOffset_GetType()
{
    return 7;
}
int main()
{
    if (CAnimComponentCombatFlourishTargetOffset_GetType() == 7) { std::printf("AUTO_TINY_005df780_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_005df780_TEST FAIL\n");
    return 1;
}