#include <cstdio>
int __fastcall CReadableDef_GetSizeofClass()
{
    return 38;
}
int main()
{
    if (CReadableDef_GetSizeofClass() == 38) { std::printf("AUTO_TINY_004d7ae5_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004d7ae5_TEST FAIL\n");
    return 1;
}