#include <cstdio>
int __fastcall CTavernDef_GetSizeofClass()
{
    return 44;
}
int main()
{
    if (CTavernDef_GetSizeofClass() == 44) { std::printf("AUTO_TINY_004d5211_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004d5211_TEST FAIL\n");
    return 1;
}