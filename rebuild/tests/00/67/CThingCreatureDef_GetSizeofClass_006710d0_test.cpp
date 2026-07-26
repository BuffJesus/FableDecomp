#include <cstdio>
int __fastcall CThingCreatureDef_GetSizeofClass()
{
    return 332;
}
int main()
{
    if (CThingCreatureDef_GetSizeofClass() == 332) { std::printf("AUTO_TINY_006710d0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_006710d0_TEST FAIL\n");
    return 1;
}