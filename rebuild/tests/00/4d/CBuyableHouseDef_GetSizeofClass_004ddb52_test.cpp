#include <cstdio>
int __fastcall CBuyableHouseDef_GetSizeofClass()
{
    return 76;
}
int main()
{
    if (CBuyableHouseDef_GetSizeofClass() == 76) { std::printf("AUTO_TINY_004ddb52_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004ddb52_TEST FAIL\n");
    return 1;
}