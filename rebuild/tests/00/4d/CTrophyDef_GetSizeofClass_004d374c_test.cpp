#include <cstdio>
int __fastcall CTrophyDef_GetSizeofClass()
{
    return 100;
}
int main()
{
    if (CTrophyDef_GetSizeofClass() == 100) { std::printf("AUTO_TINY_004d374c_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004d374c_TEST FAIL\n");
    return 1;
}