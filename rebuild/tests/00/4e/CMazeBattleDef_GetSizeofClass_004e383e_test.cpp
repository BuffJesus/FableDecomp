#include <cstdio>
int __fastcall CMazeBattleDef_GetSizeofClass()
{
    return 96;
}
int main()
{
    if (CMazeBattleDef_GetSizeofClass() == 96) { std::printf("AUTO_TINY_004e383e_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004e383e_TEST FAIL\n");
    return 1;
}