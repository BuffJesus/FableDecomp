#include <cstdio>
int __fastcall CArmourDef_GetSizeofClass()
{
    return 108;
}
int main()
{
    if (CArmourDef_GetSizeofClass() == 108) { std::printf("AUTO_TINY_0045859d_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0045859d_TEST FAIL\n");
    return 1;
}