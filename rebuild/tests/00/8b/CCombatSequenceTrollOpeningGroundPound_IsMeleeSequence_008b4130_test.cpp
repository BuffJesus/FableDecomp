#include <cstdio>
bool __fastcall CCombatSequence_TrollOpeningGroundPound_IsMeleeSequence()
{
    return true;
}
int main()
{
    if (CCombatSequence_TrollOpeningGroundPound_IsMeleeSequence() == true) { std::printf("AUTO_TINY_008b4130_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b4130_TEST FAIL\n");
    return 1;
}