#include <cstdio>
int __fastcall CBlurredAttitudesDef_GetSizeofClass()
{
    return 72;
}
int main()
{
    if (CBlurredAttitudesDef_GetSizeofClass() == 72) { std::printf("AUTO_TINY_0067c510_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0067c510_TEST FAIL\n");
    return 1;
}