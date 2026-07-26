#include <cstdio>
int __fastcall CParticleAttacherDef_GetSizeofClass()
{
    return 52;
}
int main()
{
    if (CParticleAttacherDef_GetSizeofClass() == 52) { std::printf("AUTO_TINY_004e0bb9_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004e0bb9_TEST FAIL\n");
    return 1;
}