#include <cstdio>
bool __fastcall CCombatSequence_WillForcePush_IsToPreferSequenceWhenValid()
{
    return true;
}
int main()
{
    if (CCombatSequence_WillForcePush_IsToPreferSequenceWhenValid() == true) { std::printf("AUTO_TINY_008b52b0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b52b0_TEST FAIL\n");
    return 1;
}