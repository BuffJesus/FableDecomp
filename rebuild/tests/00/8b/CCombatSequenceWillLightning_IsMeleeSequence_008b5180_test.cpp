#include <cstdio>
bool __fastcall CCombatSequence_WillLightning_IsMeleeSequence()
{
    return true;
}
int main()
{
    if (CCombatSequence_WillLightning_IsMeleeSequence() == true) { std::printf("AUTO_TINY_008b5180_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b5180_TEST FAIL\n");
    return 1;
}