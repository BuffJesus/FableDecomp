#include <cstdio>
bool __fastcall CCombatSequenceBase_IsToPreferSequenceWhenValid()
{
    return false;
}
int main()
{
    if (CCombatSequenceBase_IsToPreferSequenceWhenValid() == false) { std::printf("AUTO_TINY_008b32c0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b32c0_TEST FAIL\n");
    return 1;
}