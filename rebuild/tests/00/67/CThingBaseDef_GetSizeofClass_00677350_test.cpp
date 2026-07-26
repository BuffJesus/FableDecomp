#include <cstdio>
int __fastcall CThingBaseDef_GetSizeofClass()
{
    return 112;
}
int main()
{
    if (CThingBaseDef_GetSizeofClass() == 112) { std::printf("AUTO_TINY_00677350_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00677350_TEST FAIL\n");
    return 1;
}