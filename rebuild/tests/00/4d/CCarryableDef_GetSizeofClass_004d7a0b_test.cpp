#include <cstdio>
int __fastcall CCarryableDef_GetSizeofClass()
{
    return 80;
}
int main()
{
    if (CCarryableDef_GetSizeofClass() == 80) { std::printf("AUTO_TINY_004d7a0b_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004d7a0b_TEST FAIL\n");
    return 1;
}