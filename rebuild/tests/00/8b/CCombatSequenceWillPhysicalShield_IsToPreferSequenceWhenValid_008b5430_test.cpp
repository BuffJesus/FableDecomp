#include <cstdio>
bool __fastcall CCombatSequence_WillPhysicalShield_IsToPreferSequenceWhenValid()
{
    return true;
}
int main()
{
    if (CCombatSequence_WillPhysicalShield_IsToPreferSequenceWhenValid() == true) { std::printf("AUTO_TINY_008b5430_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b5430_TEST FAIL\n");
    return 1;
}