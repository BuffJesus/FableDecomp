#include <cstdio>
int __fastcall CPlayerDef_GetSizeofClass()
{
    return 68;
}
int main()
{
    if (CPlayerDef_GetSizeofClass() == 68) { std::printf("AUTO_TINY_0045957e_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0045957e_TEST FAIL\n");
    return 1;
}