#include <cstdio>
int __fastcall CGlobalDef_GetSizeofClass()
{
    return 336;
}
int main()
{
    if (CGlobalDef_GetSizeofClass() == 336) { std::printf("AUTO_TINY_00458a2d_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00458a2d_TEST FAIL\n");
    return 1;
}