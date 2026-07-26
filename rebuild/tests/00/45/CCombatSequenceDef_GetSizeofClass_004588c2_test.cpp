#include <cstdio>
int __fastcall CCombatSequenceDef_GetSizeofClass()
{
    return 104;
}
int main()
{
    if (CCombatSequenceDef_GetSizeofClass() == 104) { std::printf("AUTO_TINY_004588c2_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004588c2_TEST FAIL\n");
    return 1;
}