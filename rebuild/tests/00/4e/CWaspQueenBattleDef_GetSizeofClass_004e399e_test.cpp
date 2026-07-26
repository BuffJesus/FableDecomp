#include <cstdio>
int __fastcall CWaspQueenBattleDef_GetSizeofClass()
{
    return 64;
}
int main()
{
    if (CWaspQueenBattleDef_GetSizeofClass() == 64) { std::printf("AUTO_TINY_004e399e_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004e399e_TEST FAIL\n");
    return 1;
}