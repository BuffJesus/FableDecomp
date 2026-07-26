#include <cstdio>
int __fastcall CTavernGameDef_GetSizeofClass()
{
    return 420;
}
int main()
{
    if (CTavernGameDef_GetSizeofClass() == 420) { std::printf("AUTO_TINY_004e1086_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004e1086_TEST FAIL\n");
    return 1;
}