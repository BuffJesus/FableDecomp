#include <cstdio>
int __fastcall CScorpionKingBattleDef_GetSizeofClass()
{
    return 96;
}
int main()
{
    if (CScorpionKingBattleDef_GetSizeofClass() == 96) { std::printf("AUTO_TINY_004e3adc_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004e3adc_TEST FAIL\n");
    return 1;
}