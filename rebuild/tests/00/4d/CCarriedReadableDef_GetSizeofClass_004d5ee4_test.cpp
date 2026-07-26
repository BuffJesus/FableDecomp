#include <cstdio>
int __fastcall CCarriedReadableDef_GetSizeofClass()
{
    return 48;
}
int main()
{
    if (CCarriedReadableDef_GetSizeofClass() == 48) { std::printf("AUTO_TINY_004d5ee4_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004d5ee4_TEST FAIL\n");
    return 1;
}