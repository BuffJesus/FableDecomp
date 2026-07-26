#include <cstdio>
int __fastcall CAbilityTrackDef_GetSizeofClass()
{
    return 80;
}
int main()
{
    if (CAbilityTrackDef_GetSizeofClass() == 80) { std::printf("AUTO_TINY_00478702_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00478702_TEST FAIL\n");
    return 1;
}