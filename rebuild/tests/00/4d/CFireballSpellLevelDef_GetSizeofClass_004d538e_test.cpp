#include <cstdio>
int __fastcall CFireballSpellLevelDef_GetSizeofClass()
{
    return 44;
}
int main()
{
    if (CFireballSpellLevelDef_GetSizeofClass() == 44) { std::printf("AUTO_TINY_004d538e_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004d538e_TEST FAIL\n");
    return 1;
}