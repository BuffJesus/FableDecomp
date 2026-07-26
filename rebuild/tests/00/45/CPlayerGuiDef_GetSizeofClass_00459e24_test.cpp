#include <cstdio>
int __fastcall CPlayerGuiDef_GetSizeofClass()
{
    return 2740;
}
int main()
{
    if (CPlayerGuiDef_GetSizeofClass() == 2740) { std::printf("AUTO_TINY_00459e24_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00459e24_TEST FAIL\n");
    return 1;
}