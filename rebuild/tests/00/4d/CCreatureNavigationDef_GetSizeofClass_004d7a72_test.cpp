#include <cstdio>
int __fastcall CCreatureNavigationDef_GetSizeofClass()
{
    return 56;
}
int main()
{
    if (CCreatureNavigationDef_GetSizeofClass() == 56) { std::printf("AUTO_TINY_004d7a72_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004d7a72_TEST FAIL\n");
    return 1;
}