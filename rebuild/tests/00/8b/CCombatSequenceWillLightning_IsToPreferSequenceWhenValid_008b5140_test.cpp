#include <cstdio>
bool __fastcall CCombatSequence_WillLightning_IsToPreferSequenceWhenValid()
{
    return true;
}
int main()
{
    if (CCombatSequence_WillLightning_IsToPreferSequenceWhenValid() == true) { std::printf("AUTO_TINY_008b5140_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b5140_TEST FAIL\n");
    return 1;
}