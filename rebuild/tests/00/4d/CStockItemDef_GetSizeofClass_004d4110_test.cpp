#include <cstdio>
int __fastcall CStockItemDef_GetSizeofClass()
{
    return 64;
}
int main()
{
    if (CStockItemDef_GetSizeofClass() == 64) { std::printf("AUTO_TINY_004d4110_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004d4110_TEST FAIL\n");
    return 1;
}