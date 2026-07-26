#include <cstdio>
int __fastcall CWillResponseDef_GetSizeofClass()
{
    return 45;
}
int main()
{
    if (CWillResponseDef_GetSizeofClass() == 45) { std::printf("AUTO_TINY_004d649f_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004d649f_TEST FAIL\n");
    return 1;
}