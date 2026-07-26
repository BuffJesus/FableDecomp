#include <cstdio>
int __fastcall CIdleSchedulerDef_GetSizeofClass()
{
    return 72;
}
int main()
{
    if (CIdleSchedulerDef_GetSizeofClass() == 72) { std::printf("AUTO_TINY_004e3f3b_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004e3f3b_TEST FAIL\n");
    return 1;
}