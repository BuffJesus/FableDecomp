#include <cstdio>
int __fastcall CSoundAtmospheresDef_GetSizeofClass()
{
    return 52;
}
int main()
{
    if (CSoundAtmospheresDef_GetSizeofClass() == 52) { std::printf("AUTO_TINY_004e1765_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004e1765_TEST FAIL\n");
    return 1;
}