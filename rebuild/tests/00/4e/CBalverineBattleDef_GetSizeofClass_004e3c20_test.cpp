#include <cstdio>
int __fastcall CBalverineBattleDef_GetSizeofClass()
{
    return 72;
}
int main()
{
    if (CBalverineBattleDef_GetSizeofClass() == 72) { std::printf("AUTO_TINY_004e3c20_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004e3c20_TEST FAIL\n");
    return 1;
}