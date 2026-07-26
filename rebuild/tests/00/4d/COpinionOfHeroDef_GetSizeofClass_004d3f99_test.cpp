#include <cstdio>
int __fastcall COpinionOfHeroDef_GetSizeofClass()
{
    return 60;
}
int main()
{
    if (COpinionOfHeroDef_GetSizeofClass() == 60) { std::printf("AUTO_TINY_004d3f99_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004d3f99_TEST FAIL\n");
    return 1;
}