#include <cstdio>
int __fastcall CQuickAccessItemButtonGuiDef_GetSizeofClass()
{
    return 80;
}
int main()
{
    if (CQuickAccessItemButtonGuiDef_GetSizeofClass() == 80) { std::printf("AUTO_TINY_0046ee66_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0046ee66_TEST FAIL\n");
    return 1;
}