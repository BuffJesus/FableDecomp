#include <cstdio>
int __fastcall CSpotLightDef_GetSizeofClass()
{
    return 68;
}
int main()
{
    if (CSpotLightDef_GetSizeofClass() == 68) { std::printf("AUTO_TINY_004d341e_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004d341e_TEST FAIL\n");
    return 1;
}