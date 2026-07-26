#include <cstdio>
int __fastcall CEngineThemeGroupDef_GetSizeofClass()
{
    return 58;
}
int main()
{
    if (CEngineThemeGroupDef_GetSizeofClass() == 58) { std::printf("AUTO_TINY_00459760_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00459760_TEST FAIL\n");
    return 1;
}