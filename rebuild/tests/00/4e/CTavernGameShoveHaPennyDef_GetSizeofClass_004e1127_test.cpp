#include <cstdio>
int __fastcall CTavernGameShoveHaPennyDef_GetSizeofClass()
{
    return 512;
}
int main()
{
    if (CTavernGameShoveHaPennyDef_GetSizeofClass() == 512) { std::printf("AUTO_TINY_004e1127_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004e1127_TEST FAIL\n");
    return 1;
}