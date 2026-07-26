#include <cstdio>
int __fastcall CLensFlareElementDef_GetSizeofClass()
{
    return 76;
}
int main()
{
    if (CLensFlareElementDef_GetSizeofClass() == 76) { std::printf("AUTO_TINY_0046f723_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0046f723_TEST FAIL\n");
    return 1;
}