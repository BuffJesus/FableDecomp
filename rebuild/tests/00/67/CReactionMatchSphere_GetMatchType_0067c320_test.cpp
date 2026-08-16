#include <cstdio>
int __fastcall CReactionMatch_Sphere_GetMatchType()
{
    return 4;
}
int main()
{
    if (CReactionMatch_Sphere_GetMatchType() == 4) { std::printf("AUTO_TINY_0067c320_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0067c320_TEST FAIL\n");
    return 1;
}