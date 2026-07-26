#include <cstdio>
int __fastcall CHighlightItemDef_GetSizeofClass()
{
    return 72;
}
int main()
{
    if (CHighlightItemDef_GetSizeofClass() == 72) { std::printf("AUTO_TINY_004d458a_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004d458a_TEST FAIL\n");
    return 1;
}