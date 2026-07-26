#include <cstdio>
int __fastcall CTavernGameCoinBaseDef_GetSizeofClass()
{
    return 68;
}
int main()
{
    if (CTavernGameCoinBaseDef_GetSizeofClass() == 68) { std::printf("AUTO_TINY_004d5991_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004d5991_TEST FAIL\n");
    return 1;
}