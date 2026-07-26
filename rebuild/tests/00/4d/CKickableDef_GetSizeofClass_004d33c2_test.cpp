#include <cstdio>
int __fastcall CKickableDef_GetSizeofClass()
{
    return 84;
}
int main()
{
    if (CKickableDef_GetSizeofClass() == 84) { std::printf("AUTO_TINY_004d33c2_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004d33c2_TEST FAIL\n");
    return 1;
}