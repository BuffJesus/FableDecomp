#include <cstdio>
int __fastcall CBossDef_GetSizeofClass()
{
    return 84;
}
int main()
{
    if (CBossDef_GetSizeofClass() == 84) { std::printf("AUTO_TINY_004de8f1_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004de8f1_TEST FAIL\n");
    return 1;
}