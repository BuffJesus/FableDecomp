#include <cstdio>
int __fastcall CTavernTableDef_GetSizeofClass()
{
    return 39;
}
int main()
{
    if (CTavernTableDef_GetSizeofClass() == 39) { std::printf("AUTO_TINY_004d513e_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004d513e_TEST FAIL\n");
    return 1;
}