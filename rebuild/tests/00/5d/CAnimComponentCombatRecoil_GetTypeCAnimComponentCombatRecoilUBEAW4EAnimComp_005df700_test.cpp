#include <cstdio>
int __fastcall CAnimComponentCombatRecoil_GetType()
{
    return 5;
}
int main()
{
    if (CAnimComponentCombatRecoil_GetType() == 5) { std::printf("AUTO_TINY_005df700_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_005df700_TEST FAIL\n");
    return 1;
}