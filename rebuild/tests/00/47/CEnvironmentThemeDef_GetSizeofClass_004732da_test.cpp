#include <cstdio>
int __fastcall CEnvironmentThemeDef_GetSizeofClass()
{
    return 196;
}
int main()
{
    if (CEnvironmentThemeDef_GetSizeofClass() == 196) { std::printf("AUTO_TINY_004732da_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004732da_TEST FAIL\n");
    return 1;
}