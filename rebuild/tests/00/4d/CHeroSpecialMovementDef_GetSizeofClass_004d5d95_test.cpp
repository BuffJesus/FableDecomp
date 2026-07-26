#include <cstdio>
int __fastcall CHeroSpecialMovementDef_GetSizeofClass()
{
    return 56;
}
int main()
{
    if (CHeroSpecialMovementDef_GetSizeofClass() == 56) { std::printf("AUTO_TINY_004d5d95_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004d5d95_TEST FAIL\n");
    return 1;
}