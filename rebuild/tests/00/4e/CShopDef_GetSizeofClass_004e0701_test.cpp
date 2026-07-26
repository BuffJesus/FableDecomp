#include <cstdio>
int __fastcall CShopDef_GetSizeofClass()
{
    return 208;
}
int main()
{
    if (CShopDef_GetSizeofClass() == 208) { std::printf("AUTO_TINY_004e0701_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004e0701_TEST FAIL\n");
    return 1;
}