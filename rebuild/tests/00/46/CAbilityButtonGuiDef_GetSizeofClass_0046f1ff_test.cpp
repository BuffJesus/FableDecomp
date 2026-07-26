#include <cstdio>
int __fastcall CAbilityButtonGuiDef_GetSizeofClass()
{
    return 92;
}
int main()
{
    if (CAbilityButtonGuiDef_GetSizeofClass() == 92) { std::printf("AUTO_TINY_0046f1ff_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0046f1ff_TEST FAIL\n");
    return 1;
}