#include <cstdio>
int __fastcall COccupiableDef_GetSizeofClass()
{
    return 44;
}
int main()
{
    if (COccupiableDef_GetSizeofClass() == 44) { std::printf("AUTO_TINY_004d4e78_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004d4e78_TEST FAIL\n");
    return 1;
}