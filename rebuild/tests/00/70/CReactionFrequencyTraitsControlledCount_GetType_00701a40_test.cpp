#include <cstdio>
int __fastcall CReactionFrequencyTraits_ControlledCount_GetType()
{
    return 3;
}
int main()
{
    if (CReactionFrequencyTraits_ControlledCount_GetType() == 3) { std::printf("AUTO_TINY_00701a40_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00701a40_TEST FAIL\n");
    return 1;
}