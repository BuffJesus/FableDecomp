#include <cstdio>
int __fastcall CReactionMatch_SecondaryAxisMRShifted_GetMatchType()
{
    return 0;
}
int main()
{
    if (CReactionMatch_SecondaryAxisMRShifted_GetMatchType() == 0) { std::printf("AUTO_TINY_006775a0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_006775a0_TEST FAIL\n");
    return 1;
}