#include <cstdio>
int __fastcall CThingDrainLifeShotDef_GetSizeofClass()
{
    return 60;
}
int main()
{
    if (CThingDrainLifeShotDef_GetSizeofClass() == 60) { std::printf("AUTO_TINY_004d53a4_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004d53a4_TEST FAIL\n");
    return 1;
}