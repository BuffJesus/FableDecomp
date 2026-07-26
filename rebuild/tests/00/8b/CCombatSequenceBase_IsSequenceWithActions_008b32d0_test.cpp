#include <cstdio>
bool __fastcall CCombatSequenceBase_IsSequenceWithActions()
{
    return true;
}
int main()
{
    if (CCombatSequenceBase_IsSequenceWithActions() == true) { std::printf("AUTO_TINY_008b32d0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008b32d0_TEST FAIL\n");
    return 1;
}