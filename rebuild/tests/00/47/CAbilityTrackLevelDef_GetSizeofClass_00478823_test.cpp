#include <cstdio>
int __fastcall CAbilityTrackLevelDef_GetSizeofClass()
{
    return 72;
}
int main()
{
    if (CAbilityTrackLevelDef_GetSizeofClass() == 72) { std::printf("AUTO_TINY_00478823_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00478823_TEST FAIL\n");
    return 1;
}