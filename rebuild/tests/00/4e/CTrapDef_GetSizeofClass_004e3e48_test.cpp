#include <cstdio>
int __fastcall CTrapDef_GetSizeofClass()
{
    return 100;
}
int main()
{
    if (CTrapDef_GetSizeofClass() == 100) { std::printf("AUTO_TINY_004e3e48_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004e3e48_TEST FAIL\n");
    return 1;
}