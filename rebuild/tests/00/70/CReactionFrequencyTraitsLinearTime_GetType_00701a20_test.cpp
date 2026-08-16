#include <cstdio>
int __fastcall CReactionFrequencyTraits_LinearTime_GetType()
{
    return 1;
}
int main()
{
    if (CReactionFrequencyTraits_LinearTime_GetType() == 1) { std::printf("AUTO_TINY_00701a20_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00701a20_TEST FAIL\n");
    return 1;
}