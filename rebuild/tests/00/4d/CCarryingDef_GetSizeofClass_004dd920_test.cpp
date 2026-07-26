#include <cstdio>
int __fastcall CCarryingDef_GetSizeofClass()
{
    return 56;
}
int main()
{
    if (CCarryingDef_GetSizeofClass() == 56) { std::printf("AUTO_TINY_004dd920_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004dd920_TEST FAIL\n");
    return 1;
}