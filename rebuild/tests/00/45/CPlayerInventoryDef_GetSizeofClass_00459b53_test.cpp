#include <cstdio>
int __fastcall CPlayerInventoryDef_GetSizeofClass()
{
    return 112;
}
int main()
{
    if (CPlayerInventoryDef_GetSizeofClass() == 112) { std::printf("AUTO_TINY_00459b53_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00459b53_TEST FAIL\n");
    return 1;
}