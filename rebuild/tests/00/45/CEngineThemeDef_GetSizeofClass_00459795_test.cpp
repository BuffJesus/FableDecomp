#include <cstdio>
int __fastcall CEngineThemeDef_GetSizeofClass()
{
    return 164;
}
int main()
{
    if (CEngineThemeDef_GetSizeofClass() == 164) { std::printf("AUTO_TINY_00459795_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00459795_TEST FAIL\n");
    return 1;
}