#include <cstdio>
int __fastcall CBuyHouseDef_GetSizeofClass()
{
    return 38;
}
int main()
{
    if (CBuyHouseDef_GetSizeofClass() == 38) { std::printf("AUTO_TINY_004d325d_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004d325d_TEST FAIL\n");
    return 1;
}