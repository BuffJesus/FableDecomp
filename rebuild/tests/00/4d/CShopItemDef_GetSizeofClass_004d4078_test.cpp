#include <cstdio>
int __fastcall CShopItemDef_GetSizeofClass()
{
    return 72;
}
int main()
{
    if (CShopItemDef_GetSizeofClass() == 72) { std::printf("AUTO_TINY_004d4078_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004d4078_TEST FAIL\n");
    return 1;
}