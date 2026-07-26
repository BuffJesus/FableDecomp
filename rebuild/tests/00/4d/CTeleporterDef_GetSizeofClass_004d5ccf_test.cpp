#include <cstdio>
int __fastcall CTeleporterDef_GetSizeofClass()
{
    return 52;
}
int main()
{
    if (CTeleporterDef_GetSizeofClass() == 52) { std::printf("AUTO_TINY_004d5ccf_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004d5ccf_TEST FAIL\n");
    return 1;
}