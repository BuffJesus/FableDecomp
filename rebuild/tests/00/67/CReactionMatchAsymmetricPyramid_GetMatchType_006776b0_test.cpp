#include <cstdio>
int __fastcall CReactionMatch_AsymmetricPyramid_GetMatchType()
{
    return 2;
}
int main()
{
    if (CReactionMatch_AsymmetricPyramid_GetMatchType() == 2) { std::printf("AUTO_TINY_006776b0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_006776b0_TEST FAIL\n");
    return 1;
}