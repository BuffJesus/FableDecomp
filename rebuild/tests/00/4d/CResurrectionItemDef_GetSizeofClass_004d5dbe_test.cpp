#include <cstdio>
int __fastcall CResurrectionItemDef_GetSizeofClass()
{
    return 44;
}
int main()
{
    if (CResurrectionItemDef_GetSizeofClass() == 44) { std::printf("AUTO_TINY_004d5dbe_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004d5dbe_TEST FAIL\n");
    return 1;
}