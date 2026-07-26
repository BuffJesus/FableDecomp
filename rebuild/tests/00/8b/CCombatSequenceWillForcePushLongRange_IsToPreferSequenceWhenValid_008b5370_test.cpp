#include <cstdio>
bool __fastcall CCombatSequence_WillForcePushLongRange_IsToPreferSequenceWhenValid()
{
    return true;
}
int main()
{
    if (CCombatSequence_WillForcePushLongRange_IsToPreferSequenceWhenValid() == true) { std::printf("AUTO_TINY_008b5370_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b5370_TEST FAIL\n");
    return 1;
}