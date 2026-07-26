#include <cstdio>
int __fastcall CBedDef_GetSizeofClass()
{
    return 60;
}
int main()
{
    if (CBedDef_GetSizeofClass() == 60) { std::printf("AUTO_TINY_004d7a46_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004d7a46_TEST FAIL\n");
    return 1;
}