#include <cstdio>
int __fastcall CReactionMatch_SecondaryAxisCentredMRShifted_GetMatchType()
{
    return 1;
}
int main()
{
    if (CReactionMatch_SecondaryAxisCentredMRShifted_GetMatchType() == 1) { std::printf("AUTO_TINY_00677610_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00677610_TEST FAIL\n");
    return 1;
}