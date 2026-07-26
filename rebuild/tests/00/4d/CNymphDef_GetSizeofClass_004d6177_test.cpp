#include <cstdio>
int __fastcall CNymphDef_GetSizeofClass()
{
    return 80;
}
int main()
{
    if (CNymphDef_GetSizeofClass() == 80) { std::printf("AUTO_TINY_004d6177_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004d6177_TEST FAIL\n");
    return 1;
}