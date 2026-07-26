#include <cstdio>
int __fastcall CDrunkennessDef_GetSizeofClass()
{
    return 44;
}
int main()
{
    if (CDrunkennessDef_GetSizeofClass() == 44) { std::printf("AUTO_TINY_004d52fa_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004d52fa_TEST FAIL\n");
    return 1;
}