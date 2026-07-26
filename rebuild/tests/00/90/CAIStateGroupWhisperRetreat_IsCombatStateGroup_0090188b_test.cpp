#include <cstdio>
bool __fastcall CAIStateGroup_WhisperRetreat_IsCombatStateGroup()
{
    return false;
}
int main()
{
    if (CAIStateGroup_WhisperRetreat_IsCombatStateGroup() == false) { std::printf("AUTO_TINY_0090188b_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0090188b_TEST FAIL\n");
    return 1;
}