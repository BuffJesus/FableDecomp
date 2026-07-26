#include <cstdio>
int __fastcall CAnimComponentStrikeResponseAnim_GetType()
{
    return 12;
}
int main()
{
    if (CAnimComponentStrikeResponseAnim_GetType() == 12) { std::printf("AUTO_TINY_005df920_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_005df920_TEST FAIL\n");
    return 1;
}